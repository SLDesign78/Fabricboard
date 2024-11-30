/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_POWER_REFCLK_RESET.c
*************************************************************************************************************/
//  Description  : Power, Reclock, Reset PCIe control
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 10/05/2024: creation

*************************************************************************************************************/

#include "fabricboard.h"


/********************************************************************************
 *
 * 	                            S Y S    P O W E R
 *
 *********************************************************************************/


/*	STATES Finite States Machine
 * 0: ST_INIT
 * 1: ST_UP_P3V3
 * 2: ST_UP_P0V9
 * 3: ST_UP_P1V8
 * 4: ST_PWR_OK
 * 5: ST_DWN_P1V8
 * 6: ST_DWN_P0V9
 * 7: ST_DWN_P3V3
 * 8: ST_PWR_FAULT
 * 9: ST_PWR_OFF
*/

void SYS_Power_CTRL(void) {

	/*-------------------------------------------------------------------------------------
									STATE #0: ST_INIT STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_INIT ) {
		FSM_PWR_CURR=ucPWR_STATE;
		FSM_PWR_ERR=ucPWR_STATE;
		FSM_VR_Timeout = VR_PG_TIMEOUT;	//Reset timeout

		// Update FSM Power Status
		ClrBit(FSM_PWR_STATUS, FSM_PWR_STATUS__PWR_FAULT_BIT);
		ClrBit(PWR_STATUS, PWR_STATUS__PWR_OK_BIT);
		ClrBit(PWR_STATUS, PWR_STATUS__PWR_FAIL_BIT);

		//Wait power ON is detected
		if( SYS_PWR_EN && !SYS_PWR_EPO ) {	//Power ON only if EPO is not asserted
			printf("%s", V_PWR_ON);
			ucPWR_STATE = ST_UP_P3V3;	//next state
		}
	}

	/*-------------------------------------------------------------------------------------
								   STATE #9: ST_PWR_OFF STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_PWR_OFF ) {
		FSM_PWR_CURR=ucPWR_STATE;

		//WR_PIN_PORT(GPIOB, VR_P3V3_EN_Pin, LOW);	//P3V3 OFF
		//WR_PIN_PORT(GPIOB, VR_P0V9_EN_Pin, LOW);	//P0V9 OFF
		//WR_PIN_PORT(GPIOB, VR_P1V8_EN_Pin, LOW);	//P1V8 OFF

		SYS_PWR_OK=LOW;	//POWER_OK disabled
		ClrBit(FSM_PWR_STATUS, FSM_PWR_STATUS__PWR_OK_BIT);	//Update FSM Power Status
		ClrBit(PWR_STATUS, PWR_STATUS__PWR_OK_BIT);
		ClrBit(PWR_STATUS, PWR_STATUS__PWR_FAIL_BIT);

		//Update CAN Registers
		/*
		PWR_EN_REG=PWR_OFF;
		PWR_PG_REG=PWR_OFF;
		FSM_PWR_STATUS=PWR_OFF;
		*/


		if( SYS_PWR_EPO ) {		//Emergency Power OFF
			printf("%s", V_PWR_EPO);
			ucPWR_STATE = ST_INIT;
		}
		else if ( !SYS_PWR_EN ) {	//Power OFF
			printf("%s", V_PWR_OFF);
			ucPWR_STATE = ST_INIT;
		}
	}

	/*-------------------------------------------------------------------------------------
								 STATE #1: ST_UP_P3V3 STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_UP_P3V3 ) {
		FSM_PWR_CURR=ucPWR_STATE;

		if( !SYS_PWR_EN || SYS_PWR_EPO ) {	//Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}

		//P3V3 ON
		GPIO_I2CEXP_WR(EXP1, P3V3_EN, HIGH);
		SetBit(PWR_EN_REG, PWR_EN_REG__P3V3_EN_BIT);

		//Check P3V3_PG
		if(GPIO_I2CEXP_RD(EXP1, P3V3_PG)) {
			SetBit(PWR_PG_REG, PWR_PG_REG__P3V3_PG_BIT);
			printf("%s", V_P3V3_OK);

			FSM_VR_Timeout = VR_PG_TIMEOUT;	//Reset timeout
			ucPWR_STATE = ST_UP_P0V9;		//next state

		}
		else if(FSM_VR_Timeout) {
			FSM_VR_Timeout--;
		}
		else {	//Timeout detected
			FSM_PWR_ERR=ucPWR_STATE;
			printf("%s", V_P3V3_PG_KO);
			ucPWR_STATE = ST_PWR_FAULT;
		}
	}


	/*-------------------------------------------------------------------------------------
								 STATE #2: ST_UP_P0V9 STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_UP_P0V9 ) {
		FSM_PWR_CURR=ucPWR_STATE;

		if( !SYS_PWR_EN || SYS_PWR_EPO ) {	//Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}

		//P0V9 ON
		GPIO_I2CEXP_WR(EXP1, VR_P0V9_EN, HIGH);
		SetBit(PWR_EN_REG, PWR_EN_REG__VR_P0V9_EN_BIT);

		//Check P0V9_PG
		if(GPIO_I2CEXP_RD(EXP1, VR_P0V9_PG)) {
			SetBit(PWR_PG_REG, PWR_PG_REG__VR_P0V9_PG_BIT);
			printf("%s", V_P0V9_OK);

			FSM_VR_Timeout = VR_PG_TIMEOUT;	//Reset timeout
			ucPWR_STATE = ST_UP_P1V8;		//next state
		}
		else if(FSM_VR_Timeout) {
			FSM_VR_Timeout--;
		}
		else {	//Timeout detected
			FSM_PWR_ERR=ucPWR_STATE;
			printf("%s", V_P0V9_PG_KO);
			ucPWR_STATE = ST_PWR_FAULT;
		}
			
		
	}



	/*-------------------------------------------------------------------------------------
									STATE #3: ST_UP_P1V8 STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_UP_P1V8 ) {
		FSM_PWR_CURR=ucPWR_STATE;

		if( !SYS_PWR_EN || SYS_PWR_EPO ) {	//Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}

		//P1V8 ON
		GPIO_I2CEXP_WR(EXP1, VR_P1V8_EN, HIGH);
		SetBit(PWR_EN_REG, PWR_EN_REG__VR_P1V8_EN_BIT);

		//Check P1V8_PG
		if(GPIO_I2CEXP_RD(EXP1, VR_P1V8_PG)) {
			SetBit(PWR_PG_REG, PWR_PG_REG__VR_P1V8_PG_BIT);
			printf("%s", V_P1V8_OK);
			printf("%s", V_PWR_OK);
			
			FSM_VR_Timeout = VR_PG_TIMEOUT;	//Reset timeout
			ucPWR_STATE = ST_PWR_OK;		//next state
		}
		else if(FSM_VR_Timeout) {
			FSM_VR_Timeout--;
		}
		else {	//Timeout detected
			FSM_PWR_ERR=ucPWR_STATE;
			printf("%s", V_P1V8_PG_KO);
			ucPWR_STATE = ST_PWR_FAULT;
		}
	}


	/*-------------------------------------------------------------------------------------
								 STATE #4: ST_PWR_OK STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_PWR_OK ) {
		FSM_PWR_CURR=ucPWR_STATE;

		SYS_PWR_OK=HIGH;	//POWER_OK enabled
		SetBit(FSM_PWR_STATUS, FSM_PWR_STATUS__PWR_OK_BIT);	//Update FSM Power Status
		SetBit(PWR_STATUS, PWR_STATUS__PWR_OK_BIT);			//Update Power Status

		if( !SYS_PWR_EN ) {	//Power down asserted
			ucPWR_STATE = ST_DWN_P1V8;
		}
		else if ( SYS_PWR_EPO ) { //Emergency Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}
		
		//Check P3V3_PG
		if(!GPIO_I2CEXP_RD(EXP1, P3V3_PG)) {
			FSM_PWR_ERR=ucPWR_STATE;
			ClrBit(PWR_PG_REG, PWR_PG_REG__P3V3_PG_BIT);
			printf("%s", V_P3V3_PG_KO);
			ucPWR_STATE = ST_PWR_FAULT;
		}
		//Check P0V9_PG
		else if(!GPIO_I2CEXP_RD(EXP1, VR_P0V9_PG)) {
			FSM_PWR_ERR=ucPWR_STATE;
			ClrBit(PWR_PG_REG, PWR_PG_REG__VR_P0V9_PG_BIT);
			printf("%s", V_P0V9_PG_KO);
			ucPWR_STATE = ST_PWR_FAULT;
		}
		//Check P1V8_PG
		else if(!GPIO_I2CEXP_RD(EXP1, VR_P1V8_PG)) {
			FSM_PWR_ERR=ucPWR_STATE;
			ClrBit(PWR_PG_REG, PWR_PG_REG__VR_P1V8_PG_BIT);
			printf("%s", V_P1V8_PG_KO);
			ucPWR_STATE = ST_PWR_FAULT;

		}
	}


	/*-------------------------------------------------------------------------------------
								STATE #5: ST_DWN_P1V8 STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_DWN_P1V8 ) {
		FSM_PWR_CURR=ucPWR_STATE;

		if( SYS_PWR_EPO ) {	//Emergency Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}

		SYS_PWR_OK=LOW;	//POWER_OK disabled
		ClrBit(FSM_PWR_STATUS, FSM_PWR_STATUS__PWR_OK_BIT);	//Update FSM Power Status
		ClrBit(PWR_STATUS, PWR_STATUS__PWR_OK_BIT);			//Update Power Status

		//P1V8 OFF
		GPIO_I2CEXP_WR(EXP1, VR_P1V8_EN, LOW);
		ClrBit(PWR_EN_REG, PWR_EN_REG__VR_P1V8_EN_BIT);

		//Check P1V8_PG
		if(!GPIO_I2CEXP_RD(EXP1, VR_P1V8_PG)) {
			ClrBit(PWR_PG_REG, PWR_PG_REG__VR_P1V8_PG_BIT);
			printf("%s", V_P1V8_OFF);
			ucPWR_STATE = ST_DWN_P0V9;		//next state
		}

	}



	/*-------------------------------------------------------------------------------------
								STATE #6: ST_DWN_P0V9 STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_DWN_P0V9 ) {
		FSM_PWR_CURR=ucPWR_STATE;

		if( SYS_PWR_EPO ) {	//Emergency Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}

		//P0V9 OFF
		GPIO_I2CEXP_WR(EXP1, VR_P0V9_EN, LOW);
		ClrBit(PWR_EN_REG, PWR_EN_REG__VR_P0V9_EN_BIT);

		//Check P0V9_PG
		if(!GPIO_I2CEXP_RD(EXP1, VR_P0V9_PG)) {
			ClrBit(PWR_PG_REG, PWR_PG_REG__VR_P0V9_PG_BIT);
			printf("%s", V_P0V9_OFF);
			ucPWR_STATE = ST_DWN_P3V3;		//next state
		}

	}



	/*-------------------------------------------------------------------------------------
								STATE #7: ST_DWN_P3V3 STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_DWN_P3V3 ) {
		FSM_PWR_CURR=ucPWR_STATE;

		if( SYS_PWR_EPO ) {	//Emergency Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}

		//P3V3 OFF
		GPIO_I2CEXP_WR(EXP1, P3V3_EN, LOW);
		ClrBit(PWR_EN_REG, PWR_EN_REG__P3V3_EN_BIT);

		//Check P3V3_PG
		if(!GPIO_I2CEXP_RD(EXP1, P3V3_PG)) {
			ClrBit(PWR_PG_REG, PWR_PG_REG__P3V3_PG_BIT);
			printf("%s", V_P3V3_OFF);
			ucPWR_STATE = ST_PWR_OFF;		//Back to Init state
		}
	}





	/*-------------------------------------------------------------------------------------
								STATE #8: ST_PWR_FAULT STATE
	-------------------------------------------------------------------------------------*/
	if( ucPWR_STATE == ST_PWR_FAULT ) {
		FSM_PWR_CURR=ST_PWR_FAULT;

		GPIO_I2CEXP_WR(EXP1, P3V3_EN, LOW);       //P3V3 OFF
		GPIO_I2CEXP_WR(EXP1, VR_P0V9_EN, LOW);    //P0V9 OFF
		GPIO_I2CEXP_WR(EXP1, VR_P1V8_EN, LOW);    //P1V8 OFF


		SYS_PWR_OK=LOW;	//POWER_OK disabled
		ClrBit(FSM_PWR_STATUS, FSM_PWR_STATUS__PWR_OK_BIT);	//Update FSM Power Status
		ClrBit(PWR_STATUS, PWR_STATUS__PWR_OK_BIT);			//Update Power Status

		if( !SYS_PWR_EN || SYS_PWR_EPO ) {	//Power OFF
			ucPWR_STATE = ST_PWR_OFF;
		}

		SetBit(FSM_PWR_STATUS, FSM_PWR_STATUS__PWR_FAULT_BIT);	// Update FSM Power Status
		SetBit(PWR_STATUS, PWR_STATUS__PWR_FAIL_BIT);			//Update Power Status
	}

}






