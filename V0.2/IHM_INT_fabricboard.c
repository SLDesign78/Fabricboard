/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  INT_fabricboard.c
*************************************************************************************************************/
//  Description  : Interrupt Compule Module Fabricboard
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 15/01/2023: creation

*************************************************************************************************************/

#include "fabricboard.h"


/****************************************************
   		USER DECLARATIONS
****************************************************/


//******* STRUCTURES *******

/*------------------------- GPIOD STRUCT ---------------------------*/
//INT I2C EXPANDER
struct gpiod_chip *chip_INT_I2CEXP;
struct gpiod_line *line_INT_I2CEXP;
struct gpiod_line_event event_INT_I2CEXP;

//INT FAN ALERT
struct gpiod_chip *chip_FAN_ALERT;
struct gpiod_line *line_FAN_ALERT;
struct gpiod_line_event event_FAN_ALERT;



/*------------------------- SLOT CFG STRUCT ---------------------------*/
/*
Num				: Slot Number [1-7]
Type            : 0:Host, 1:Downstream
Reset_dir       : 0:input, 1:output
Pcie_refclk_en  : 0:disabled, 1:enabled
Prsnt_Status    : 0:Card no present, 1:Card present
*/


Slot_CfgTypeDef Slot1_Cfg;
Slot_CfgTypeDef Slot2_Cfg;
Slot_CfgTypeDef Slot3_Cfg;
Slot_CfgTypeDef Slot4_Cfg;
Slot_CfgTypeDef Slot5_Cfg;
Slot_CfgTypeDef Slot6_Cfg;
Slot_CfgTypeDef Slot7_Cfg;


/*------------------------- SWITCH CFG STRUCT ---------------------------*/
/*
Host_Port	: Host Port number
*/

Switch_CfgDef Sw_Cfg;



//------------ I2C File Description ------------
int fd_MCP23017_EXP0_i2c;
int fd_MCP23017_EXP1_i2c;
int fd_PCA9545A_MUX4_i2c;
int fd_PCA9548A_MUX8_i2c;
int fd_SHT41_i2c;
int fd_EMC2302_i2c;
int fd_PEX_i2c;
int fd_ISL28022_i2c;


//----- I2C Device -----
char* i2cDevice = "/dev/i2c-2";
//char* i2cDevice_test = "/dev/i2c-1";


/*------- THREAD -------*/
pthread_t thr1, thr2, thr3, thr4;


/*------- SLOT Type config file -------*/
char* File_slot_type = "slot_type.cfg";

/*------------------------------REGISTERS----------------------------------*/

//FW Version
const uint8_t Version_Major = VERSION_MAJOR;
const uint8_t Version_Minor = VERSION_MINOR;


//SYSTEM REG
volatile uint8_t SYS_STATUS= 0x00;
volatile uint8_t PWR_STATUS= 0x00;
volatile uint8_t REFCLK_STATUS= 0x00;


//FSM_PWR_CTRL_REG
volatile uint8_t PWR_EN_REG = 0x00;
volatile uint8_t PWR_PG_REG;

//FSM_PWR
volatile uint8_t FSM_PWR_CTRL_EN = 0x00;
volatile uint8_t FSM_PWR_CTRL_OK;
volatile uint8_t FSM_PWR_CURR;
volatile uint8_t FSM_PWR_ERR;
volatile uint8_t FSM_PWR_STATUS;

//ALERT REG
volatile uint8_t FAN_ALERT_REG;
volatile uint8_t TEMP_ALERT_REG;
volatile uint8_t MSC_ALERT_ERR_REG;

//SLOT TYPE
volatile uint8_t SLOT_TYPE_REG;

//PRESENCE REG
volatile uint8_t SLOT_PRSNT;
volatile uint8_t MSC_PRSNT;

//CLOCK CFG
volatile uint8_t SLOT_CLK_CFG;
volatile uint8_t MSC_CLK_CFG;

//PCIe RESET CTRL/DIR/STATUS
volatile uint8_t SYS_PERST_MODE_CTRL;
volatile uint8_t SYS_PERST_CTRL = 0x00;
volatile uint8_t SLOT_PERST_DIR;
volatile uint8_t SLOT_PERST_STATUS;
volatile uint8_t MSC_PERST_STATUS;


