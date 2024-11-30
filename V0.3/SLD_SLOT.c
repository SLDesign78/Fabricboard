/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_SLOT.c
*************************************************************************************************************/
//  Description  : Slots configuration
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 10/05/2024: creation

*************************************************************************************************************/

#include "fabricboard.h"



/*______________________________________________________________________________________*/
/*!
 *\fn Get_SLOT_TYPE_from_file()
 * \brief Get Slot Type Configuration from config file (file_name)
 *
 */
/*______________________________________________________________________________________*/

uint8_t Get_SLOT_TYPE_from_file(char* file_name, char* _str_verb) {

	char buf[7];
	FILE* F_Slot_type;



	// Open file by File_Slot_type pointer in read mode
	F_Slot_type = fopen(file_name, "r");
	
	//Check file
	if (F_Slot_type == NULL) {
		printf("%s", V_SLOT_NOCFG_FILE);
		SLOT_CFG_DFLT();
		printf("%s", V_SLOT_TYPE_NO_CONFIG);
		return 0;
	}
 
	//Get data from file
	fscanf(F_Slot_type, "%c %c %c %c %c %c %c", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6]);

	for (uint8_t i=0; i < 7; i++) {
		if( buf[i]=='H') SLOT_TYPE_CFG[i]=SLOT_TYPE_HOST;			//Check if Host Port
		else if( buf[i]=='D') SLOT_TYPE_CFG[i]=SLOT_TYPE_ENDPOINT; 	//Check if Downstream port
	
		//printf("Buf[%d]=%c SLOT_TYPE_CFG[%d]=%d\n\r", i, buf[i],i, SLOT_TYPE_CFG[i]);
	}			
 
	
	sprintf(_str_verb, "%s", V_SLOT_TYPE_CONFIG);
 
	// closes the file pointed by F_Slot_type
	fclose(F_Slot_type);
	return 1;
}

		
	

/*______________________________________________________________________________________*/
/*!
 *\fn SLOT_CFG_DFLT()
 * \brief Initialization Default Slot Configuration
 *
 */
/*______________________________________________________________________________________*/

void SLOT_CFG_DFLT() {

    //Default configuration
	SLOT_TYPE_CFG[0] =	SLOT_TYPE_HOST;			//SLOT1 (P0): 	HOST		
	SLOT_TYPE_CFG[1] =	SLOT_TYPE_ENDPOINT;		//SLOT2 (P10):	ENDPOINT	
	SLOT_TYPE_CFG[2] =	SLOT_TYPE_ENDPOINT;		//SLOT3 (P20):	ENDPOINT	
	SLOT_TYPE_CFG[3] =	SLOT_TYPE_ENDPOINT;		//SLOT4 (P30):	ENDPOINT	
	SLOT_TYPE_CFG[4] =	SLOT_TYPE_ENDPOINT;		//SLOT5 (P40):	ENDPOINT	
	SLOT_TYPE_CFG[5] =	SLOT_TYPE_ENDPOINT;		//SLOT6 (P50):	ENDPOINT	
	SLOT_TYPE_CFG[6] =	SLOT_TYPE_ENDPOINT;		//SLOT7 (P96):	ENDPOINT
}




/*______________________________________________________________________________________*/
/*!
 *\fn Init_SW_CFG()
 * \brief Initialization Switch mode (Local or Remote) by checking each slot type
 *	if any Host port is detected then the mode will be "local" otherwise "remote"
 *
 *	MODE:0 => Local Mode
 *	MODE:1 => Remote Mode
 *
 *	Host_Port:0 => Slot1
 *	Host_Port:1 => Slot2
 *	Host_Port:2 => Slot3
 *	Host_Port:3 => Slot4
 *	Host_Port:4 => Slot5
 *	Host_Port:5 => Slot6
 *  Host_Port:5 => Slot7
 */
/*______________________________________________________________________________________*/

