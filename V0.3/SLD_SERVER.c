
/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_SERVER.c
*************************************************************************************************************/
//  Description  : Server Management
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 16/12/2023: creation

*************************************************************************************************************/

#include "fabricboard.h"


/*___________________________________________________________________________*/
/*!
* fn void Server_MNGT(void)
* brief Main Console management
*
*/

void Server_MNGT(char *message) {

	uint32_t val, dirval;
	uint8_t Nbr_char; //,reg_data;
	char First_char;
	uint8_t I2C_BUFF_DATA[4];	
	
	int PWM_FAN;
	uint16_t RPM_FAN;
 	uint8_t status_fan, stall_fan, spin_fan, drive_fan;
   
	//uint16_t ISL_DATA;
	//uint32_t Shtval, Uval, Ival, Pval;
	float VshuntVal, VinAvgVal, IinAvgVal, PinAvgVal;

	uint16_t PexRawTemp;
	float PexTemp;
 
	char* arg1;
	char* arg2;

	int arg2_val;


	const char* v_separator 	   	= ",";
 	const char* v_stars_line     	= "*****************************************\n\r";
	const char* v_temp_rh_status 	= "\n\r****** T E M P  &  H U M I D I T Y ******\n\r";
	const char* v_fan1_status    	= "\n\r******** F A N 1 ********\n\r";
	const char* v_fan2_status    	= "\n\r******** F A N 2 ********\n\r";
	const char* v_power_status   	= "\n\r******* P O W E R *******\n\r";
	const char* v_refclck_status 	= "\n\r**** R E F C L O C K ****\n\r";
	const char* v_reset_status		= "\n\r******* R E S E T *******\n\r";
	const char* v_system_status		= "\n\r****** S Y S T E M ******\n\r";
	const char* v_slot_config		= "\n\r**** S L O T S  C O N F I G ****\n\r";


 	const char* v_PWM_status 	    = "PWM(%%):             %d\n\r";
	const char* v_RPM_status 	    = "RPM:                %d\n\r";



	//Declaration & Memory allocations for string messages
	char *str_data1 = (char*) calloc(BUFF_M_SIZE, sizeof(char));
	char *str_data2 = (char*) calloc(BUFF_M_SIZE, sizeof(char));
	char *str_data3 = (char*) calloc(BUFF_M_SIZE, sizeof(char));
	char *str_data4 = (char*) calloc(BUFF_M_SIZE, sizeof(char));
	char *returnvalue = (char*) calloc(BUFF_H_SIZE, sizeof(char));


 	
	//Check arguments Message 
	if( strchr(message, ',') == NULL ) {	//one argument
		strcpy(CMD_USER, message);
		arg2_val=0;
	}
	else {	//two arguments
		arg1 = strtok( message, v_separator );
		arg2 = strtok( NULL, v_separator );			
		
		//Get command User form arg1's message	
		strcpy(CMD_USER, arg1);
			
		//Get Value from arg2's message
		arg2_val=atoi(arg2);

	}		
	
	//printf("Command receive:%s, Value:%d\n", CMD_USER, arg2_val);
 
 
	//How many char(s)
	Nbr_char=strlen(CMD_USER);
	//Get first char
	First_char=CMD_USER[0];



 
	//-------------------------------------------------------------
	//					CHARACTER ACQUISITION	  				   
	//-------------------------------------------------------------
	if(	(((First_char >= 'A') && (First_char <= 'Z')) ||
		((First_char >= 'a') && (First_char <= 'z')) ||
		((First_char >= '0') && (First_char <= '9'))) && Nbr_char==1)
	{
		//Debug
		//printf("\nFirst character is : %c, Number of char is: %d \n\r",First_char, Nbr_char);
	
		if(	//First_char == 'A' ||
			//First_char == 'Z' ||
			//First_char == 'E' ||
			//First_char == 'R' ||
			//First_char == 'T' ||
			//First_char == 'Y' ||
			First_char == 'U' ||
			First_char == 'I' ||
			//First_char == 'O' ||
			//First_char == 'P' ||
			//First_char == 'Q' ||
			//First_char == 'S' ||
			//First_char == 'D' ||
			//First_char == 'F' ||
			//First_char == 'G' ||
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
	
				case 'A':
					//P3V3 ON
					GPIO_I2CEXP_WR(EXP1, P3V3_EN, HIGH);
					printf("=> P3V3 ON\n\r");
					delay_ms(5);
					val=GPIO_I2CEXP_RD(EXP1, P3V3_PG);
					printf("=> P3V3_PG=%X\n\r", val);
					break;
	
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
	
	
	
				case 'T':

				break;	
	
				
				case 'D':
		
				break;
	
	
				case 'F':

				break;
	

				case 'G':
	
				break;
	
	
	
				default: break;
			} //switch case	
	
		}
		else {
			//"Unknown command" sent to the terminal screen
			printf("%s", V_UnKwn_cmd); 
		}
	}
	else {
	
	
	
		// G P I O  E X T  A L L  O F F
		if(StringComp((char*)CMD_USER, gpio_ext_all_off_CMD)) {
			sprintf(str_data1, "*** ALL GPIO_Gx_EXTx OFF **** \n\r");
			GPIO_EXT_WR_OFF();

			//Return Data to Socket Client
			strcat(returnvalue, str_data1);	
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
		}	
	
	
		// P O W E R   O N
		else if(StringComp((char*)CMD_USER, pon_CMD)) {
			SYS_PWR_EN=0x01;
			SetBit(FSM_PWR_CTRL_EN, FSM_PWR_CTRL__PWR_EN_BIT);		//Update FSM Power Status Register
			
			//Return Data to Socket Client
			strcat(returnvalue, V_PWR_ON);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			delay_ms(200);
		}
		
		
		// P O W E R   O F F
		else if(StringComp((char*)CMD_USER, poff_CMD)) {
			SYS_PWR_EN=0x00;
			ClrBit(FSM_PWR_CTRL_EN, FSM_PWR_CTRL__PWR_EN_BIT);		//Update FSM Power Status Register
			ClrBit(SYS_STATUS, SYS_STATUS__SYS_OK_BIT);				//Update System Status
			
			//Return Data to Socket Client
			strcat(returnvalue, V_PWR_OFF);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);			
			delay_ms(200);
		}
	
		
		// P O W E R   S T A T U S
		else if(StringComp((char*)CMD_USER, pstatus_CMD)) {		
		
			if(SYS_PWR_EN && SYS_PWR_OK) strcat(returnvalue, V_PWR_ON);
			else if (!SYS_PWR_EN && !SYS_PWR_OK) strcat(returnvalue, V_PWR_OFF);
		
			//Return Data to Socket Client
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);	
		}

		
		/// S Y S   S T A T U S
		else if(StringComp((char*)CMD_USER, sys_status_CMD)) {
			printf("%s", v_power_status);
			printf("FSM PWR STATE:    %X\n\r", ucPWR_STATE);
			printf("FSM PWR_STATUS:   %X\n\r", FSM_PWR_STATUS);
			printf("PWR_STATUS:       %X\n\r", PWR_STATUS);
			
			Echo_LFCR();
			printf("%s", v_refclck_status);
			printf("FSM REFCLK STATE: %X\n\r", ucREFCLK_STATE);
			printf("REFCLOCK_ON:      %X\n\r", REFCLK_ON);
		
			Echo_LFCR();
			printf("%s", v_reset_status);
			printf("FSM RST_STATE:    %X\n\r", ucRST_STATE);
			printf("RST_RELEASE:      %X\n\r", RST_RELEASE);			
		
			Echo_LFCR();
			printf("%s", v_system_status);
			printf("SYS_PWR_EN:       %X\n\r", SYS_PWR_EN);
			printf("SYS_PWR_OK:       %X\n\r", SYS_PWR_OK);
			printf("SYS_PERST:        %X\n\r", SYS_PERST);
			printf("SYS_STATUS:       %X\n\r", SYS_STATUS);
			
		}		
		

		// G E T   S L O T   C F G 
		//Get Slots Configuration (Host/Downstream/Fabric)
		//Get System Pcie Reset Mode (Auto/Manual)
		//Get Card Presence
	
		else if(StringComp((char*)CMD_USER, get_slot_cfg_CMD)) {

			//Get Slots configuration
			Get_SLOT_CFG(str_data1);

			Check_SYS_PCIe_RESET_MODE();
			Check_CARD_PRSNT_STATUS(str_data2);

			//Return Data to Socket Client
			strcat(returnvalue, v_slot_config);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);			
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			//printf("%s",returnvalue);			

		}


		// S E T   S L O T   C F G 
		else if(StringComp((char*)CMD_USER, set_slot_cfg_CMD)) {

			Init_SLOT_CFG(str_data1);	//Init slot configuration
			Get_SLOT_TYPE_from_file(File_slot_type, str_data2);
			
			//Return Data to Socket Client
			strcat(returnvalue, v_slot_config);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			//printf("%s",returnvalue);	
			
		
			//printf("%d,%d,%d,%d,%d,%d,%d\n\r", Slot1_Cfg.Type,Slot2_Cfg.Type,Slot3_Cfg.Type,Slot4_Cfg.Type,Slot5_Cfg.Type,Slot6_Cfg.Type,Slot7_Cfg.Type);
			//for (uint8_t i=0; i < 7; i++) printf("SLOT_TYPE_CFG[%d]=%d\n\r", i, SLOT_TYPE_CFG[i]);

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
		
	
		// G E T   F A N  1
		else if(StringComp((char*)CMD_USER, get_fan1_CMD)) {	
	
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
			
			// *** Get FAN 1 ***
			RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN1, RPM_MULT_X2);	//RPM min: 1000 => m=2
			PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN1);
			
			//Check Status FAN1
			status_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STATUS);
			
			//Check SPIN FAN1
			spin_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_SPIN_STATUS);
			spin_fan &= 0x01;	
			
			//Check STALL FAN1
			stall_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STALL_STATUS);
			stall_fan &= 0x01;
			if (stall_fan && (status_fan&0x01)) {
				RPM_FAN=0;	//RPM null if Stall is enabled
				PWM_FAN=0;	//PWM null if Stall is enabled
			}
			
			//Check DRIVE FAN1
			drive_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_DRIVE_FAIL_STATUS);
			drive_fan &= 0x01;	

			//Get ProductID
			//reg_data=EMC2302_GetProductID(fd_EMC2302_i2c);

			//Get REV
			//reg_data=EMC2302_GetRevision(fd_EMC2302_i2c);


			//Return Data to Socket Client
			sprintf(str_data1, "PWM(%%):             %d\n\r", PWM_FAN);
			sprintf(str_data2, "RPM:                %d\n\r", RPM_FAN);
			strcat(returnvalue, v_fan1_status);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);
			sprintf(str_data3, "FAN STATUS:         %X\n\r", status_fan);
			strcat(returnvalue, str_data3);
			sprintf(str_data3, "FAN1 SPIN STATUS:   %X\n\r", spin_fan);
			strcat(returnvalue, str_data3);
			sprintf(str_data3, "FAN1 STALL STATUS:  %X\n\r", stall_fan);
			strcat(returnvalue, str_data3);
		    sprintf(str_data3, "FAN1 DRIVE STATUS:  %X\n\r", drive_fan);
			strcat(returnvalue, str_data3);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			//printf("%s",returnvalue);
	
		}


	
		// G E T   F A N  2
		else if(StringComp((char*)CMD_USER, get_fan2_CMD)) {	
	
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
	
			//Get FAN 2 status
			RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN2, RPM_MULT_X1);	//RPM min: 500 => m=1
			PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN2);
			
			//Check Status FAN2
			status_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STATUS);
			
			//Check SPIN FAN2
			spin_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_SPIN_STATUS);
			spin_fan &= 0x02;	
			
			//Check STALL FAN2
			stall_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN_STALL_STATUS);
			stall_fan &= 0x02;
			if (stall_fan && (status_fan&0x01)) {
				RPM_FAN=0;	//RPM null if Stall is enabled
				PWM_FAN=0;	//PWM null if Stall is enabled
			}
			
			//Check DRIVE FAN1
			drive_fan=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_DRIVE_FAIL_STATUS);
			drive_fan &= 0x02;	

			//Get ProductID
			//reg_data=EMC2302_GetProductID(fd_EMC2302_i2c);

			//Get REV
			//reg_data=EMC2302_GetRevision(fd_EMC2302_i2c);

			//Return Data to Socket Client
			sprintf(str_data1, v_PWM_status, PWM_FAN);
			sprintf(str_data2, v_RPM_status, RPM_FAN);
			strcat(returnvalue, v_fan2_status);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);
			sprintf(str_data3, "FAN STATUS:         %X\n\r", status_fan);
			strcat(returnvalue, str_data3);
			sprintf(str_data3, "FAN2 SPIN STATUS:   %X\n\r", spin_fan);
			strcat(returnvalue, str_data3);
			sprintf(str_data3, "FAN2 STALL STATUS:  %X\n\r", stall_fan);
			strcat(returnvalue, str_data3);
		    sprintf(str_data3, "FAN2 DRIVE STATUS:  %X\n\r", drive_fan);
			strcat(returnvalue, str_data3);
			//strcat(returnvalue, v_stars_line);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			//printf("%s",returnvalue);
			
		}	
	

	
		// S E T   F A N  1
		else if(StringComp((char*)CMD_USER, set_fan1_CMD)) {
	
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
	
			//Get PWM value fomr User
			PWM_FAN=arg2_val;		
		
			//set Fan speed 
			EMC2302_SetFanPWM(fd_EMC2302_i2c, FAN1, PWM_FAN); 
	
			//Get FAN 1 status
			RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN1, RPM_MULT_X2);	//RPM min: 1000 => m=2
			PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN1);

			//reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN1_SETTING);
			//printf("EMC2302 FAN1 SETTING:%X\n\r", reg_data);
			
			//Return Data to Socket Client
			sprintf(str_data1, v_PWM_status, PWM_FAN);
			sprintf(str_data2, v_RPM_status, RPM_FAN);			
			strcat(returnvalue, v_fan1_status);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);			
			//strcat(returnvalue, v_stars_line);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			//printf("%s",returnvalue);			
	
		} 
		
	
	
		// S E T   F A N  2
		else if(StringComp((char*)CMD_USER, set_fan2_CMD)) {	
			
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
			
			//Get PWM value fomr User
			PWM_FAN=arg2_val;			
			
			//set Fan speed 
			EMC2302_SetFanPWM(fd_EMC2302_i2c, FAN2, PWM_FAN); 			
			
			//Get FAN 2 status
			RPM_FAN=EMC2302_GetFanRPM(fd_EMC2302_i2c, FAN2, RPM_MULT_X1);	//RPM min: 500 => m=1
			PWM_FAN=EMC2302_GetFanPWM(fd_EMC2302_i2c, FAN2);

			//reg_data=SLD_PiI2CReadRegByte (fd_EMC2302_i2c, EMC2302_FAN2_SETTING);
			//printf("EMC2302 FAN2 SETTING:%X\n\r", reg_data);

			//Return Data to Socket Client
			sprintf(str_data1, v_PWM_status, PWM_FAN);
			sprintf(str_data2, v_RPM_status, RPM_FAN);			
			strcat(returnvalue, v_fan2_status);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);			
			//strcat(returnvalue, v_stars_line);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			//printf("%s",returnvalue);	

		}
	
	


		// P O W E R   M O N I T O R I N G
		else if(StringComp((char*)CMD_USER, get_power_CMD)) {
			

			/*
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
			*/	
	
			//Get Global U,I,P
			if(SYS_PWR_EN) VshuntVal=Get_ISL_Shunt_Voltage(fd_ISL28022_i2c);
			else VshuntVal=0.0;
			sprintf(str_data1, "GBL_SHNT_VOLTAGE (mV): %.2f\n\r", VshuntVal);
				
			if(SYS_PWR_EN) VinAvgVal=Get_ISL_Voltage(fd_ISL28022_i2c);
			else VinAvgVal=0.0;
			sprintf(str_data2, "GBL_VOLTAGE (V):       %.2f\n\r", VinAvgVal);
				
			if(SYS_PWR_EN) IinAvgVal=Get_ISL_Current(fd_ISL28022_i2c);
			else IinAvgVal=0.0;
			sprintf(str_data3, "GBL_CURRENT (A):       %.2f\n\r", IinAvgVal);
	
			if(SYS_PWR_EN) PinAvgVal=Get_ISL_Power(fd_ISL28022_i2c);
			else PinAvgVal=0.0;
			sprintf(str_data4, "GBL_POWER (W):         %.2f\n\r", PinAvgVal);


			//Return Data to Socket Client	
			strcat(returnvalue, v_stars_line);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);			
			strcat(returnvalue, str_data3);	
			strcat(returnvalue, str_data4);	
			strcat(returnvalue, v_stars_line);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			//printf("%s",returnvalue);	

		}



	
		// P E X 
		else if(StringComp((char*)CMD_USER, get_pex_CMD)) {
	
			//Select MUX4 Channel 2
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN2);
	
			//Init I2C PEX
			if (PEX_I2C_Access(1) != PEX_ERROR) {
			
				//Get PEX ARM FW Version from Register 0xA8 (Byte2, Byte3)
				PEX_get_ARM_FW_Version(fd_PEX_i2c, I2C_BUFF_DATA);
				//printf(" PEX's ARM FIRMWARE VERSION: %d.%d.%d.%d\n\r" ,I2C_BUFF_DATA[0], I2C_BUFF_DATA[1], I2C_BUFF_DATA[2], I2C_BUFF_DATA[3]);
				sprintf(str_data1, " PEX's ARM FIRMWARE VERSION: %d.%d.%d.%d\n\r" ,I2C_BUFF_DATA[0], I2C_BUFF_DATA[1], I2C_BUFF_DATA[2], I2C_BUFF_DATA[3]);
				
				//Get PEX FW Revision from Register 0xA8 (Byte0, Byte1)
				PEX_get_FW_Revision(fd_PEX_i2c, I2C_BUFF_DATA);
				//printf("    PEX's FIRMWARE REVISION: %d.%d.%d.%d\n\r" ,I2C_BUFF_DATA[0], I2C_BUFF_DATA[1], I2C_BUFF_DATA[2], I2C_BUFF_DATA[3]);
				sprintf(str_data2, "    PEX's FIRMWARE REVISION: %d.%d.%d.%d\n\r" ,I2C_BUFF_DATA[0], I2C_BUFF_DATA[1], I2C_BUFF_DATA[2], I2C_BUFF_DATA[3]);
		
				//Get PEX Temperature
				PexRawTemp=PEX_get_RawTemp(fd_PEX_i2c);
				U16toArr(PexRawTemp, I2C_BUFF_DATA);	//Convert  Ulong (16-bit) to Array
				PexTemp=PEX_get_Temp(PexRawTemp);
			}
			else PexTemp=0.0;

			//Return Data to Socket Client		
			sprintf(str_data3, " PEX DIE TEMPERATURE (Deg C): %0.1f\n\r", PexTemp);
			strcat(returnvalue, v_stars_line);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);
			strcat(returnvalue, str_data3);
			strcat(returnvalue, v_stars_line);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);			
			//printf("%s",returnvalue);

		}
	
	
		// T E M P  &  H U M I D I T Y 
		else if(StringComp((char*)CMD_USER, get_temp_rh_CMD)) {	

			///////////// PEX DIE TEMP SENSOR /////////////////
			//Select MUX4 Channel 2
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN2);
		
			//Init I2C PEX
			if (PEX_I2C_Access(0) != PEX_ERROR) {
				//Get PEX Temperature
				PexRawTemp=PEX_get_RawTemp(fd_PEX_i2c);
				U16toArr(PexRawTemp, I2C_BUFF_DATA);	//Convert  Ulong (16-bit) to Array
				PexTemp=PEX_get_Temp(PexRawTemp);
			}
			else PexTemp=0.0;
		
			///////////// BOARD TEMP & RH SENSOR /////////////////					
			//Select MUX4 Channel 0
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN0);
			
			// Get serial number
			//val=SHT4x_GetSerialNumber(fd_SHT41_i2c);
			//printf("SN DATA:%X\n\r", val);
				
			// Get Temp & RH with High precision
			SHT4x_GetTemp_Humidity_HPM(fd_SHT41_i2c, &temperature, &humidity);


			//Return Data to Socket Client		
			sprintf(str_data1, "PEX DIE TEMPERATURE (Deg C): %0.1f\n\r", PexTemp);
			sprintf(str_data2, "  BOARD TEMPERATURE (Deg C): %0.2f\n\r",temperature);
			sprintf(str_data3, "         BOARD HUMIDITY (%%): %0.2f\n\r", humidity);
			strcat(returnvalue, v_temp_rh_status);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, str_data2);
			strcat(returnvalue, str_data3);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);			
			//printf("%s",returnvalue);

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
			
			/*
			//Select MUX4 Channel 1
			PCA9545A_SetChannel (fd_PCA9545A_MUX4_i2c, PCA9545A_CHAN1);
	
			val= Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_CFG, SIZE_WORD); 
			printf ("ISL28022 CFG REG: %X\n", val);
	
			val= Get_Reg_ISL28022(fd_ISL28022_i2c, ISL_RG_CAL, SIZE_WORD); 
			printf ("ISL28022 CAL REG: %X\n", val);
			*/
	
	
	
	
	
	
		}
	
	
	
		// V E R S
		else if(StringComp((char*)CMD_USER, get_fw_vers_CMD)) {
			sprintf(str_data1,"Firmware version: %d.%d\n\r", VERSION_MAJOR, VERSION_MINOR);
		
			//Return Data to Socket Client	
			strcat(returnvalue, v_stars_line);
			strcat(returnvalue, str_data1);
			strcat(returnvalue, v_stars_line);
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);			
			//printf("%s",returnvalue);
		
		
		}  
		
		// H E L P
		else if(StringComp((char*)CMD_USER, help_CMD)) {
			Help_DSPL();
		}
		
		// E X I T
		else if(StringComp((char*)CMD_USER, exit_CMD)) {
			EXIT=1;
		}  
		else {

			if(Nbr_char > 0) {
			//"Unknown command" sent to the client
			strcat(returnvalue, V_UnKwn_cmd);			
			send(client_fd, returnvalue, BUFF_H_SIZE, 0);
			
			}
		}
	}
	//Check any command requested
	//if(Nbr_char > 0) printf("\n\r"); 
	memset((void*)CMD_USER, 0, BUFF_L_SIZE);	//Clear CMD_USER Register 
 
	//Release Memory
	free(str_data1);
	free(str_data2);
	free(str_data3);
	free(str_data4);
	free(returnvalue);
 
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

	
	/*
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
    printf("      'G': Get Board Configuration: Slot Type, Presence\r\n");
	printf("      'D': Get Global Power Monitoring from sensor\n\r");
*/
	printf(" Use =>  ./client_fabricboard [OPTION]\r\n\r\n");
	printf("      [OPTION]\r\n");
	printf("      power_on: Set System Power ON\r\n");
	printf("     power_off: Set System Power OFF\r\n");
	printf("  power_status: Get System Power Status\r\n");
	
	printf("    sys_status: Get all status: Power, Reset, Refclock, System\r\n");
	printf("  get_slot_cfg: Get Slot configuration: Slot Type, Presence\r\n");
	printf("  set_slot_cfg: Set Slot configuration from set_slot.cfg file\r\n");		
			 
	printf("      get_fan1: Get FAN1 Status\r\n");
	printf("      get_fan1: Get FAN2 Status\r\n");	
	printf("      set_fan1: Fan1 speed setting\r\n");
	printf("      set_fan2: Fan2 speed setting\r\n");
	printf("     get_power: Get Global Power Monitoring from sensor\n\r");
	printf("       get_pex: Get PEX's information\r\n");
	printf("   get_temp_rh: Get Temperature (Board, PEX) and Relative Humidity\r\n");
	printf("      mux4_ch0: I2C MUX4 Select Channel 0\r\n");
	printf("      mux4_ch1: I2C MUX4 Select Channel 1\r\n");
	printf("      mux4_ch2: I2C MUX4 Select Channel 2\r\n");
	printf("      mux4_all: I2C MUX4 Select All Channels\r\n");	
	
	printf("       p3v3_on: Set P3V3 ON\r\n");
	printf("      p3v3_off: Set P3V3 Off\r\n");
	printf("       p1v8_on: Set P1V8 ON\r\n");
	printf("      p1v8_off: Set P1V8 Off\r\n");	
	printf("       p0v9_on: Set P0V9 ON\r\n");
	printf("      p0v9_off: Set P0V9 Off\r\n");		
 
	printf("  gpio_ext_off: Turn OFF all GPIO_Gx_EXTx OFF\r\n");	
	printf("          test: Use for Test only\r\n");
	printf("   get_fw_vers: Get Current Firmware version\r\n");	
	printf("          exit: Quit the program\r\n");
	printf("          help: Display this commands list\r\n");

}