//Board TEMPERATURE SENSORS
volatile uint16_t BRD_TEMP_AMB;
volatile uint8_t BRD_TEMP_CR_REG;
volatile uint8_t BRD_TEMP_UP_REG;
volatile uint8_t BRD_TEMP_LO_REG;

//PEX TEMPERATURE
volatile uint16_t PEX_TEMP;

//PEX FIRMWARE REVISION
volatile uint16_t PEX_FW_REV;

//VR TEMPERATURE REG
volatile uint16_t VR_TEMP_CHA;
volatile uint16_t VR_TEMP_CHB;

//VR U/I/P MEASURES REG
volatile uint16_t VR_VIN_CHA;
volatile uint16_t VR_VIN_CHB;
volatile uint16_t VR_VOUT_CHA;
volatile uint16_t VR_VOUT_CHB;
volatile uint16_t VR_IOUT_CHA;
volatile uint16_t VR_IOUT_CHB;
volatile uint16_t VR_POUT_CHA;
volatile uint16_t VR_POUT_CHB;

//VR STATUS REG
volatile uint16_t VR_STATUS_WORD_CHA;
volatile uint16_t VR_STATUS_WORD_CHB;
volatile uint8_t VR_STATUS_VOUT_CHA;
volatile uint8_t VR_STATUS_VOUT_CHB;
volatile uint8_t VR_STATUS_IOUT_CHA;
volatile uint8_t VR_STATUS_IOUT_CHB;
volatile uint8_t VR_STATUS_TEMP_CHA;
volatile uint8_t VR_STATUS_TEMP_CHB;

//ISL28022 U/I/P MEASURES REG
volatile uint8_t ISL_V_MEAS_REG1;
volatile uint8_t ISL_V_MEAS_REG2;
volatile uint8_t ISL_I_MEAS_REG1;
volatile uint8_t ISL_I_MEAS_REG2;
volatile uint8_t ISL_P_MEAS_REG1;
volatile uint8_t ISL_P_MEAS_REG2;





/*------------------------------SYSTEM INIT----------------------------------*/
volatile uint8_t INIT_READY=0;


/*------------------------------POWER CONTROL----------------------------------*/

//Power Control
volatile uint8_t SYS_PWR_EN=0x00, SYS_PWR_OK=0x00;

//Power Supply Control
volatile uint8_t SYS_P3V3_EN=0x00, SYS_P0V9_EN=0x00, SYS_P1V8_EN=0x00;

//Emergency Power Off
volatile uint8_t SYS_PWR_EPO=0x00;

/*State machine initial state*/
volatile uint8_t ucPWR_STATE = ST_INIT;

/*Timeout*/
volatile uint32_t FSM_VR_Timeout = VR_PG_TIMEOUT;


/*--------------------------PCIe REFClock CONTROL----------------------------------*/

//Refclock Control
volatile uint8_t REFCLK_ON=0x00;

/*State machine initial state*/
volatile uint8_t ucREFCLK_STATE = ST_REFCLK_OFF;


/*----------------------------PCIe RESET CONTROL----------------------------------*/

//System Reset Control for all slots an devices
/* 0x00: Asserted
 * 0x01: Deasserted
*/
volatile uint8_t SYS_PERST=0x00; 	//default: PERST asserted


//System Reset Mode Control
/* 0x00: AUTO
 * 0x01: MANUAL (action from user to release reset is required after power up)
*/
volatile uint8_t SYS_PERST_MODE=0x00; 	//default: Mode Auto


//Reset Control
volatile uint8_t RST_RELEASE=0x00;

/*State machine initial state*/
volatile uint8_t ucRST_STATE = ST_RST_INIT;


/*--------------------------SLOT TYPE CONFIG----------------------------------*/
/* 0x00: HOST
 * 0x01: DOWNSTREAM (End Point)
 *
 * 	SLOT_TYPE_CFG[0]: SLOT1;
 *	SLOT_TYPE_CFG[1]: SLOT2;
 *	SLOT_TYPE_CFG[2]: SLOT3;
 *	SLOT_TYPE_CFG[3]: SLOT4;
 *	SLOT_TYPE_CFG[4]: SLOT5;
 *	SLOT_TYPE_CFG[5]: SLOT6;
 *	SLOT_TYPE_CFG[6]: SLOT7;
 */

volatile uint8_t SLOT_TYPE_CFG[7];