void Init_SW_CFG() {
	
	for(uint8_t i=0; i < 7; i++) {
		//Check SLOTx in Host Port
		if(!SLOT_TYPE_CFG[i]) {
			Sw_Cfg.Host_Port=i;				//Save the Host Port from SLOT_TYPE_CFG array
			break;
		}
	}

	//Echo_LFCR();
	//printf("HOST PORT: 0x%d\n\r", Sw_Cfg.Host_Port);

}




/*______________________________________________________________________________________*/
/*!
 *\fn Get_SLOT_CFG()
 * \brief Get Slots Configuration
 *
 * 0x00: HOST
 * 0x01: DOWNSTREAM (End Point)
 *
 */
/*______________________________________________________________________________________*/

void Get_SLOT_CFG(char* _str_verb) {

	char* str_data;

	//Check SLOT1 to SLOT7
	for(uint8_t i=1; i <= 7; i++) {

	//Memory allocations for string messages
	str_data = (char*) calloc(60, sizeof(char));

		switch(i) {
			//SLOT1
			case 0x01:
				switch(Slot1_Cfg.Type)	{
					//Host Port
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT1, V_SLOT_TYPE_HOST);
					break;

					//SLOT DOWNSTREAM PORT (EP)
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT1, V_SLOT_TYPE_DWN);
					break;

				}	//switch
				break;

			//SLOT2
			case 0x02:
				switch(Slot2_Cfg.Type)	{
					//Host Port
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT2, V_SLOT_TYPE_HOST);
					break;

					//SLOT DOWNSTREAM PORT (EP)
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT2, V_SLOT_TYPE_DWN);
					break;

				}	//switch
				break;

			//SLOT3
			case 0x03:
				switch(Slot3_Cfg.Type)	{
					//Host Port
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT3, V_SLOT_TYPE_HOST);
					break;

					//SLOT DOWNSTREAM PORT (EP)
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT3, V_SLOT_TYPE_DWN);
					break;

				}	//switch

				break;

			//SLOT4
			case 0x04:
				switch(Slot4_Cfg.Type)	{
					//Host Port
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT4, V_SLOT_TYPE_HOST);
					break;

					//SLOT DOWNSTREAM PORT (EP)
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT4, V_SLOT_TYPE_DWN);
					break;

				}	//switch
				break;

			//SLOT5
			case 0x05:
				switch(Slot5_Cfg.Type)	{
					//Host Port
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT5, V_SLOT_TYPE_HOST);
					break;

					//SLOT DOWNSTREAM PORT (EP)
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT5, V_SLOT_TYPE_DWN);
					break;

				}	//switch
				break;

			//SLOT6
			case 0x06:
				switch(Slot6_Cfg.Type)	{
					//Host Port
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT6, V_SLOT_TYPE_HOST);
					break;

					//SLOT DOWNSTREAM PORT (EP)
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT6, V_SLOT_TYPE_DWN);
					break;

				}	//switch
				break;

			//SLOT7
			case 0x07:
				switch(Slot7_Cfg.Type)	{
					//Host Port
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT7, V_SLOT_TYPE_HOST);
					break;

					//SLOT DOWNSTREAM PORT (EP)
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT7, V_SLOT_TYPE_DWN);
					break;

				}	//switch
				break;

		}	//switch

		strcat(_str_verb, str_data);
		free(str_data);

	} 	//for

}


/*______________________________________________________________________________________*/
/*!
 *\fn Init_SLOT_PCIe_RESET_CFG()
 * \brief Slot PCIe Reset Initialization
 *
 */
/*______________________________________________________________________________________*/