/********************************************************************************
 *
 * 	                       S Y S    P C I E    R E F C L O C K
 *
 *********************************************************************************/


void SYS_Refclk_CTRL(void) {

	/*-------------------------------------------------------------------------------------
								STATE #0:  ST_REFCLK_OFF STATE
	-------------------------------------------------------------------------------------*/

	//Refclock OFF
	if( ucREFCLK_STATE == ST_REFCLK_OFF ) {

		//Check if Power OK is enabled and stable
		if(SYS_PWR_OK && SYS_PWR_EN && !SYS_PWR_EPO) {

			//PEX: Refclock enabled
			GPIO_I2CEXP_WR(EXP0, REFCLK_OE0, HIGH);	
			SetBit(MSC_CLK_CFG, MSC_CLK_CFG__PEX_CLK_EN_BIT);


			//*** SLOTx: Refclock enabled ***
			//REFCLOCK SLOT1 enabled
			if(Slot1_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE1, HIGH);	
				SetBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT1_CLK_EN_BIT);
			}

			//REFCLOCK SLOT2 enabled
			if(Slot2_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE2, HIGH);
				SetBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT2_CLK_EN_BIT);
			}

			//REFCLOCK SLOT3 enabled
			if(Slot3_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE3, HIGH);
				SetBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT3_CLK_EN_BIT);
			}

			//REFCLOCK SLOT4 enabled
			if(Slot4_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE6, HIGH);
				SetBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT4_CLK_EN_BIT);
			}

			//REFCLOCK SLOT5 enabled
			if(Slot5_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE5, HIGH);
				SetBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT5_CLK_EN_BIT);
			}

			//REFCLOCK SLOT6 enabled
			if(Slot6_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE4, HIGH);
				SetBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT6_CLK_EN_BIT);
			}

			//REFCLOCK SLOT7 enabled
			if(Slot7_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE7, HIGH);
				SetBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT7_CLK_EN_BIT);
			}


			//Refclock status
			REFCLK_ON=0x01;
			SetBit(REFCLK_STATUS, SYS_REFCLK__REFCLK_ON_BIT);
			printf("%s", V_REFCLK_ON);Echo_LFCR();
			ucREFCLK_STATE = ST_REFCLK_ON;
		}
	}



	/*-------------------------------------------------------------------------------------
								STATE #1:  ST_REFCLK_ON STATE
	-------------------------------------------------------------------------------------*/


	if( ucREFCLK_STATE == ST_REFCLK_ON) {

		//Check if Power OFF is detected
		if( !SYS_PWR_EN || SYS_PWR_EPO ) {

			//PEX: Refclock disabled
			GPIO_I2CEXP_WR(EXP0, REFCLK_OE0, LOW);
			ClrBit(MSC_CLK_CFG, MSC_CLK_CFG__PEX_CLK_EN_BIT);

			//*** SLOTx: Refclock disabled ***
			//REFCLOCK SLOT1 disabled
			if(Slot1_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE1, LOW);
				ClrBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT1_CLK_EN_BIT);
			}

			//REFCLOCK SLOT2 disabled
			if(Slot2_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE2, LOW);
				ClrBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT2_CLK_EN_BIT);
			}

			//REFCLOCK SLOT3 disabled
			if(Slot3_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE3, LOW);
				ClrBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT3_CLK_EN_BIT);
			}

			//REFCLOCK SLOT4 disabled
			if(Slot4_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE6, LOW);
				ClrBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT4_CLK_EN_BIT);
			}

			//REFCLOCK SLOT5 disabled
			if(Slot5_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE5, LOW);
				ClrBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT5_CLK_EN_BIT);
			}

			//REFCLOCK SLOT6 disabled
			if(Slot6_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE4, LOW);
				ClrBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT6_CLK_EN_BIT);
			}

			//REFCLOCK SLOT7 disabled
			if(Slot7_Cfg.Type==SLOT_TYPE_ENDPOINT) {
				GPIO_I2CEXP_WR(EXP0, REFCLK_OE7, LOW);
				ClrBit(SLOT_CLK_CFG, SLT_CLK_CFG__SLOT7_CLK_EN_BIT);
			}


			//Refclock status
			REFCLK_ON=0x00;
			ClrBit(REFCLK_STATUS, SYS_REFCLK__REFCLK_ON_BIT);
			printf("%s", V_REFCLK_OFF);Echo_LFCR();
			ucREFCLK_STATE = ST_REFCLK_OFF;
		}
	}
}