//Interrupt I2C EXP
uint8_t Reg_INTFA;	//PortA
uint8_t Reg_INTCAPA; //PortA
uint8_t Reg_INTFB;	//PortB
uint8_t Reg_INTCAPB; //PortB


//Sensors
float temperature;
float humidity;


//IHM
char CMD_USER[CMD_MAX_SIZE];
int Done=0;



// ***** User commands *****
const char* prompt = "]> ";
const char* pon_CMD = "pon"; 
const char* poff_CMD = "poff";
const char* status_CMD = "status";
const char* set_slot_CMD = "setslot";

const char* MUX4_CH0_CMD = "mux4ch0";
const char* MUX4_CH1_CMD = "mux4ch1";
const char* MUX4_CH2_CMD = "mux4ch2";
const char* MUX4_ALL_CMD = "mux4all";

const char* gpio_ext_off_CMD = "gextoff";


const char* setfan1_CMD = "setfan1";
const char* setfan2_CMD = "setfan2";
const char* pex_CMD = "pex";

const char* test_CMD = "test";
const char* vers_CMD = "vers";
const char* exit_CMD = "exit"; 
const char* help_CMD = "help";
const char* UnKwn_CMD = "command not found";



//***** CONSTANT VERBOSE *****

//THREAD
const char* V_THREAD1_EN = "=> THREAD#1 enabled: IHM\r\n";
const char* V_THREAD2_EN = "=> THREAD#2 enabled: POWER_RESET_REFCLK\r\n";
const char* V_THREAD3_EN = "=> THREAD#3 enabled: SYSTEM\r\n";
const char* V_THREAD4_EN = "=> THREAD#4 enabled: INT\r\n";

const char* V_THREAD1_DIS = "=> THREAD#1 disabled: IHM\r\n";
const char* V_THREAD2_DIS = "=> THREAD#2 disabled: POWER_RESET_REFCLK\r\n";
const char* V_THREAD3_DIS = "=> THREAD#3 disabled: INT\r\n";
const char* V_THREAD4_DIS = "=> THREAD#4 disabled: SYS\r\n";


//POWER CONTROL
const char* V_PWR_ON = "*** POWER ON ***\n\r";
const char* V_PWR_OFF = "*** POWER OFF ***\n\r";
const char* V_PWR_EPO = "*** EMERGENCY POWER OFF ***\n\r";
const char* V_PWR_OK = "*** POWER OK ***\n\r";

const char* V_P3V3_OK = "=> P3V3 OK\n\r";
const char* V_P0V9_OK = "=> P0V9 OK\n\r";
const char* V_P1V8_OK = "=> P1V8 OK\n\r";

const char* V_P3V3_ON = "=> P3V3 ON\n\r";
const char* V_P0V9_ON = "=> P0V9 ON\n\r";
const char* V_P1V8_ON = "=> P1V8 ON\n\r";

const char* V_P3V3_OFF = "=> P3V3 OFF\n\r";
const char* V_P0V9_OFF = "=> P0V9 OFF\n\r";
const char* V_P1V8_OFF = "=> P1V8 OFF\n\r";

const char* V_P3V3_PG_KO = "\n\r*** Error: P3V3_PG ***\n\r";
const char* V_P0V9_PG_KO = "\n\r*** Error: P0V9_PG ***\n\r";
const char* V_P1V8_PG_KO = "\n\r*** Error: P1V8_PG ***\n\r";

//REFCLOCK CONTROL
const char* V_REFCLK_ON = "=> REFCLOCK ON\n\r";
const char* V_REFCLK_OFF = "=> REFCLOCK OFF\n\r";

//SLOT TYPE CONFIG
const char* V_SLOT1 = "SLOT1: ";
const char* V_SLOT2 = "SLOT2: ";
const char* V_SLOT3 = "SLOT3: ";
const char* V_SLOT4 = "SLOT4: ";
const char* V_SLOT5 = "SLOT5: ";
const char* V_SLOT6 = "SLOT6: ";
const char* V_SLOT7 = "SLOT7: ";

const char* V_SLOT_TYPE_HOST = "HOST Port\n\r";
const char* V_SLOT_TYPE_DWN = "DOWNSTREAM Port\n\r";
const char* V_SLOT_NOCFG_FILE = "No such file Slot config\n\r";


const char* V_CARD_PRESENCE = "CARD PRESENCE\n\r";
const char* V_CARD_NOPRESENCE = "CARD NO PRESENCE\n\r";