void Init_SLOT_CFG(char* _str_verb) {

	char* str_data;

	//Check SLOT1 to SLOT7
	for(uint8_t i=1; i <= 7; i++) {

		//Memory allocations for string messages
		str_data = (char*) calloc(60, sizeof(char));
	

		switch(i) {

			//***********
			//   SLOT1
			//***********
			case 0x01:

				Slot1_Cfg.Num = 1;
				Slot1_Cfg.Type = SLOT_TYPE_CFG[0];

				/*------------------------SLOT1 TYPE Identification----------------------------*/
				/* 0x00: HOST
				 * 0x01: DOWNSTREAM (End Point)
				*/
				switch(Slot1_Cfg.Type)	{

					//---------------------------
					//	    SLOT HOST PORT
					//			Type:0
					//---------------------------

					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT1, V_SLOT_TYPE_HOST);

						//PCIe Reset direction is set as input	(0:input/1:output)
						Slot1_Cfg.Reset_dir=0;
						GPIO_I2CEXP_DirWR(EXP0, SLOT1_PERST_N, INPUT);	//Dir as input
						ClrBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT1_PERST_DIR_BIT);

						//PCIe refclock disabled	(0:disabled/1:enabled)
						Slot1_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE1, LOW);		//SLOT1

						break;

					//---------------------------
					//	SLOT DOWNSTREAM PORT (EP)
					//			Type:1
					//---------------------------

					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT1, V_SLOT_TYPE_DWN);
						
						//PCIe Reset set as output	(0:input/1:output)
						Slot1_Cfg.Reset_dir=1;
						GPIO_I2CEXP_DirWR(EXP0, SLOT1_PERST_N, OUTPUT);	//Dir as output 
						GPIO_I2CEXP_WR(EXP0, SLOT1_PERST_N, LOW);		//Reset is asserted
						SetBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT1_PERST_DIR_BIT);

						//PCIe refclock disabled		(0:disabled/1:enabled)
						Slot1_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE1, LOW);	
						
						break;

				} //switch


				break;

			//***********
			//   SLOT2
			//***********
			case 0x02:

				Slot2_Cfg.Num = 2;
				Slot2_Cfg.Type = SLOT_TYPE_CFG[1];

				/*------------------------SLOT2 TYPE Identification----------------------------*/
				/* 0x00: HOST
				 * 0x01: DOWNSTREAM (End Point)
				*/
				switch(Slot2_Cfg.Type)	{


					//---------------------------
					//	    SLOT HOST PORT
					//			Type:0
					//---------------------------
					//PCIe Reset set as input
					//PCIe refclock disabled
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT2, V_SLOT_TYPE_HOST);

						//PCIe Reset direction is set as input	(0:input/1:output)
						Slot2_Cfg.Reset_dir=0;
						GPIO_I2CEXP_DirWR(EXP0, SLOT2_PERST_N, INPUT);	//Dir as input
						ClrBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT2_PERST_DIR_BIT);

						//PCIe refclock disabled	(0:disabled/1:enabled)
						Slot2_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE2, LOW);

						break;

					//---------------------------
					//	SLOT DOWNSTREAM PORT (EP)
					//			Type:1
					//---------------------------
					//PCIe Reset set as output
					//PCIe refclock disabled
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT2, V_SLOT_TYPE_DWN);

						//PCIe Reset set as output	(0:input/1:output)
						Slot2_Cfg.Reset_dir=1;
						GPIO_I2CEXP_DirWR(EXP0, SLOT2_PERST_N, OUTPUT);	//Dir as output 
						GPIO_I2CEXP_WR(EXP0, SLOT2_PERST_N, LOW);		//Reset is asserted						
						SetBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT2_PERST_DIR_BIT);

						//PCIe refclock disabled		(0:disabled/1:enabled)
						Slot2_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE2, LOW);

						break;

				} //switch

				break;

			//***********
			//   SLOT3
			//***********
			case 0x03:

				Slot3_Cfg.Num = 3;
				Slot3_Cfg.Type = SLOT_TYPE_CFG[2];

				/*------------------------SLOT3 TYPE Identification----------------------------*/
				/* 0x00: HOST
				 * 0x01: DOWNSTREAM (End Point)
				*/
				switch(Slot3_Cfg.Type)	{

					//---------------------------
					//	    SLOT HOST PORT
					//			Type:0
					//---------------------------
					//PCIe Reset set as input
					//PCIe refclock disabled
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT3, V_SLOT_TYPE_HOST);

						//PCIe Reset direction is set as input	(0:input/1:output)
						Slot3_Cfg.Reset_dir=0;
						GPIO_I2CEXP_DirWR(EXP0, SLOT3_PERST_N, INPUT);	//Dir as input
						ClrBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT3_PERST_DIR_BIT);

						//PCIe refclock disabled	(0:disabled/1:enabled)
						Slot3_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE3, LOW);

						break;

					//---------------------------
					//	SLOT DOWNSTREAM PORT (EP)
					//			Type:1
					//---------------------------
					//PCIe Reset set as output
					//PCIe refclock disabled
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT3, V_SLOT_TYPE_DWN);

						//PCIe Reset set as output	(0:input/1:output)
						Slot3_Cfg.Reset_dir=1;
						GPIO_I2CEXP_DirWR(EXP0, SLOT3_PERST_N, OUTPUT);	//Dir as output 
						GPIO_I2CEXP_WR(EXP0, SLOT3_PERST_N, LOW);		//Reset is asserted								
						SetBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT3_PERST_DIR_BIT);

						//PCIe refclock disabled		(0:disabled/1:enabled)
						Slot3_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE3, LOW);

						break;

				} //switch

				break;

			//***********
			//   SLOT4
			//***********
			case 0x04:

				Slot4_Cfg.Num = 4;
				Slot4_Cfg.Type = SLOT_TYPE_CFG[3];

				/*------------------------SLOT4 TYPE Identification----------------------------*/
				/* 0x00: HOST
				 * 0x01: DOWNSTREAM (End Point)
				*/
				switch(Slot4_Cfg.Type)	{


					//---------------------------
					//	    SLOT HOST PORT
					//			Type:0
					//---------------------------
					//PCIe Reset set as input
					//PCIe refclock disabled
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT4, V_SLOT_TYPE_HOST);

						//PCIe Reset direction is set as input	(0:input/1:output)
						Slot4_Cfg.Reset_dir=0;
						GPIO_I2CEXP_DirWR(EXP0, SLOT4_PERST_N, INPUT);	//Dir as input
						ClrBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT4_PERST_DIR_BIT);
	
						//PCIe refclock disabled	(0:disabled/1:enabled)
						Slot4_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE6, LOW);

						break;

					//---------------------------
					//	SLOT DOWNSTREAM PORT (EP)
					//			Type:1
					//---------------------------
					//PCIe Reset set as output
					//PCIe refclock disabled
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT4, V_SLOT_TYPE_DWN);

						//PCIe Reset set as output	(0:input/1:output)
						Slot4_Cfg.Reset_dir=1;
						GPIO_I2CEXP_DirWR(EXP0, SLOT4_PERST_N, OUTPUT);	//Dir as output 
						GPIO_I2CEXP_WR(EXP0, SLOT4_PERST_N, LOW);		//Reset is asserted	
						SetBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT4_PERST_DIR_BIT);

						//PCIe refclock disabled		(0:disabled/1:enabled)
						Slot4_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE6, LOW);

						break;


				} //switch

				break;

			//***********
			//   SLOT5
			//***********
			case 0x05:

				Slot5_Cfg.Num = 5;
				Slot5_Cfg.Type = SLOT_TYPE_CFG[4];

				/*------------------------SLOT5 TYPE Identification----------------------------*/
				/* 0x00: HOST
				 * 0x01: DOWNSTREAM (End Point)
				*/
				switch(Slot5_Cfg.Type)	{

					//---------------------------
					//	    SLOT HOST PORT
					//			Type:0
					//---------------------------
					//PCIe Reset set as input
					//PCIe refclock disabled
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT5, V_SLOT_TYPE_HOST);

						//PCIe Reset direction is set as input	(0:input/1:output)
						Slot5_Cfg.Reset_dir=0;
						GPIO_I2CEXP_DirWR(EXP0, SLOT5_PERST_N, INPUT);	//Dir as input
						ClrBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT5_PERST_DIR_BIT);

						//PCIe refclock disabled	(0:disabled/1:enabled)
						Slot5_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE5, LOW);

						break;

					//---------------------------
					//	SLOT DOWNSTREAM PORT (EP)
					//			Type:1
					//---------------------------
					//PCIe Reset set as output
					//PCIe refclock disabled
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT4, V_SLOT_TYPE_DWN);

						//PCIe Reset set as output	(0:input/1:output)
						Slot5_Cfg.Reset_dir=1;
						GPIO_I2CEXP_DirWR(EXP0, SLOT5_PERST_N, OUTPUT);	//Dir as output 
						GPIO_I2CEXP_WR(EXP0, SLOT5_PERST_N, LOW);		//Reset is asserted	
						SetBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT5_PERST_DIR_BIT);

						//PCIe refclock disabled		(0:disabled/1:enabled)
						Slot5_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE5, LOW);

						break;

				} //switch

				break;

			//***********
			//   SLOT6
			//***********
			case 0x06:

				Slot6_Cfg.Num = 6;
				Slot6_Cfg.Type = SLOT_TYPE_CFG[5];

				/*------------------------SLOT6 TYPE Identification----------------------------*/
				/* 0x00: HOST
				 * 0x01: DOWNSTREAM (End Point)
				*/
				switch(Slot6_Cfg.Type)	{


					//---------------------------
					//	    SLOT HOST PORT
					//			Type:0
					//---------------------------
					//PCIe Reset set as input
					//PCIe refclock disabled
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT6, V_SLOT_TYPE_HOST);
						
						//PCIe Reset direction is set as input	(0:input/1:output)
						Slot6_Cfg.Reset_dir=0;
						GPIO_I2CEXP_DirWR(EXP0, SLOT6_PERST_N, INPUT);	//Dir as input
						ClrBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT6_PERST_DIR_BIT);

						//PCIe refclock disabled	(0:disabled/1:enabled)
						Slot6_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE4, LOW);

						break;

					//---------------------------
					//	SLOT DOWNSTREAM PORT (EP)
					//			Type:1
					//---------------------------
					//PCIe Reset set as output
					//PCIe refclock disabled
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT6, V_SLOT_TYPE_DWN);

						//PCIe Reset set as output	(0:input/1:output)
						Slot6_Cfg.Reset_dir=1;
						GPIO_I2CEXP_DirWR(EXP0, SLOT6_PERST_N, OUTPUT);	//Dir as output 
						GPIO_I2CEXP_WR(EXP0, SLOT6_PERST_N, LOW);		//Reset is asserted							
						SetBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT6_PERST_DIR_BIT);

						//PCIe refclock disabled		(0:disabled/1:enabled)
						Slot6_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE4, LOW);

						break;

				} //switch

			//***********
			//   SLOT7
			//***********
			case 0x07:

				Slot7_Cfg.Num = 7;
				Slot7_Cfg.Type = SLOT_TYPE_CFG[6];

				/*------------------------SLOT7 TYPE Identification----------------------------*/
				/* 0x00: HOST
				 * 0x01: DOWNSTREAM (End Point)
				*/
				switch(Slot7_Cfg.Type)	{


					//---------------------------
					//	    SLOT HOST PORT
					//			Type:0
					//---------------------------
					//PCIe Reset set as input
					//PCIe refclock disabled
					case 0x00:
						sprintf(str_data, "%s%s", V_SLOT7, V_SLOT_TYPE_HOST);

						//PCIe Reset direction is set as input	(0:input/1:output)
						Slot7_Cfg.Reset_dir=0;
						GPIO_I2CEXP_DirWR(EXP0, SLOT7_PERST_N, INPUT);	//Dir as input
						ClrBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT7_PERST_DIR_BIT);

						//PCIe refclock disabled	(0:disabled/1:enabled)
						Slot7_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE7, LOW);

						break;

					//---------------------------
					//	SLOT DOWNSTREAM PORT (EP)
					//			Type:1
					//---------------------------
					//PCIe Reset set as output
					//PCIe refclock disabled
					case 0x01:
						sprintf(str_data, "%s%s", V_SLOT7, V_SLOT_TYPE_DWN);

						//PCIe Reset set as output	(0:input/1:output)
						Slot7_Cfg.Reset_dir=1;
						GPIO_I2CEXP_DirWR(EXP0, SLOT7_PERST_N, OUTPUT);	//Dir as output 
						GPIO_I2CEXP_WR(EXP0, SLOT7_PERST_N, LOW);		//Reset is asserted							
						SetBit(SLOT_PERST_DIR, SLT_RST_DIR__SLOT7_PERST_DIR_BIT);

						//PCIe refclock disabled		(0:disabled/1:enabled)
						Slot7_Cfg.Pcie_refclk_en=0;
						GPIO_I2CEXP_WR(EXP0, REFCLK_OE7, LOW);

						break;

				} //switch

				break;
		} //switch

		strcat(_str_verb, str_data);
		free(str_data);

	} //for
}