/********************************************************************************
 *
 * 	                       S Y S    P C I E    R E S E T
 *
 *********************************************************************************/


/*______________________________________________________________________________________*/
/*!
 *\fn PERST_ASSERT(void)
 * \brief PCIe Reset asserted
 */
/*______________________________________________________________________________________*/

void PERST_Assert(void) {

	//PEX: PERST_N asserted
	GPIO_I2CEXP_WR(EXP0, PEX_PERST_N, LOW);									//PEX_PERST_N asserted
	ClrBit(MSC_PERST_STATUS, MSC_RST_STATUS__PEX_PERST_N_BIT);


	if(Slot1_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT1_PERST_N, LOW);							//SLOT1_PERST_N asserted
		ClrBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT1_PERST_N_BIT);
	}
	if(Slot2_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT2_PERST_N, LOW);							//SLOT2_PERST_N asserted
		ClrBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT2_PERST_N_BIT);
	}
	if(Slot3_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT3_PERST_N, LOW);							//SLOT3_PERST_N asserted
		ClrBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT3_PERST_N_BIT);
	}
	if(Slot4_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT4_PERST_N, LOW);							//SLOT4_PERST_N asserted
		ClrBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT4_PERST_N_BIT);
	}
	if(Slot5_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT5_PERST_N, LOW);							//SLOT5_PERST_N asserted
		ClrBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT5_PERST_N_BIT);
	}
	if(Slot6_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT5_PERST_N, LOW);							//SLOT6_PERST_N asserted
		ClrBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT6_PERST_N_BIT);
	}
	if(Slot7_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT7_PERST_N, LOW);							//SLOT7_PERST_N asserted
		ClrBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT7_PERST_N_BIT);
	}
}



