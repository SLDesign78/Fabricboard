/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  fabricboard.h
*************************************************************************************************************/
//  Description  : fabricboard header
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 20/01/2023: creation

*************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FABRICBOARD_H
#define __FABRICBOARD_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <gpiod.h>


#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <asm/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>



//User Headers
#include "Define.h"
#include "MCP23017.h"
#include "PCA9545A.h"
#include "SHT4x.h"
#include "EMC2302.h"
#include "ISL28022.h"
#include "SLD_PiI2C.h"
#include "SLD_GPIO.h"
#include "SLD_MISC_FUNC.h"
#include "SLD_SERVER.h"
#include "SLD_POWER_REFCLK_RESET.h"
#include "SLD_SLOT.h"
#include "SLD_PEX.h"


/****************************************************
   		USER DECLARATIONS
****************************************************/


/*------------------------- SLOT CFG STRUCT ---------------------------*/
/*
Num				: Slot Number [1-7]
Type            : 0:Host, 1:Downstream
Reset_dir       : 0:input, 1:output
Pcie_refclk_en  : 0:disabled, 1:enabled
Prsnt_Status    : 0:Card no present, 1:Card present
*/

typedef struct __Slot_CfgTypeDef
{
	uint8_t Num;
	uint8_t Type;
	uint8_t Reset_dir;
	uint8_t Pcie_refclk_en;
	uint8_t Prsnt_Status;

} Slot_CfgTypeDef;



//Slots cfg structure declaration
extern Slot_CfgTypeDef Slot1_Cfg;
extern Slot_CfgTypeDef Slot2_Cfg;
extern Slot_CfgTypeDef Slot3_Cfg;
extern Slot_CfgTypeDef Slot4_Cfg;
extern Slot_CfgTypeDef Slot5_Cfg;
extern Slot_CfgTypeDef Slot6_Cfg;
extern Slot_CfgTypeDef Slot7_Cfg;


/*------------------------- SWITCH CFG STRUCT ---------------------------*/
/*
Host_Port	: Host Port number
*/


typedef struct __Switch_CfgDef
{
	uint8_t Host_Port;

} Switch_CfgDef;


//Switch config structure declaration
extern Switch_CfgDef Sw_Cfg;


//------------ I2C File Description ------------
extern int fd_MCP23017_EXP0_i2c;
extern int fd_MCP23017_EXP1_i2c;
extern int fd_PCA9545A_MUX4_i2c;
extern int fd_PCA9548A_MUX8_i2c;
extern int fd_SHT41_i2c;
extern int fd_EMC2302_i2c;
extern int fd_PEX_i2c;
extern int fd_ISL28022_i2c;

//----- I2C Device -----
extern char* i2cDevice;


/*------------------------------REGISTERS----------------------------------*/

//FW Version
extern const uint8_t Version_Major;
extern const uint8_t Version_Minors;

//SYSTEM REG
extern volatile uint8_t SYS_STATUS;
extern volatile uint8_t PWR_STATUS;
extern volatile uint8_t REFCLK_STATUS;


//FSM_PWR_CTRL_REG
extern volatile uint8_t PWR_EN_REG;
extern volatile uint8_t PWR_PG_REG;

//FSM_PWR
extern volatile uint8_t FSM_PWR_CTRL_EN;
extern volatile uint8_t FSM_PWR_CTRL_OK;
extern volatile uint8_t FSM_PWR_CURR;
extern volatile uint8_t FSM_PWR_ERR;
extern volatile uint8_t FSM_PWR_STATUS;

//ALERT REG
extern volatile uint8_t FAN_ALERT_REG;
extern volatile uint8_t TEMP_ALERT_REG;
extern volatile uint8_t MSC_ALERT_ERR_REG;

//SLOT TYPE
extern volatile uint8_t SLOT_TYPE_REG;

//PRESENCE REG
extern volatile uint8_t SLOT_PRSNT;
extern volatile uint8_t MSC_PRSNT;

//CLOCK CFG
extern volatile uint8_t SLOT_CLK_CFG;
extern volatile uint8_t MSC_CLK_CFG;

//PCIe RESET CTRL/DIR/CFG
extern volatile uint8_t SYS_PERST_MODE_CTRL;
extern volatile uint8_t SYS_PERST_CTRL;
extern volatile uint8_t SLOT_PERST_DIR;
extern volatile uint8_t SLOT_PERST_STATUS;
extern volatile uint8_t MSC_PERST_STATUS;