/*______________________________________________________________________________________*/
/*!
 *\fn Check_SYS_PCIe_RESET_MODE()
 * \brief Get System PCIe Reset Mode
 *
 * SYS_RST_MODE=0 => Mode: Auto
 * SYS_RST_MODE=1 => Mode: Manual
 *
 */
/*______________________________________________________________________________________*/

void Check_SYS_PCIe_RESET_MODE() {

/*
	Echo_LFCR(huart);
	if(SYS_PERST_MODE) UART_Print(huart, V_SW_SYS_RST_MODE_MANUAL);
	else UART_Print(huart, V_SW_SYS_RST_MODE_AUTO);
*/
}




/*______________________________________________________________________________________*/
/*!
 *\fn Check_SLOT_PRSNT_STATUS()
 * \brief Get Slot PCIe Card Presence Status
 *
 */
/*______________________________________________________________________________________*/


void Check_SLOT_PRSNT_STATUS() {

	//SLOT1
	if(!GPIO_I2CEXP_RD(EXP1, SLOT1_PRSNT_N)) SetBit(SLOT_PRSNT, SLT_PRSNT__SLOT1_PRSNT_BIT);
	else ClrBit(SLOT_PRSNT, SLT_PRSNT__SLOT1_PRSNT_BIT);

	//SLOT2
	if(!GPIO_I2CEXP_RD(EXP1, SLOT2_PRSNT_N)) SetBit(SLOT_PRSNT, SLT_PRSNT__SLOT2_PRSNT_BIT);
	else ClrBit(SLOT_PRSNT, SLT_PRSNT__SLOT2_PRSNT_BIT);

	//SLOT3
	if(!GPIO_I2CEXP_RD(EXP1, SLOT3_PRSNT_N)) SetBit(SLOT_PRSNT, SLT_PRSNT__SLOT3_PRSNT_BIT);
	else ClrBit(SLOT_PRSNT, SLT_PRSNT__SLOT3_PRSNT_BIT);

	//SLOT4
	if(!GPIO_I2CEXP_RD(EXP1, SLOT4_PRSNT_N)) SetBit(SLOT_PRSNT, SLT_PRSNT__SLOT4_PRSNT_BIT);
	else ClrBit(SLOT_PRSNT, SLT_PRSNT__SLOT4_PRSNT_BIT);

	//SLOT5
	if(!GPIO_I2CEXP_RD(EXP1, SLOT5_PRSNT_N)) SetBit(SLOT_PRSNT, SLT_PRSNT__SLOT5_PRSNT_BIT);
	else ClrBit(SLOT_PRSNT, SLT_PRSNT__SLOT5_PRSNT_BIT);

	//SLOT6
	if(!GPIO_I2CEXP_RD(EXP1, SLOT6_PRSNT_N)) SetBit(SLOT_PRSNT, SLT_PRSNT__SLOT6_PRSNT_BIT);
	else ClrBit(SLOT_PRSNT, SLT_PRSNT__SLOT6_PRSNT_BIT);

	//SLOT7
	if(!GPIO_I2CEXP_RD(EXP1, SLOT7_PRSNT_N)) SetBit(SLOT_PRSNT, SLT_PRSNT__SLOT7_PRSNT_BIT);
	else ClrBit(SLOT_PRSNT, SLT_PRSNT__SLOT7_PRSNT_BIT);

}