const char* V_SLOT_TYPE_NO_CMPLT = "\n\r*** SLOT TYPE non compliant ***\n\r";
const char* V_SLOT_TYPE_CONFIG = "=> SLOT TYPE configured\n\r";
const char* V_SLOT_TYPE_NO_CONFIG = "\n\r*** SLOT TYPE are not configured, the Default configuration has been used ***\n\r";


//HOST/SLOT PERST
const char* V_HOST_PERST_DEASSERT ="Host PERST de-asserted \n\r";
const char* V_HOST_PERST_ASSERT ="Host PERST asserted \n\r";
const char* V_SLOTx_PERST_ASSERT ="SLOTx_PERST asserted \n\r";
const char* V_SLOTx_PERST_DEASSERT ="SLOTx_PERST de-asserted \n\r";

//SWICTH MODE
const char* V_SW_FB_LOCAL_MODE =  "SWITCH FABRIC MODE: LOCAL\n\r";
const char* V_SW_FB_REMOTE_MODE = "SWITCH FABRIC MODE: REMOTE\n\r";

//system PCIe Reset Mode
const char* V_SW_SYS_RST_MODE_AUTO =  "RESET MODE: AUTO\n\r";
const char* V_SW_SYS_RST_MODE_MANUAL =  "RESET MODE: MANUAL\n\r";

//ALERT/FAULT/ERROR
const char* V_I2C_TEMP_ALRT = "*** BOARD ALERT TEMPERATURE !!! ***\n\r";
const char* V_VR_HOT_ALRT = "*** VR HOT ALERT TEMPERATURE !!! ***\n\r";
const char* V_VR_I2C_ALRT = "*** VR I2C ALERT !!! ***\n\r";
const char* V_VR_FLT = "*** VR FAULT !!! ***\n\r";
const char* V_PEX_SYS_ERR = "*** PEX SYS ERROR !!! ***\n\r";
const char* V_P12V_PWR_FLT = "*** P12V POWER SENSOR FAULT !!! ***\n\r";








////////////////////////////////////////////////
//        T H R E A D   F U N C T I O N S
////////////////////////////////////////////////

// 	****  THREAD1 - IHM  ****
void * IHM_THR()
{
	while(!Done) {
		IHM_Console_MNGT();	//IHM Console Management started
 		delay_ms(20);
	}
    return LOW;
}


// 	****  THREAD2 - POWER PCIE RESET  ****
void * POWER_RESET_THR()
{
	while(!Done) {
   
		SYS_Power_CTRL();
		SYS_Refclk_CTRL();
		if (!SYS_PERST_MODE) SYS_Reset_CTRL();
 		delay_ms(20);
	}
    return LOW;
}


// 	****  THREAD3 - SYSTEM  ****
void * SYS_THR()
{
	while(!Done) {

		// **** FAN Control ****
		//Enable FAN Alert Interrupt when power is OK
		if(SYS_PWR_OK) EMC2302_SetIntEnable(fd_EMC2302_i2c, (F2ITEN | F1ITEN));
		else EMC2302_SetIntEnable(fd_EMC2302_i2c, FxITOFF); 

		//Reset FAN Alert when power is down
		if(!SYS_PWR_EN) EMC2302_ClearError(fd_EMC2302_i2c);
 		delay_ms(50);
	}
    return LOW;
}



