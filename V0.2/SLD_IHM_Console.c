
/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_IHM_Console.c
*************************************************************************************************************/
//  Description  : IHM Console Management
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 16/12/2023: creation

*************************************************************************************************************/

#include "fabricboard.h"


/*___________________________________________________________________________*/
/*!
* fn void IHM_Console_MNGT(void)
* brief Main Console management
*
*/

void IHM_Console_MNGT(void) {

	uint32_t val, dirval;
	uint8_t reg_data, Nbr_char;
	char First_char;
	uint8_t I2C_BUFF_DATA[4];	
	
	int PWM_FAN;
	uint16_t RPM_FAN;
  
	uint16_t ISL_DATA;
	uint32_t Shtval, Uval, Ival, Pval;
	float VshuntVal, VinAvgVal, IinAvgVal, PinAvgVal;

	uint16_t PexRawTemp;
	float PexTemp;


	printf("%s", prompt);
	fflush (stdout);    //Get command from User
	scanf("%[^\n]", CMD_USER);
	fgetc( stdin );   /* to delete '\n' character */ 
	
	//How many char(s)
	Nbr_char=strlen(CMD_USER);
	//Get first char
	First_char=CMD_USER[0];

	/*-------------------------------------------------------------*/
	/*					CHARACTER ACQUISITION	  				   */
	/*-------------------------------------------------------------*/
	if(	(((First_char >= 'A') && (First_char <= 'Z')) ||
		((First_char >= 'a') && (First_char <= 'z')) ||
		((First_char >= '0') && (First_char <= '9'))) && Nbr_char==1)
	{
		//Debug
		//printf("\nFirst character is : %c, Number of char is: %d \n\r",First_char, Nbr_char);
	
		if(	First_char == 'A' ||
			First_char == 'Z' ||
			//First_char == 'E' ||
			//First_char == 'R' ||
			First_char == 'T' ||
			//First_char == 'Y' ||
			First_char == 'U' ||
			First_char == 'I' ||
			First_char == 'O' ||
			First_char == 'P' ||
			//First_char == 'Q' ||
			//First_char == 'S' ||
			First_char == 'D' ||
			First_char == 'F' ||
			First_char == 'G' ||
			//First_char == 'H' ||
			//First_char == 'J' ||
			//First_char == 'K' ||
			//First_char == 'L' ||
			First_char == 'M' ||			
			First_char == 'N' ||
			//First_char == 'W' ||
			First_char == 'X' ||
			First_char == 'C' ||
			First_char == 'V' ||
			First_char == 'B' 
			
			
			)
		{
			
			//Run Task
			switch(First_char)	{
	
				//
				case 'A':
					//P3V3 ON
					GPIO_I2CEXP_WR(EXP1, P3V3_EN, HIGH);
					printf("=> P3V3 ON\n\r");
					delay_ms(5);
					val=GPIO_I2CEXP_RD(EXP1, P3V3_PG);
					printf("=> P3V3_PG=%X\n\r", val);
					break;
	
				//
				case 'Z':
					//P3V3 OFF
					GPIO_I2CEXP_WR(EXP1, P3V3_EN, LOW);
					printf("=> P3V3 OFF\n\r");
					delay_ms(5);
					val=GPIO_I2CEXP_RD(EXP1, P3V3_PG);
					printf("=> P3V3_PG=%X\n\r", val);	
					break;
		


				//P0V9 and P1V8 enabled
				case 'O':
					//P0V9 ON
					GPIO_I2CEXP_WR(EXP1, VR_P0V9_EN, HIGH);
					printf("=> P0V9 ON\n\r");
					delay_ms(5);
					val=GPIO_I2CEXP_RD(EXP1, VR_P0V9_PG);
					printf("=> P0V9_PG=%X\n\r", val);

					//P1V8 ON
					GPIO_I2CEXP_WR(EXP1, VR_P1V8_EN, HIGH);
					printf("=> P1V8 ON\n\r");
					delay_ms(5);
					val=GPIO_I2CEXP_RD(EXP1, VR_P1V8_PG);
					printf("=> P1V8_PG=%X\n\r", val);					
					break;


				//P0V9 and P1V8 disabled
				case 'P':
					//P0V9 OFF
					GPIO_I2CEXP_WR(EXP1, VR_P0V9_EN, LOW);
					printf("=> P0V9 OFF\n\r");
					delay_ms(5);
					val=GPIO_I2CEXP_RD(EXP1, VR_P0V9_PG);
					printf("=> P0V9_PG=%X\n\r", val);

					//P1V8 OFF
					GPIO_I2CEXP_WR(EXP1, VR_P1V8_EN, LOW);
					printf("=> P1V8 OFF\n\r");
					delay_ms(5);
					val=GPIO_I2CEXP_RD(EXP1, VR_P1V8_PG);
					printf("=> P1V8_PG=%X\n\r", val);	
					break;



				//PCIe Refclock enabled
				case 'V':
					printf("*** REFCLOCK ON*** \n\r");
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE0, HIGH);		//PEX
					//GPIO_I2CEXP_WR(EXP0, REFCLK_OE1, HIGH);		//SLOT1
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE2, HIGH);		//SLOT2
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE3, HIGH);		//SLOT3
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE6, HIGH);		//SLOT4
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE5, HIGH);		//SLOT5
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE4, HIGH);		//SLOT6
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE7, HIGH);		//SLOT7
					
					break;


				//PCIe Refclock disabled
				case 'B':
					printf("*** REFCLOCK OFF*** \n\r");
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE0, LOW);		//PEX
					//GPIO_I2CEXP_WR(EXP0, REFCLK_OE1, LOW);		//SLOT1
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE2, LOW);		//SLOT2
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE3, LOW);		//SLOT3
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE6, LOW);		//SLOT4
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE5, LOW);		//SLOT5
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE4, LOW);		//SLOT6
					GPIO_I2CEXP_WR(EXP0, REFCLK_OE7, LOW);		//SLOT7					
				
					break;
				
				
				////PCIe Reset Asserted
				case 'X':
					printf("*** PEX RESET ASSERT *** \n\r");
					PERST_Assert();
					break;
				
				
				//PCIe Reset Desasserted/Asserted/Desasserted |0|1|0|
				//PCIe Reset Desasserted
				case 'C':
					
					printf("*** PEX RESET DESASSERT *** \n\r");
					PERST_Deassert();
					delay_ms(100);
					
					printf("*** PEX RESET ASSERT *** \n\r");
					PERST_Assert();
					delay_ms(100);
					
					printf("*** PEX RESET DESASSERT *** \n\r");
					PERST_Deassert();
					break;


				//SPI FLASH setting: PEX (default)
				case 'M':
					printf("SEL MUX_SPI: FLASH > SPI_PEX\r\n");	
					GPIO_WR(SEL_SPI_MUX, CHIP1, LOW);
					break;

				//SPI FLASH setting:  SPI_CM4
				case 'N':
					printf("SEL MUX_SPI: FLASH > SPI_CM4\r\n");	
					GPIO_WR(SEL_SPI_MUX, CHIP1, HIGH);
					break;


				//UART ARM PEX: HEADER (default)
				case 'U':
					printf("SEL UART ARM PEX: UART > HEADER\r\n");	
					GPIO_I2CEXP_WR(EXP1, SEL_SW_UART_CM4, LOW);
					break;


				//UART ARM PEX: UART_CM4
				case 'I':
					printf("SEL UART ARM PEX: UART > UART_CM4\r\n");	
					GPIO_I2CEXP_WR(EXP1, SEL_SW_UART_CM4, HIGH);
					break;



				//Temperature & Humidity from sensor
				case 'T':

					Echo_LFCR();
					for(uint8_t i=0; i<40; i++) printf("*");
					Echo_LFCR();


					///////////// PEX DIE TEMP SENSOR /////////////////
					//Select MUX4 Channel 2
					PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN2);

					//Init I2C PEX
					PEX_I2C_Access(0);

					//Get PEX Temperature
					PexRawTemp=PEX_get_RawTemp(fd_PEX_i2c);
					U16toArr(PexRawTemp, I2C_BUFF_DATA);	//Convert  Ulong (16-bit) to Array
					PexTemp=PEX_get_Temp(PexRawTemp);
					if(PexTemp < 150.0) printf("PEX DIE TEMPERATURE (Deg C): %0.1f\n\r", PexTemp);
					

					///////////// BOARD TEMP & RH SENSOR /////////////////					
					//Select MUX4 Channel 0
					PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN0);
		

					// Get serial number
					//val=SHT4x_GetSerialNumber(fd_SHT41_i2c);
					//printf("SN DATA:%X\n\r", val);
						
					// Get Temp & RH with High precision
					SHT4x_GetTemp_Humidity_HPM(fd_SHT41_i2c, &temperature, &humidity);
					printf("  BOARD TEMPERATURE (Deg C): %0.2f\n\r", temperature);
					printf("         BOARD HUMIDITY (%%): %0.2f\n\r", humidity);




					for(uint8_t i=0; i<40; i++)  printf("*");
					Echo_LFCR();
			
					break;	


				//Power Monitoring from sensor
				case 'D':

					Echo_LFCR();
					for(uint8_t i=0; i<40; i++) printf("*");
					Echo_LFCR();


					//Select MUX4 Channel 1
					PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);


					//Get ISL Configuration
					ISL_DATA=Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_CFG, SIZE_WORD);
					printf("ISL_CONFIG:%Xh\n\r", ISL_DATA);
	
					//Get Calibration Register
					ISL_DATA=Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_CAL, SIZE_WORD);
					printf("ISL_CAL_REG:%Xh\n\r", ISL_DATA);
						
					//Get Shunt Voltage Threshold
					ISL_DATA=Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_SHUNT_VLT_THRSH, SIZE_WORD);
					printf("ISL_SHUNT_VLT_THRSH:%Xh\n\r", ISL_DATA);
	
					//Get Average Shunt Voltage Register
					Shtval=GetAvgVal_ISL28022(fd_ISL28022_i2c, MOVAVG_SIZE, ISL_RG_SHUNT_VLT);
					printf("ISL_SHUNT_VOLT(mV):%Xh\n\r", Shtval);
	
					//Get Average ISL Bus Voltage Register
					Uval=GetAvgVal_ISL28022(fd_ISL28022_i2c, MOVAVG_SIZE, ISL_RG_BUS_VLT);
					printf("ISL_RG_BUS_VLT:%Xh\n\r", Uval);
						
					//Get Average ISL Current Register
					Ival=GetAvgVal_ISL28022(fd_ISL28022_i2c, MOVAVG_SIZE, ISL_RG_CURRENT);
					printf("ISL_RG_CURRENT:%Xh\n\r", Ival);
						
					//Get Average ISL Power Register
					Pval=GetAvgVal_ISL28022(fd_ISL28022_i2c, MOVAVG_SIZE, ISL_RG_POWER);
					printf("ISL_RG_POWER:%Xh\n\r", Pval);
						

					//Get Global U,I,P
					for(uint8_t i=0; i<40; i++)  printf("*");
					Echo_LFCR();
	
					if(SYS_PWR_EN) VshuntVal=Get_ISL_Shunt_Voltage(fd_ISL28022_i2c);
					else VshuntVal=0.0;
					printf("GBL_SHNT_VOLTAGE (mV): %.2f\n\r", VshuntVal);
						
					if(SYS_PWR_EN) VinAvgVal=Get_ISL_Voltage(fd_ISL28022_i2c);
					else VinAvgVal=0.0;
					printf("GBL_VOLTAGE (V):       %.2f\n\r", VinAvgVal);
				
					if(SYS_PWR_EN) IinAvgVal=Get_ISL_Current(fd_ISL28022_i2c);
					else IinAvgVal=0.0;
					printf("GBL_CURRENT (A):       %.2f\n\r", IinAvgVal);
	
					if(SYS_PWR_EN) PinAvgVal=Get_ISL_Power(fd_ISL28022_i2c);
					else PinAvgVal=0.0;
					printf("GBL_POWER (W):         %.2f\n\r", PinAvgVal);
						
					for(uint8_t i=0; i<40; i++)  printf("*");
					Echo_LFCR();


				break;


				//FAN Controller
				case 'F':

					//Select MUX4 Channel 1
					PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
					
					//*** FAN 1 ***
					RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN1, RPM_MULT_X2);	//RPM min: 1000 => m=2
					PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN1);
					printf("***** FAN 1 *****\n\r");                                                             
					printf("PWM:%d%%\n\r", PWM_FAN);
					printf("RPM:%d\n\r", RPM_FAN);
					printf("\n\r");
	
					//*** FAN 2 ***
					RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN2, RPM_MULT_X1);	//RPM min: 500 => m=1
					PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN2);
					printf("***** FAN 2 *****\n\r");
					printf("PWM:%d%%\n\r", PWM_FAN);
					printf("RPM:%d\n\r", RPM_FAN);
					printf("\n\r");
				
					printf("******* EMC2302 FAN Controler *******\n\r");
					reg_data=EMC2302_GetProductID(fd_EMC2302_i2c);
					printf("PRODUCT ID:%X\n\r", reg_data);

					reg_data=EMC2302_GetRevision(fd_EMC2302_i2c);
					printf("REV:%X\n\r", reg_data);
					printf("\n\r");

					
					reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STATUS);
					printf("FANx STATUS:%X\n\r", reg_data);

					reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_SPIN_STATUS);
					printf("FANx SPIN STATUS:%X\n\r", reg_data);

					reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STALL_STATUS);
					printf("FANx STALL STATUS:%X\n\r", reg_data);
					
					reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_DRIVE_FAIL_STATUS);
					printf("FANx DRIVE STATUS:%X\n\r", reg_data);		
			
					//reg_data=GPIO_RD(FAN_ALERT_N, CHIP1);
					//printf("FAN ALERT:%X\n\r", !reg_data);				
								
				break;



				//**** Get Board Configuration: ****
				//Get Slots Configuration (Host/Downstream/Fabric)
				//Get System Pcie Reset Mode (Auto/Manual)
				//Get Card Presence
				case 'G':
	
					Echo_LFCR();
					for(uint8_t i=0; i<40; i++) printf("*");
					Echo_LFCR();
					printf("     ***  BOARD CONFIGURATION  ***\n\r");
					for(uint8_t i=0; i<40; i++) printf("*");
					Echo_LFCR();
	
					Get_SLOT_CFG();
					//Get_SW_MODE();
					Check_SYS_PCIe_RESET_MODE();
					Check_CARD_PRSNT_STATUS();
	
				break;


	
				default: break;
			} //switch case	
	
		}
		else {
			//"Unknown command" sent to the terminal screen
			printf("%s", UnKwn_CMD); 
		
		}
	}
	else {
	

	
		// G P I O  E X T  O F F
		if(StringComp((char*)CMD_USER, gpio_ext_off_CMD)) {
			printf("*** ALL GPIO_Gx_EXTx OFF **** \n\r");
			GPIO_EXT_WR_OFF();

		}	
	
	
		// P O N
		else if(StringComp((char*)CMD_USER, pon_CMD)) {
			SYS_PWR_EN=0x01;
			SetBit(FSM_PWR_CTRL_EN, FSM_PWR_CTRL__PWR_EN_BIT);		//Update FSM Power Status Register
			delay_ms(500);
		
		}
		
		
		// P O F F
		else if(StringComp((char*)CMD_USER, poff_CMD)) {
			SYS_PWR_EN=0x00;
			ClrBit(FSM_PWR_CTRL_EN, FSM_PWR_CTRL__PWR_EN_BIT);		//Update FSM Power Status Register
			ClrBit(SYS_STATUS, SYS_STATUS__SYS_OK_BIT);				//Update System Status
			delay_ms(500);
		}

		
		/// S T A T U S
		else if(StringComp((char*)CMD_USER, status_CMD)) {
			printf("**** POWER STATUS **** \n\r");	
			printf("FSM PWR STATE:%X\n\r", ucPWR_STATE);
			printf("FSM PWR_STATUS:%X\n\r", FSM_PWR_STATUS);
			printf("PWR_STATUS:%X\n\r", PWR_STATUS);
			
			Echo_LFCR();
			printf("**** REFCLOCK STATUS **** \n\r");
			printf("FSM REFCLK STATE:%X\n\r", ucREFCLK_STATE);
			printf("REFCLOCK_ON:%X\n\r", REFCLK_ON);
		
			Echo_LFCR();
			printf("**** RESET STATUS **** \n\r");
			printf("FSM RST_STATE:%X\n\r", ucRST_STATE);
			printf("RST_RELEASE:%X\n\r", RST_RELEASE);			
		
			Echo_LFCR();
			printf("**** SYSTEM STATUS **** \n\r");
			printf("SYS_PWR_EN:%X\n\r", SYS_PWR_EN);
			printf("SYS_PWR_OK:%X\n\r", SYS_PWR_OK);
			printf("SYS_PERST:%X\n\r", SYS_PERST);
			printf("SYS_STATUS:%X\n\r", SYS_STATUS);
			
		}		
		
		
		// M U X 4  -  C H A N #0
		else if(StringComp((char*)CMD_USER, MUX4_CH0_CMD)) {		
			printf("*** MUX4 CHAN0 **** \n\r");
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN0);
		}

		
		// M U X 4  -  C H A N #1
		else if(StringComp((char*)CMD_USER, MUX4_CH1_CMD)) {		
			printf("*** MUX4 CHAN1 **** \n\r");
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
		}	

		// M U X 4  -  C H A N #2
		else if(StringComp((char*)CMD_USER, MUX4_CH2_CMD)) {		
			printf("*** MUX4 CHAN2 **** \n\r");
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN2);
		}
	
		// M U X 4   A L L  C H A N S
		else if(StringComp((char*)CMD_USER, MUX4_ALL_CMD)) {		
			printf("*** MUX4 ALL CHANNELS **** \n\r");
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_ALLCHAN);
		}		
		

		// F A N  1
		else if(StringComp((char*)CMD_USER, setfan1_CMD)) {
	
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);

			//*** FAN 1 ***
			RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN1, RPM_MULT_X2);	//RPM min: 1000 => m=2
			PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN1);
			printf("***** FAN 1 *****\n\r");                                                             
			printf("PWM:%d%%\n\r", PWM_FAN);
			printf("RPM:%d\n\r", RPM_FAN);
			Echo_LFCR();

	
			printf("Enter Speed Fan1 (%%):"); 
			scanf("%d",&PWM_FAN);
					 
		 	//set Fan speed 
			EMC2302_SetFanPWM(fd_EMC2302_i2c, FAN1, PWM_FAN); 
			
			reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN1_SETTING);
			printf("EMC2302 FAN1 SETTING:%X\n\r", reg_data);

			printf("FAN1 Speed is %d %%", PWM_FAN);
			
			fgetc( stdin );   /* to delete '\n' character */ 
			 
		} 
		 


		// F A N  2
		else if(StringComp((char*)CMD_USER, setfan2_CMD)) {	
			
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
			
			//*** FAN 2 ***
			RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN2, RPM_MULT_X1);	//RPM min: 500 => m=1
			PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN2);
			printf("***** FAN 2 *****\n\r");
			printf("PWM:%d%%\n\r", PWM_FAN);
			printf("RPM:%d\n\r", RPM_FAN);
			Echo_LFCR();			 
			
			printf("Enter Speed Fan2 (%%):"); 
			scanf("%d",&PWM_FAN);
		 
			//set Fan speed 
			EMC2302_SetFanPWM(fd_EMC2302_i2c, FAN2, PWM_FAN); 
		
			reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN2_SETTING);
			printf("EMC2302 FAN2 SETTING:%X\n\r", reg_data);

			printf("FAN2 Speed is %d %%", PWM_FAN);
			fgetc( stdin );   /* to delete '\n' character */ 

		}

  

		// S L O T 
		else if(StringComp((char*)CMD_USER, set_slot_CMD)) {
		
			Get_SLOT_TYPE_from_file(File_slot_type, HIGH);
			Init_SLOT_CFG(1);	//Init slot configuration - verbose
			//Get_SLOT_CFG();
		
			//printf("%d,%d,%d,%d,%d,%d,%d\n\r", Slot1_Cfg.Type,Slot2_Cfg.Type,Slot3_Cfg.Type,Slot4_Cfg.Type,Slot5_Cfg.Type,Slot6_Cfg.Type,Slot7_Cfg.Type);
			//for (uint8_t i=0; i < 7; i++) printf("SLOT_TYPE_CFG[%d]=%d\n\r", i, SLOT_TYPE_CFG[i]);
		}
		


		// P E X 
		else if(StringComp((char*)CMD_USER, pex_CMD)) {

			//Select MUX4 Channel 2
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN2);

			Echo_LFCR();
			for(uint8_t i=0; i<40; i++)  printf("*");
			Echo_LFCR();

			//Init I2C PEX
			PEX_I2C_Access(1);
			
			//Get PEX ARM FW Version from Register 0xA8 (Byte2, Byte3)
			PEX_get_ARM_FW_Version(fd_PEX_i2c, I2C_BUFF_DATA);
			printf(" PEX's ARM FIRMWARE VERSION: %d.%d.%d.%d\n\r" ,I2C_BUFF_DATA[0], I2C_BUFF_DATA[1], I2C_BUFF_DATA[2], I2C_BUFF_DATA[3]);
			
			//Get PEX FW Revision from Register 0xA8 (Byte0, Byte1)
			PEX_get_FW_Revision(fd_PEX_i2c, I2C_BUFF_DATA);
			printf("    PEX's FIRMWARE REVISION: %d.%d.%d.%d\n\r" ,I2C_BUFF_DATA[0], I2C_BUFF_DATA[1], I2C_BUFF_DATA[2], I2C_BUFF_DATA[3]);

			//Get PEX Temperature
			PexRawTemp=PEX_get_RawTemp(fd_PEX_i2c);
			U16toArr(PexRawTemp, I2C_BUFF_DATA);	//Convert  Ulong (16-bit) to Array
			PexTemp=PEX_get_Temp(PexRawTemp);
			printf("PEX DIE TEMPERATURE (Deg C): %0.1f\n\r", PexTemp);

			for(uint8_t i=0; i<40; i++)  printf("*");
			Echo_LFCR();			

		}


		// T E S T 
		else if(StringComp((char*)CMD_USER, test_CMD)) {
	
			//GPIO_I2CEXP_DirWR(EXP0, SLOT7_PERST_N, HIGH);	//1: input, 0: output
			//GPIO_I2CEXP_DirWR(EXP0, SLOT2_PERST_N, HIGH);	//1: input, 0: output
	
			printf("********** IO EXP #0 *********** \n\r");			
			printf("*** DIR: (0:output/1:input) **** \n\r");
			
			
			//MCP23017_CheckRegister(fd_MCP23017_EXP0_i2c);		//Debug Registers
			for (int i = 0 ; i < 16 ; ++i) {
				val=MCP23017_DigitalRead(fd_MCP23017_EXP0_i2c, i);
				dirval=MCP23017_DirRead(fd_MCP23017_EXP0_i2c, i);
				
				if(i < 8) {
					printf("PA%d:%d, DIR:%d\n\r", i, val, dirval); 
				}
				else {
					printf("PB%d:%d, DIR:%d\n\r", (i-8), val, dirval); 
				}	
			}
			
			printf("********** IO EXP #1 *********** \n\r");			
			printf("*** DIR: (0:output/1:input) **** \n\r");
			//MCP23017_CheckRegister(fd_MCP23017_EXP1_i2c);		//Debug Registers
			for (int i = 0 ; i < 16 ; ++i) {
				val=MCP23017_DigitalRead(fd_MCP23017_EXP1_i2c, i);
				dirval=MCP23017_DirRead(fd_MCP23017_EXP1_i2c, i);	

				if(i < 8) {
					printf("PA%d:%d, DIR:%d\n\r", i, val, dirval);
				}
				else {
					printf("PB%d:%d, DIR:%d\n\r", (i-8), val, dirval); 
				}	
			}
		
			//printf("*** ALL GPIO_Gx_EXTx ON **** \n\r");
			//GPIO_EXT_WR_ON();
			
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);

			val= Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_CFG, SIZE_WORD); 
			printf ("ISL28022 CFG REG: %X\n", val);

			val= Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_CAL, SIZE_WORD); 
			printf ("ISL28022 CAL REG: %X\n", val);

			

			printf("OK OK OK\n");

			
			
			
			
			
			
			
			
	
		}

	

		// V E R S
		else if(StringComp((char*)CMD_USER, vers_CMD)) {
			printf("Firmawre version: %d.%d\n\r", VERSION_MAJOR, VERSION_MINOR);
		}  
		
		// H E L P
		else if(StringComp((char*)CMD_USER, help_CMD)) {
			Help_DSPL();
		}
		
		// E X I T
		else if(StringComp((char*)CMD_USER, exit_CMD)) {
			Done=1;
		}  
		else {
			if(Nbr_char > 0) {
			//"Unknown command" sent to the terminal screen
			printf("%s", UnKwn_CMD); 
			}
		}
	}
	//Check any command requested
	if(Nbr_char > 0) printf("\n\r"); 
	memset((void*)CMD_USER, 0, CMD_MAX_SIZE);	//Clear CMD_USER Register

}