/*______________________________________________________________________________________*/
/*!
 *\fn PERST_DEASSERT(void)
 * \brief PCIe Reset de-asserted
 */
/*______________________________________________________________________________________*/

void PERST_Deassert(void) {

	//PEX: PERST_N de-asserted
	GPIO_I2CEXP_WR(EXP0, PEX_PERST_N, HIGH);				//PEX_PERST_N de-asserted
	SetBit(MSC_PERST_STATUS, MSC_RST_STATUS__PEX_PERST_N_BIT);


	if(Slot1_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT1_PERST_N, HIGH);								//SLOT1_PERST_N de-asserted
		SetBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT1_PERST_N_BIT);
	}
	if(Slot2_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT2_PERST_N, HIGH);								//SLOT2_PERST_N de-asserted
		SetBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT2_PERST_N_BIT);
	}
	if(Slot3_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT3_PERST_N, HIGH);								//SLOT3_PERST_N de-asserted
		SetBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT3_PERST_N_BIT);
	}
	if(Slot4_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT4_PERST_N, HIGH);								//SLOT4_PERST_N de-asserted
		SetBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT4_PERST_N_BIT);
	}
	if(Slot5_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT5_PERST_N, HIGH);								//SLOT5_PERST_N de-asserted
		SetBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT5_PERST_N_BIT);
	}
	if(Slot6_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT6_PERST_N, HIGH);								//SLOT6_PERST_N de-asserted
		SetBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT6_PERST_N_BIT);
	}
	if(Slot7_Cfg.Type==SLOT_TYPE_ENDPOINT) {
		GPIO_I2CEXP_WR(EXP0, SLOT7_PERST_N, HIGH);								//SLOT7_PERST_N de-asserted
		SetBit(SLOT_PERST_STATUS, SLT_RST_STATUS__SLOT7_PERST_N_BIT);
	}
}