// 	****  THREAD4 - INTERRUPT  ****
void * INT_THR()
{
	int ret;
	uint8_t reg_data;

/*
	chip_INT_I2CEXP = gpiod_chip_open_by_name(CHIP1);
	if (!chip_INT_I2CEXP) {
		perror("Open chip failed\n");
		ret = -1;
	}

	line_INT_I2CEXP = gpiod_chip_get_line(chip_INT_I2CEXP, IOEXP0_INT);
	if (!line_INT_I2CEXP) {
		perror("Get line failed\n");
		ret = -1;
	}

	ret = gpiod_line_request_falling_edge_events(line_INT_I2CEXP, CONSUMER);
	if (ret < 0) {
		perror("Request event notification failed\n");
		ret = -1;
	}

	//Check error
	if(ret < 0) Done=1;

	while(!Done) {

		gpiod_line_event_wait(line_INT_I2CEXP, NULL);
		if (gpiod_line_event_read(line_INT_I2CEXP, &event_INT_I2CEXP) == 0) {
			if (event_INT_I2CEXP.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
				//Ckeck Interrupt state
				if (!gpiod_line_get_value(line_INT_I2CEXP)) {

					// *** Interrupt is cleared ***
					//Reg_INTFA=MCP23017_ReadREGx(fd_MCP23017_EXP0_i2c, MCP23x17_INTFA);
					//Reg_INTCAPA=MCP23017_ReadREGx(fd_MCP23017_EXP0_i2c, MCP23x17_INTCAPA);		
					Reg_INTFB=MCP23017_ReadREGx(fd_MCP23017_EXP0_i2c, MCP23x17_INTFB);
					Reg_INTCAPB=MCP23017_ReadREGx(fd_MCP23017_EXP0_i2c, MCP23x17_INTCAPB);
		
					// *** DEBUG ***
					//printf("INTFA: 0x%X\n\r", Reg_INTFA);
					//printf("INTCAPA: 0x%X\n\r", Reg_INTCAPA);		
					//printf("INTFB: 0x%X\n\r", Reg_INTFB);
					//printf("INTCAPB: 0x%X\n\r", Reg_INTCAPB);
				
					// Check each bit from Register
					for (int i = 0; i < 8; i++) {
						if (Reg_INTFB & (1 << i)) { 			// Check if Reg_INTFB.bit(i)=1
							if( !(Reg_INTCAPB & (1 << i)) ) {	// Check if Reg_INTCAPB.bit(i)=0
							printf("\r");
							printf("Int on PB%d\n", i);
							}
						} 
					}
				}		
			}
		}
	*/	
	
	
	chip_FAN_ALERT = gpiod_chip_open_by_name(CHIP1);
	if (!chip_FAN_ALERT) {
		perror("FAN ALERT: Open chip failed\n");
		ret = -1;
	}
	delay_ms(1);

	line_FAN_ALERT = gpiod_chip_get_line(chip_FAN_ALERT, FAN_ALERT_N);
	if (!line_FAN_ALERT) {
		perror("FAN ALERT: Get line failed\n");
		ret = -1;
	}
	delay_ms(1);

	ret = gpiod_line_request_falling_edge_events(line_FAN_ALERT, CONSUMER);
	if (ret < 0) {
		perror("FAN ALERT: Request event notification failed\n");
		ret = -1;
	}
	delay_ms(1);

	//Check error
	if(ret < 0) Done=1;
	
	
	while(!Done) {

		gpiod_line_event_wait(line_FAN_ALERT, NULL);
		if (gpiod_line_event_read(line_FAN_ALERT, &event_FAN_ALERT) == 0) {
			if (event_FAN_ALERT.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
				//Ckeck Interrupt state
				if (!gpiod_line_get_value(line_FAN_ALERT)) {
	
					
					//Check FAN Status Register
					reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STATUS);
					printf("FANx STATUS:%X\n\r", reg_data);
		
					//CheckFAN Stall Status Register
					reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STALL_STATUS);
					if((reg_data & 0x01)==0x01) printf("\n\r ===> FAN 1 STALL ALERT <===\n\r");
					if((reg_data & 0x02)==0x02) printf("\n\r ===> FAN 2 STALL ALERT <===\n\r");

				}
			}
		}

		delay_ms(10);
	}

	return LOW;
}



///////////////////////////////
//         M A I N
///////////////////////////////