/*______________________________________________________________________________________*/
/*!
 *\fn Check_CARD_PRSNT_STATUS()
 * \brief Get Slot PCIe Card Presence Status
 *
 */
/*______________________________________________________________________________________*/

void Check_CARD_PRSNT_STATUS(char *_str_verb) {


	char* str_data;


	//Memory allocations for string messages
	str_data = (char*) calloc(60, sizeof(char));

	Check_SLOT_PRSNT_STATUS();


	//SLOT1
	if( (SLOT_PRSNT >> SLT_PRSNT__SLOT1_PRSNT_BIT) & 1) sprintf(str_data, "\n\r%s%s", V_SLOT1, V_CARD_PRESENCE);
	else sprintf(str_data, "\n\r%s%s", V_SLOT1, V_CARD_NOPRESENCE);
	strcat(_str_verb, str_data);

	//SLOT2
	if( (SLOT_PRSNT >> SLT_PRSNT__SLOT2_PRSNT_BIT) & 1) sprintf(str_data, "%s%s", V_SLOT2, V_CARD_PRESENCE);
	else sprintf(str_data, "%s%s", V_SLOT2, V_CARD_NOPRESENCE);
	strcat(_str_verb, str_data);

	//SLOT3
	printf("%s", V_SLOT3);
	if( (SLOT_PRSNT >> SLT_PRSNT__SLOT3_PRSNT_BIT) & 1) sprintf(str_data, "%s%s", V_SLOT3, V_CARD_PRESENCE);
	else sprintf(str_data, "%s%s", V_SLOT3, V_CARD_NOPRESENCE);
	strcat(_str_verb, str_data);

	//SLOT4
	if( (SLOT_PRSNT >> SLT_PRSNT__SLOT4_PRSNT_BIT) & 1) sprintf(str_data, "%s%s", V_SLOT4, V_CARD_PRESENCE);
	else sprintf(str_data, "%s%s", V_SLOT4, V_CARD_NOPRESENCE);
	strcat(_str_verb, str_data);

	//SLOT5
	if( (SLOT_PRSNT >> SLT_PRSNT__SLOT5_PRSNT_BIT) & 1) sprintf(str_data, "%s%s", V_SLOT5, V_CARD_PRESENCE);
	else sprintf(str_data, "%s%s", V_SLOT5, V_CARD_NOPRESENCE);
	strcat(_str_verb, str_data);

	//SLOT6
	if( (SLOT_PRSNT >> SLT_PRSNT__SLOT6_PRSNT_BIT) & 1) sprintf(str_data, "%s%s", V_SLOT6, V_CARD_PRESENCE);
	else sprintf(str_data, "%s%s", V_SLOT6, V_CARD_NOPRESENCE);
	strcat(_str_verb, str_data);

	//SLOT7
	if( (SLOT_PRSNT >> SLT_PRSNT__SLOT7_PRSNT_BIT) & 1) sprintf(str_data, "%s%s", V_SLOT7, V_CARD_PRESENCE);
	else sprintf(str_data, "%s%s", V_SLOT7, V_CARD_NOPRESENCE);
	strcat(_str_verb, str_data);

	free(str_data);

}