//Board TEMPERATURE SENSORS
extern volatile uint16_t BRD_TEMP_AMB;
extern volatile uint8_t BRD_TEMP_CR_REG;
extern volatile uint8_t BRD_TEMP_UP_REG;
extern volatile uint8_t BRD_TEMP_LO_REG;

//PEX TEMPERATURE
extern volatile uint16_t PEX_TEMP;

//PEX FIRMWARE REVISION
extern volatile uint16_t PEX_FW_REV;

//VR TEMPERATURE REG
extern volatile uint16_t VR_TEMP_CHA;
extern volatile uint16_t VR_TEMP_CHB;

//VR U/I/P MEASURES REG
extern volatile uint16_t VR_VIN_CHA;
extern volatile uint16_t VR_VIN_CHB;
extern volatile uint16_t VR_VOUT_CHA;
extern volatile uint16_t VR_VOUT_CHB;
extern volatile uint16_t VR_IOUT_CHA;
extern volatile uint16_t VR_IOUT_CHB;
extern volatile uint16_t VR_POUT_CHA;
extern volatile uint16_t VR_POUT_CHB;

//VR STATUS REG
extern volatile uint16_t VR_STATUS_WORD_CHA;
extern volatile uint16_t VR_STATUS_WORD_CHB;
extern volatile uint8_t VR_STATUS_VOUT_CHA;
extern volatile uint8_t VR_STATUS_VOUT_CHB;
extern volatile uint8_t VR_STATUS_IOUT_CHA;
extern volatile uint8_t VR_STATUS_IOUT_CHB;
extern volatile uint8_t VR_STATUS_TEMP_CHA;
extern volatile uint8_t VR_STATUS_TEMP_CHB;

//ISL28022 U/I/P MEASURES REG
extern volatile uint8_t ISL_V_MEAS_REG1;
extern volatile uint8_t ISL_V_MEAS_REG2;
extern volatile uint8_t ISL_I_MEAS_REG1;
extern volatile uint8_t ISL_I_MEAS_REG2;
extern volatile uint8_t ISL_P_MEAS_REG1;
extern volatile uint8_t ISL_P_MEAS_REG2;




/*------------------------POWER CONTROL----------------------------------*/

//Power Control
extern volatile uint8_t SYS_PWR_EN, SYS_PWR_OK;

//Power Supply Control
extern volatile uint8_t SYS_P3V3_EN, SYS_P0V9_EN, SYS_P1V8_EN;

//Emergency Power Off
extern volatile uint8_t SYS_PWR_EPO;

/*Enumerated type for Power State Machine states*/
enum PWR_STATE {
							ST_INIT, ST_UP_P3V3, ST_UP_P0V9, 
							ST_UP_P1V8, ST_PWR_OK,
							ST_DWN_P1V8, ST_DWN_P0V9, ST_DWN_P3V3,
							ST_PWR_FAULT, ST_PWR_OFF
						 };

/*State machine initial state*/
extern volatile uint8_t ucPWR_STATE;

/*Timeout*/
extern volatile uint32_t FSM_VR_Timeout;


/*--------------------------PCIe Reference Clock CONTROL----------------------------------*/

//Refclock Control
extern volatile uint8_t REFCLK_ON;

/*Enumerated type for Power State Machine states*/
enum REFCLK_STATE {
						ST_REFCLK_OFF, ST_REFCLK_ON

						 };

/*State machine initial state*/
extern volatile uint8_t ucREFCLK_STATE;



/*------------------------PCIe RESET CONTROL----------------------------------*/

//System PCIe Reset for all slots an devices
/* 0x00: Asserted
 * 0x01: Deasserted
*/
extern volatile uint8_t SYS_PERST;

//System Reset Mode Control
extern volatile uint8_t SYS_PERST_MODE;

//Reset Control
extern volatile uint8_t RST_RELEASE;

/*Enumerated type for Reset State Machine states*/
enum RST_STATE {
						ST_RST_INIT, ST_RST_PWROFF,
						ST_RST_ASSERTED, ST_RST_DEASSERTED

 			   	   };

/*State machine initial state*/
extern volatile uint8_t ucRST_STATE;



/*------------------------SLOT TYPE CONFIG----------------------------------*/
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

extern volatile uint8_t SLOT_TYPE_CFG[7];



/*------- SLOT Type config file -------*/
extern char* File_slot_type;



//System
extern char CMD_USER[BUFF_L_SIZE];
extern volatile int EXIT;