int main ()
{

	///////////////////////
	// I N I T    G P I O
	///////////////////////	
	//
	GPIO_EXT_WR_OFF();	//All GPIO EXT set as output Low level



	//////////////////////////////////////
	// I N I T    I 2 C    D E V I C E S
	//////////////////////////////////////
    
	// ----- IO EXPANDER #0 (MCP23017) -----
 	GPIO_WR(IOEXP0_RESET_N, CHIP1, HIGH);
	printf("IOEXP0 Reset released\r\n");

	if ((fd_MCP23017_EXP0_i2c = SLD_PiI2CSetupInterface (i2cDevice, MCP23017_EXP0_AD)) < 0) {
		printf ("Error: Unable to start I2C IO_EXPANDER#0: %s\n", strerror (errno));
		return HIGH;
	}
	else if ((SLD_PiI2CWrite(fd_MCP23017_EXP0_i2c, MCP23x17_IODIRA)) < 0) {	//Check Device Address
		printf ("Error: Unable to detect I2C IO_EXPANDER#0: %s\n", strerror (errno));
	}
	else {	
		
		//SETUP DEFAULT CONFIG
		MCP23017_Setup(fd_MCP23017_EXP0_i2c);
		
		//I/O DIRECTION CHA (IODIRA)
		//<PA7:PA0>, 1: input, 0: output
		SLD_PiI2CWriteRegByte(fd_MCP23017_EXP0_i2c, MCP23x17_IODIRA, 0b10000000);
	
		//I/O DIRECTION CHB (IODIRB)
		//<PB7:PB0> 1: input, 0: output
		SLD_PiI2CWriteRegByte(fd_MCP23017_EXP0_i2c, MCP23x17_IODIRB, 0b00000000);
	
		delay_ms(10);
		printf ("=> INIT I2C IO_EXPANDER#0 Done\r\n"); 
	}
  
 
  
  
  
	// ----- IO EXPANDER #1 (MCP23017) -----
	GPIO_WR(IOEXP1_RESET_N, CHIP1, HIGH);
	printf("IOEXP1 Reset released\r\n");

  	if ((fd_MCP23017_EXP1_i2c = SLD_PiI2CSetupInterface (i2cDevice, MCP23017_EXP1_AD)) < 0) {
		printf ("Error: Unable to start I2C IO_EXPANDER#1: %s\n", strerror (errno));
		return HIGH;
	}
	else if ( (SLD_PiI2CWrite(fd_MCP23017_EXP1_i2c, MCP23x17_IODIRA)) < 0 ) {	//Check Device Address
			printf ("Error: Unable to detect I2C IO_EXPANDER#1: %s\n", strerror (errno));
	}
	else {
 		
		//SETUP DEFAULT CONFIG
		MCP23017_Setup(fd_MCP23017_EXP1_i2c);
		
		//I/O DIRECTION CHA (IODIRA)
		//<PA7:PA0>, 1: input, 0: output
		SLD_PiI2CWriteRegByte(fd_MCP23017_EXP1_i2c, MCP23x17_IODIRA, 0b11111111);
	
		//I/O DIRECTION CHB (IODIRB)
		//<PB7:PB0> 1: input, 0: output
		SLD_PiI2CWriteRegByte(fd_MCP23017_EXP1_i2c, MCP23x17_IODIRB, 0b00011100);

		delay_ms(10);
		printf ("=> INIT I2C IO_EXPANDER#1 Done\r\n");  
	}
   
  
   

	// ----- M U X  4 (PCA9545A) -----
	GPIO_WR(MUX4_RESET_N, CHIP1, HIGH);
	printf("MUX4 Reset released\r\n");	
	
	if ((fd_PCA9545A_MUX4_i2c = SLD_PiI2CSetupInterface (i2cDevice, PCA9545A_AD)) < 0) {
		printf ("Error: Unable to start I2C MUX4: %s\n", strerror (errno));
		return HIGH;
	}
	else if ( (SLD_PiI2CWrite(fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN0)) < 0 ) {	//Check Device Address and select channel0
			printf ("Error: Unable to detect I2C MUX4: %s\n", strerror (errno));
	}
	else {
		delay_ms(10);
		printf ("=> INIT MUX4 Done\r\n"); 			
	}




	// ----- M U X  8 (PCA9548A) -----
	GPIO_WR(MUX8_RESET_N, CHIP1, HIGH);
	printf("MUX8 Reset released\r\n");

  	if ((fd_PCA9548A_MUX8_i2c = SLD_PiI2CSetupInterface (i2cDevice, PCA9548A_AD)) < 0) {
		printf ("Error: Unable to start I2C MUX8: %s\n", strerror (errno));
		return HIGH;
	}
	

	// ----- TEMP & HUMIDITY (SHT41) -----
 	if ((fd_SHT41_i2c = SLD_PiI2CSetupInterface (i2cDevice, SHT41_AD)) < 0) {
	//if ((fd_SHT41_i2c = SLD_PiI2CSetup (SHT41_AD)) < 0) {
		printf ("Error: Unable to start I2C Temp & Humidity sensor: %s\n", strerror (errno));
		return HIGH;
	}
	else if ( (SLD_PiI2CWrite(fd_SHT41_i2c, SHT4X_CMD_READ_SERIAL)) < 0) {	//Check Device Address	
		printf ("Error: Unable to detect I2C Temp & Humidity sensor: %s\n", strerror (errno));
	}
	else {
		delay_ms(10);
		printf ("=> INIT SHT41 Done\r\n"); 
	}
		
		
	// ----- FANs CONTROLLER (EMC2302) -----	
 	//Select MUX4 Channel 1
	PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
	if ((fd_EMC2302_i2c = SLD_PiI2CSetupInterface (i2cDevice, EMC2302_AD)) < 0) {
		printf ("Error: Unable to start I2C FAN Controller: %s\n", strerror (errno));
		return HIGH;
	}
	else if ( EMC2302_GetManufactID(fd_EMC2302_i2c) != 0x5D) 	//Check Device Address	
		printf ("Error: Unable to start I2C FAN Controller: %s\n", strerror (errno));
	else {
		
		//SETUP DEFAULT CONFIG
		EMC2302_Setup (fd_EMC2302_i2c);
		
		delay_ms(10);
		printf ("=> INIT EMC2302 Done\r\n"); 
	}		


	// ----- P W R   M O N I T O R I N G (ISL28022) -----
 	//Select MUX4 Channel 1
	PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);

	if ((fd_ISL28022_i2c = SLD_PiI2CSetupInterface (i2cDevice, ISL28022_AD)) < 0) {
		printf ("Error: Unable to start I2C POWER Monitoring: %s\n", strerror (errno));
		return HIGH;
	}
	else if( !Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_CFG, SIZE_WORD) ) 
		printf ("Error: Unable to start I2C POWER Monitoring: %s\n", strerror (errno));
	else {
	
		//SETUP DEFAULT CONFIG
		Init_ISL28022(fd_ISL28022_i2c);

		delay_ms(10);
		printf ("=> INIT ISL28022 Done\r\n"); 
	}

	
	

	//////////////////////////////////////
	// I N I T    S L O T S     C F G
	//////////////////////////////////////

    /*
     * Init Type:
	 * 		0x00: HOST
	 * 		0x01: DOWNSTREAM (End Point)
     * Refclock enable
     * PCIe Reset direction
     *
     */
    
	Get_SLOT_TYPE_from_file(File_slot_type, HIGH);	//Get Slot Type from config file - verbose
	Init_SLOT_CFG(HIGH);						//Init slot configuration - verbose
	Init_SW_CFG();								//Init Switch Configuration
	//Check_SYS_PCIe_RESET_MODE();				//System PERST Mode
	//Check_CARD_PRSNT_STATUS();


	
  
	///////////////////////////
	// I N I T    T H R E A D
	//////////////////////////

	//IHM Console
	pthread_create(&thr1, NULL, &IHM_THR, NULL); 
 	printf ("%s", V_THREAD1_EN);

	//Power and PCie Reset Control
	pthread_create(&thr2, NULL, &POWER_RESET_THR, NULL);
	printf ("%s", V_THREAD2_EN);

	//System (FAN Alert, Error ...)
	pthread_create(&thr3, NULL, &SYS_THR, NULL); 
 	printf ("%s", V_THREAD3_EN);

	//Interrupt (FAN Alert, Error ...)
	pthread_create(&thr4, NULL, &INT_THR, NULL); 
 	printf ("%s", V_THREAD4_EN); 



	////////////////////////////
	// L O O P    F O R E V E R
	////////////////////////////	
	while(!Done) delay_ms(10);
 

	////////////////////////////
	// 		  E X I T
	////////////////////////////
 
	//Thread Stopped
	pthread_cancel(thr1);
	printf ("%s", V_THREAD1_DIS);

	pthread_cancel(thr2);
	printf ("%s", V_THREAD2_DIS);

	pthread_cancel(thr3);
	printf ("%s", V_THREAD3_DIS);

	pthread_cancel(thr4);
	printf ("%s", V_THREAD4_DIS);


	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);
	pthread_join(thr3, NULL);
	pthread_join(thr4, NULL);

	//GPIO Stopped
	//gpiod_line_release(line_INT_I2CEXP);
	//gpiod_chip_close(chip_INT_I2CEXP);
 
	gpiod_line_release(line_FAN_ALERT);
	gpiod_chip_close(chip_FAN_ALERT); 

 
 	return LOW;
}