/*______________________________________________________________________________________*/
/*!
 *\fn SYS_Reset_CTRL(UART_HandleTypeDef *huart)
 * \brief System PCIe Reset Control
 */
/*______________________________________________________________________________________*/


void SYS_Reset_CTRL(void) {

	/*-------------------------------------------------------------------------------------
								STATE #0:  ST_RST_INIT STATE
	-------------------------------------------------------------------------------------*/

	//Wait Power OK state
	if( ucRST_STATE == ST_RST_INIT ) {

		//Check if Power OK is enabled
		if(SYS_PWR_OK && REFCLK_ON) {

			//Check which Slot is Host port
			switch(Sw_Cfg.Host_Port) {
				case HOST_SLOT1:
					printf("%s%s", V_SLOT1, V_HOST_PERST_ASSERT);
					break;

				case HOST_SLOT2:
					printf("%s%s", V_SLOT2, V_HOST_PERST_ASSERT);
					break;

				case HOST_SLOT3:
					printf("%s%s", V_SLOT3, V_HOST_PERST_ASSERT);
					break;

				case HOST_SLOT4:
					printf("%s%s", V_SLOT4, V_HOST_PERST_ASSERT);
					break;

				case HOST_SLOT5:
					printf("%s%s", V_SLOT5, V_HOST_PERST_ASSERT);
					break;

				case HOST_SLOT6:
					printf("%s%s", V_SLOT6, V_HOST_PERST_ASSERT);
					break;

				case HOST_SLOT7:
					printf("%s%s", V_SLOT7, V_HOST_PERST_ASSERT);
					break;				
			}
			ucRST_STATE = ST_RST_ASSERTED;
		}
	}


	/*-------------------------------------------------------------------------------------
								STATE #1:  ST_RST_PWROFF STATE
	-------------------------------------------------------------------------------------*/

	//Power OFF is detected
	if( ucRST_STATE == ST_RST_PWROFF ) {

		//PEX, SLOTx: PERST_N asserted
		PERST_Assert();

		ucRST_STATE = ST_RST_INIT;
	}


	/*-------------------------------------------------------------------------------------
								STATE #2:  ST_RST_ASSERTED STATE
	-------------------------------------------------------------------------------------*/

	//PCIe Reset from Host is asserted
	if( ucRST_STATE == ST_RST_ASSERTED ) {

		ClrBit(SYS_STATUS, SYS_STATUS__SYS_OK_BIT);		//Update System Status

		//Check if Power OK and Refclock are always enabled
		if(!SYS_PWR_OK) {
			ucRST_STATE = ST_RST_PWROFF;
		}

		//PEX, SLOTx: PERST_N asserted
		PERST_Assert();
		//delay_ms(1);

		//Check which Slot is Host port and if the PCie Reset is de-asserted
		switch(Sw_Cfg.Host_Port) {

			//***********
			//   SLOT1
			//***********
			case HOST_SLOT1:
				
				if(GPIO_I2CEXP_RD(EXP0, SLOT1_PERST_N)) {
					printf("%s%s", V_SLOT1, V_HOST_PERST_DEASSERT);
					ucRST_STATE = ST_RST_DEASSERTED;
				}
				break;



			//***********
			//   SLOT2
			//***********
			case HOST_SLOT2:
				
				if(GPIO_I2CEXP_RD(EXP0, SLOT2_PERST_N)) {
					printf("%s%s", V_SLOT2, V_HOST_PERST_DEASSERT);
					ucRST_STATE = ST_RST_DEASSERTED;
				}
				break;


			//***********
			//   SLOT3
			//***********
			case HOST_SLOT3:

				if(GPIO_I2CEXP_RD(EXP0, SLOT3_PERST_N)) {
					printf("%s%s", V_SLOT3, V_HOST_PERST_DEASSERT);
					ucRST_STATE = ST_RST_DEASSERTED;
				}
				break;


			//***********
			//   SLOT4
			//***********
			case HOST_SLOT4:

				if(GPIO_I2CEXP_RD(EXP0, SLOT4_PERST_N)) {
					printf("%s%s", V_SLOT4, V_HOST_PERST_DEASSERT);
					ucRST_STATE = ST_RST_DEASSERTED;
				}
				break;


			//***********
			//   SLOT5
			//***********
			case HOST_SLOT5:
				
				if(GPIO_I2CEXP_RD(EXP0, SLOT5_PERST_N)) {
					printf("%s%s", V_SLOT5, V_HOST_PERST_DEASSERT);
					ucRST_STATE = ST_RST_DEASSERTED;
				}
				break;

			//***********
			//   SLOT6
			//***********
			case HOST_SLOT6:

				if(GPIO_I2CEXP_RD(EXP0, SLOT6_PERST_N)) {
					printf("%s%s", V_SLOT6, V_HOST_PERST_DEASSERT);
					ucRST_STATE = ST_RST_DEASSERTED;
				}
				break;
		
			//***********
			//   SLOT7
			//***********
			case HOST_SLOT7:

				if(GPIO_I2CEXP_RD(EXP0, SLOT7_PERST_N)) {
					printf("%s%s", V_SLOT7, V_HOST_PERST_DEASSERT);
					ucRST_STATE = ST_RST_DEASSERTED;
				}
				break;		
		
		} //switch

	} //if

	/*-------------------------------------------------------------------------------------
							STATE #3:  ST_RST_DEASSERTED STATE
	-------------------------------------------------------------------------------------*/

	//PCIe Reset from Host is de-asserted
	if( ucRST_STATE == ST_RST_DEASSERTED ) {

		SetBit(SYS_STATUS, SYS_STATUS__SYS_OK_BIT);		//Update System Status

		//Check if Power OK is always enabled
		if(!SYS_PWR_OK) {
			ucRST_STATE = ST_RST_PWROFF;
		}

		//PEX, SLOTx: PERST_N de-asserted
		PERST_Deassert();
		//delay(10);

		//Check what Slot is Host port and if PCie Reset is asserted
		switch(Sw_Cfg.Host_Port) {

			//***********
			//   SLOT1
			//***********
			case HOST_SLOT1:
				
				if(!GPIO_I2CEXP_RD(EXP0, SLOT1_PERST_N)) {
					printf("%s%s", V_SLOT1, V_HOST_PERST_ASSERT);
					ucRST_STATE = ST_RST_ASSERTED;
				}
				
				break;


			//***********
			//   SLOT2
			//***********
			case HOST_SLOT2:
				
				if(!GPIO_I2CEXP_RD(EXP0, SLOT2_PERST_N)) {
					printf("%s%s", V_SLOT2, V_HOST_PERST_ASSERT);
					ucRST_STATE = ST_RST_ASSERTED;
				}
				
				break;


			//***********
			//   SLOT3
			//***********
			case HOST_SLOT3:
				
				if(!GPIO_I2CEXP_RD(EXP0, SLOT3_PERST_N)) {
					printf("%s%s", V_SLOT3, V_HOST_PERST_ASSERT);
					ucRST_STATE = ST_RST_ASSERTED;
				}
				
				break;


			//***********
			//   SLOT4
			//***********
			case HOST_SLOT4:
				
				if(!GPIO_I2CEXP_RD(EXP0, SLOT4_PERST_N)) {
					printf("%s%s", V_SLOT4, V_HOST_PERST_ASSERT);
					ucRST_STATE = ST_RST_ASSERTED;
				}
				
				break;


			//***********
			//   SLOT5
			//***********
			case HOST_SLOT5:
				
				if(!GPIO_I2CEXP_RD(EXP0, SLOT5_PERST_N)) {
					printf("%s%s", V_SLOT5, V_HOST_PERST_ASSERT);
					ucRST_STATE = ST_RST_ASSERTED;
				}
				
				break;


			//***********
			//   SLOT6
			//***********
			case HOST_SLOT6:
				
				if(!GPIO_I2CEXP_RD(EXP0, SLOT6_PERST_N)) {
					printf("%s%s", V_SLOT6, V_HOST_PERST_ASSERT);
					ucRST_STATE = ST_RST_ASSERTED;
				}
				
				break;
		
			//***********
			//   SLOT7
			//***********
			case HOST_SLOT7:
				
				if(!GPIO_I2CEXP_RD(EXP0, SLOT7_PERST_N)) {
					printf("%s%s", V_SLOT7, V_HOST_PERST_ASSERT);
					ucRST_STATE = ST_RST_ASSERTED;
				}
				
				break;		
		
		
		} //switch

	} //if

}