//Socket UNIX: Server
extern int server_fd;
extern int client_fd;
extern struct sockaddr_un server_addr;
extern struct sockaddr_un client_addr;



//Interrupt I2C EXP
extern uint8_t Reg_INTFA;	//PortA
extern uint8_t Reg_INTCAPA; //PortA
extern uint8_t Reg_INTFB;	//PortB
extern uint8_t Reg_INTCAPB; //PortB


//I2C Device
extern char* i2cDevice;
extern char* i2cDevice_test;



//Sensors
extern float temperature;
extern float humidity;



// ***** User socket commands *****
extern const char* p3v3_on_CMD; 	
extern const char* p3v3_off_CMD; 	
extern const char* p1v8_on_CMD; 	
extern const char* p1v8_off_CMD; 	
extern const char* p0v9_on_CMD; 	
extern const char* p0v9_off_CMD; 	

extern const char* pon_CMD; 
extern const char* poff_CMD;
extern const char* pstatus_CMD; 
extern const char* sys_status_CMD;
extern const char* get_slot_cfg_CMD;
extern const char* set_slot_cfg_CMD;

extern const char* get_fan1_CMD;
extern const char* get_fan2_CMD;
extern const char* set_fan1_CMD;
extern const char* set_fan2_CMD;
extern const char* get_power_CMD; 
extern const char* get_pex_CMD; 
extern const char* get_temp_rh_CMD;

extern const char* MUX4_CH0_CMD;
extern const char* MUX4_CH1_CMD;
extern const char* MUX4_CH2_CMD;
extern const char* MUX4_ALL_CMD;

extern const char* gpio_ext_all_off_CMD;

extern const char* set_slot_CMD;
extern const char* test_CMD;
extern const char* get_fw_vers_CMD; 
extern const char* exit_CMD; 
extern const char* help_CMD;





//***** CONSTANT VERBOSE *****

//COMMAND ERROR
extern const char* V_UnKwn_cmd;

//POWER CONTROL
extern const char* V_PWR_ON;
extern const char* V_PWR_OFF;
extern const char* V_PWR_EPO;
extern const char* V_PWR_OK;

extern const char* V_P3V3_OK;
extern const char* V_P0V9_OK;
extern const char* V_P1V8_OK;
 
extern const char* V_P3V3_ON;
extern const char* V_P0V9_ON;
extern const char* V_P1V8_ON;
 
extern const char* V_P3V3_OFF;
extern const char* V_P0V9_OFF;
extern const char* V_P1V8_OFF;

extern const char* V_P3V3_PG_KO;
extern const char* V_P0V9_PG_KO;
extern const char* V_P1V8_PG_KO;

//REFCLOCK CONTROL
extern const char* V_REFCLK_ON;
extern const char* V_REFCLK_OFF;

//SLOT TYPE CONFIG
extern const char* V_SLOT1;
extern const char* V_SLOT2;
extern const char* V_SLOT3;
extern const char* V_SLOT4;
extern const char* V_SLOT5;
extern const char* V_SLOT6;
extern const char* V_SLOT7;
 
extern const char* V_SLOT_TYPE_HOST;
extern const char* V_SLOT_TYPE_DWN;
extern const char* V_SLOT_NOCFG_FILE;

extern const char* V_CARD_PRESENCE;
extern const char* V_CARD_NOPRESENCE;

extern const char* V_SLOT_TYPE_NO_CMPLT;
extern const char* V_SLOT_TYPE_CONFIG;
extern const char* V_SLOT_TYPE_NO_CONFIG;
 
//HOST/SLOT PERST
extern const char* V_HOST_PERST_ASSERT;
extern const char* V_HOST_PERST_DEASSERT;
extern const char* V_SLOTx_PERST_ASSERT;
extern const char* V_SLOTx_PERST_DEASSERT;

//SWICTH MODE
extern const char* V_SW_FB_LOCAL_MODE;
extern const char* V_SW_FB_REMOTE_MODE;

//system PCIe Reset Mode
extern const char* V_SW_SYS_RST_MODE_AUTO;
extern const char* V_SW_SYS_RST_MODE_MANUAL;

//ALERT/FAULT/ERROR
extern const char* V_I2C_TEMP_ALRT;
extern const char* V_VR_HOT_ALRT;
extern const char* V_VR_I2C_ALRT;
extern const char* V_VR_FLT;
extern const char* V_PEX_SYS_ERR;
extern const char* V_P12V_PWR_FLT;




#endif  //__MAIN_H