/*___________________________________________________________________________*/
/*!
*\fn void Help_DSPL(void)
*\brief Firmware information and Commands List
*/
void Help_DSPL(void) {

uint8_t i=0x00;

// Init Console

	printf("\n\r");
	for(i=0; i<80; i++)  printf("*");
    printf("\n\r");
	printf("                           * * *  H E L P  * * *                 \n\r");
	printf("                               COMMANDS LIST                     \n\r");
//**********************************************************************

	printf("      'A': P3V3 ON\r\n");
    printf("      'Z': P3V3 OFF\r\n");
    printf("      'O': P0V9 & P1V8 ON\r\n");
    printf("      'P': P0V9 & P1V8 OFF\r\n");
    printf("      'V': Refclock enabled\r\n");
    printf("      'B': Refclock disabled\r\n");
    printf("      'X': PCIE Reset Asserted\r\n");
    printf("      'C': PCIE Reset Cycle DEASSERT|ASSERT|DESASSERT\r\n");
    printf("      'M': MUX SPI: PEX Flash > SPI_PEX\r\n");
    printf("      'N': MUX SPI: PEX Flash > SPI_CM4\r\n");
    printf("      'U': Select UART ARM PEX > HEADER\r\n");
    printf("      'I': Select UART ARM PEX > UART_CM4\r\n");
    printf("      'F': FANs Status\r\n");
    printf("      'T': Get Temperature (Board, PEX) and Relative Humidity\r\n");
    printf("      'G': Get Board Configuration: Slot Type, Presence\r\n");
	printf("      'D': Get Global Power Monitoring from sensor\n\r");

	printf("    'pon': System Power ON\r\n");
	printf("   'poff': System Power OFF\r\n");
	printf(" 'status': Get all status: Power, Reset, Refclock, System\r\n");
	printf("'mux4ch0': I2C MUX4 Select Channel 0\r\n");
	printf("'mux4ch1': I2C MUX4 Select Channel 1\r\n");
	printf("'mux4ch2': I2C MUX4 Select Channel 2\r\n");
	printf("'mux4all': I2C MUX4 Select All Channels\r\n");
	printf("'setfan1': Fan1 speed setting\r\n");
	printf("'setfan2': Fan2 speed setting\r\n");
	printf("'    pex': Get PEX's information\r\n");
	printf("   'test': Use for Test onky\r\n");
	printf("'gextoff': Turn OFF all GPIO_Gx_EXTx OFF\r\n");
	printf("   'vers': Get Current Firmware version\r\n");
	printf("   'exit': Quit the program\r\n");
	printf("   'help': Display this commands list\r\n");

}