/*______________________________________________________________________________________*/
/*!
 *\fn void Set_PERST_MODE_to_EEP(uint16_t, uint8_t)
 * \brief Set System PCIe Reset to Eeprom
 *
 * SYS_PERST_MODE:
 *	0: PCIe Reset AUTO
 *	1: PCIe Reset Manual
 *
 */
/*______________________________________________________________________________________*/

/*
void Set_PERST_MODE_to_EEP(uint16_t SYS_PERST_MODE_CFG_EEP_ADD, uint8_t* p_SYS_PERST_MODE_CFG_Data, uint8_t count_data) {
	ret=Write_EEPROM(MEM_24LC512_ADDR, SYS_PERST_MODE_CFG_EEP_ADD, p_SYS_PERST_MODE_CFG_Data, count_data);
	if (ret != HAL_OK) {
		//UART_Print(huart, V_I2C_WRITE_ERR);		=> to be changed by Write I2C CAN Reg
	}
}
*/


/*______________________________________________________________________________________*/
/*!
 *\fn void Get_PERST_MODE_from_EEP(uint8_t Slot_type_CFG_EEP_ADD)
 * \brief Get System PCIe Reset to Eeprom
 *
 */
/*______________________________________________________________________________________*/

/*
void Get_PERST_MODE_from_EEP(uint16_t SYS_PERST_MODE_CFG_EEP_ADD, uint8_t* p_SYS_PERST_MODE_CFG_Data, uint8_t count_data) {
	ret=Read_EEPROM(MEM_24LC512_ADDR, SYS_PERST_MODE_CFG_EEP_ADD, p_SYS_PERST_MODE_CFG_Data, count_data);
	if (ret != HAL_OK) {
		//UART_Print(huart, V_I2C_WRITE_ERR);		=> to be changed by Write I2C CAN Reg
	}
}
*/


