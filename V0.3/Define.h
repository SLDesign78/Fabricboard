/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  Define.h
*************************************************************************************************************/
//  Description  : Define file
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 20/01/2023: creation

*************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEFINE_H
#define __DEFINE_H


//*** FIRMWARE VERSION ***
#define VERSION_MAJOR	0
#define VERSION_MINOR	3


//SERIAL
#define BAUDS 115200

//MEMORY define for string messages => SERVER SOCKET
#define BUFF_H_SIZE 1024
#define BUFF_M_SIZE 512
#define BUFF_L_SIZE 128


#define LOW  0
#define HIGH 1

#define TRUE   1
#define FALSE  0


//I2C MCP23017 DIR
#define OUTPUT	0
#define INPUT	1

//*** SLOT CONFIG ***
/* 0x00: HOST
 * 0x01: DOWNSTREAM (End Point)
 */
#define SLOT_TYPE_HOST			0
#define SLOT_TYPE_ENDPOINT		1

//PEX ACCESS ERROR
#define PEX_ERROR	0xFFFFFFFF


/*--------------------------SWITCH CONFIG----------------------------------*/
/* 0x00: LOCAL_SW_MODE (at least one HOST is present)
 * 0x01: REMOTE_SW_MODE (No Host port)
 */
#define LOCAL_SW_MODE		0
#define REMOTE_SW_MODE		1

#define HOST_SLOT1			0
#define HOST_SLOT2			1
#define HOST_SLOT3			2
#define HOST_SLOT4			3
#define HOST_SLOT5			4
#define HOST_SLOT6			5
#define HOST_SLOT7			6

//*** Power Control ***
#define VR_PG_TIMEOUT	0x400


//******************* GPIOD / WiringPI LIB ********************

//-------- CHIP1 --------
#define GPIO_G0_EXT2		21  	//GPIOH_4:  gpio number 431, Line 21 / wiringPi pin 2												 
#define GPIO_G1_EXT7		22  	//GPIOH_5:  gpio number 432, Line 22 / wiringPi pin 7
#define P3V3_I2C_ALT_N		23		//GPIOH_6:  gpio number 433, Line 23
#define P12V_PWR_ISL_INT	24		//GPIOH_7:  gpio number 434, Line 24
#define FAN_ALERT_N			25		//GPIOH_8:  gpio number 435, Line 25
															    
#define GPIO_G1_EXT4		50  	//GPIOA_0:  gpio number 460, Line 50 / wiringPi pin 4
#define GPIO_G0_EXT1		51  	//GPIOA_1:  gpio number 461, Line 51 / wiringPi pin 1

#define GPIO_G1_EXT5  		52		//GPIOA_2:  gpio number 462, Line 52 / wiringPi pin 5
//#define INT_MCP23017  		52

#define GPIO_G2_EXT8 		53  	//GPIOA_3:  gpio number 463, Line 53 / wiringPi pin 11

#define IOEXP0_INT			54  	//GPIOA_4:  gpio number 464, Line 54 
#define MUX8_RESET_N		55		//GPIOA_5:  gpio number 465, Line 55 
#define MUX4_RESET_N		56		//GPIOA_6:  gpio number 466, Line 56 

#define GPIO_G1_EXT6 		57  	//GPIOA_7:  gpio number 467, Line 57 / wiringPi pin 6
		
#define SEL_SPI_MUX 		59		//GPIOA_9:  gpio number 469, Line 59
#define PEX_SYS_ERR_N		60		//GPIOA_10: gpio number 470, Line 60
#define IOEXP0_RESET_N  	61		//GPIOA_11: gpio number 471, Line 61
#define IOEXP1_RESET_N  	62		//GPIOA_12: gpio number 472, Line 62
#define IOEXP1_INT			63		//GPIOA_13: gpio number 473, Line 63
#define VR_P0V9_I2C_ALT_N	64  	//GPIOA_14: gpio number 474, Line 64
#define VR_P1V8_I2C_ALT_N	65 		//GPIOA_15: gpio number 475, Line 65

									//GPIOX_6:  gpio number 482, Line 72 / wiringPi pin 15	(UART_TXD)
									//GPIOX_7:  gpio number 483, Line 73 / wiringPi pin 16	(UART_RXD)
									//GPIOX_8:  gpio number 484, Line 74 / wiringPi pin 12	(SPI_MOSI)
									//GPIOX_9:  gpio number 485, Line 75 / wiringPi pin 13	(SPI_MISO)
									//GPIOX_10: gpio number 486, Line 76 / wiringPi pin 10	(SPI_CS)
									//GPIOX_11: gpio number 487, Line 77 / wiringPi pin 14	(SPI SCLK)
									
									//GPIOX_17: gpio number 486, Line 76 / wiringPi pin 8	(I2C_SDA)	
									//GPIOX_18: gpio number 486, Line 76 / wiringPi pin 9	(I2C_SCL)

//-------- CHIP0 --------
#define GPIO_G0_EXT3  		5		//GPIOA0_5:  gpio number 501, Line 5  / wiringPi pin 3 
#define GPIO_G0_EXT0		10  	//GPIOA0_10: gpio number 506, Line 10 / wiringPi pin 0
#define SPI_CM4_RST_N  		11		//GPIOA0_11: gpio number 507, Line 11 


#define	CONSUMER	"Consumer"
#define CHIP0  		"gpiochip0"
#define CHIP1  		"gpiochip1"



// **** I2C ***
#define MCP23017_EXP0_AD 	0x20 // 7-bit address All Add pin low (GND) IO EXPANDER#0 MCP23017
#define MCP23017_EXP1_AD 	0x21 // 7-bit address All Add pin low (GND) IO EXPANDER#1 MCP23017
#define PCA9545A_AD			0x70 // 7-bit address All Add pin low (GND) I2C MUX4
#define PCA9548A_AD			0x71 // 7-bit address A0 pin high I2C MUX8
#define SHT41_AD 			0x44 // 7-bit address All Add pin low (GND) Temperature & Humidity Sensor
#define EMC2302_AD			0x2E // Fans Controller
#define PEX88096_AD			0x59 // PCIE Switch Broadcom PEX88096			

#define ISL28022_AD    		0x40 // 7-bit address All Add pin low (GND), default for InvenSense evaluation board
#define ISL28022_BDCST_AD 	0x3F // Broadcast address



//I2C EXPANDERS PINS
#define EXP0	0
#define EXP1	1

//--------  IO EXPANDER #0 --------  
//Channel A
#define 	PEX_PERST_N			0       //GPA0
#define 	SLOT7_PERST_N		1       //GPA1
#define 	SLOT6_PERST_N		2       //GPA2
#define 	SLOT5_PERST_N		3       //GPA3
#define 	SLOT4_PERST_N		4       //GPA4
#define 	SLOT3_PERST_N		5       //GPA5
#define 	SLOT2_PERST_N		6       //GPA6
#define 	SLOT1_PERST_N		7       //GPA7
    
//Channel B							
#define 	REFCLK_OE7			8       //GPB0
#define 	REFCLK_OE6			9       //GPB1
#define 	REFCLK_OE5			10      //GPB2
#define 	REFCLK_OE4			11      //GPB3
#define 	REFCLK_OE3			12      //GPB4
#define 	REFCLK_OE2			13      //GPB5
#define 	REFCLK_OE1			14      //GPB6
#define 	REFCLK_OE0			15      //GPB7

//--------  IO EXPANDER #1 -------- 
//Channel A
#define 	PCIE_CM4_PRSNT_N	0       //GPA0
#define 	SLOT7_PRSNT_N		1       //GPA1
#define 	SLOT6_PRSNT_N		2       //GPA2
#define 	SLOT5_PRSNT_N		3       //GPA3
#define 	SLOT4_PRSNT_N		4       //GPA4
#define 	SLOT3_PRSNT_N		5       //GPA5
#define 	SLOT2_PRSNT_N		6       //GPA6
#define 	SLOT1_PRSNT_N		7       //GPA7

//Channel B							
#define 	SEL_SW_UART_CM4		8       //GPB0
#define 	EEP_WP_EN			9       //GPB1
#define 	P3V3_PG				10      //GPB2
#define 	VR_P1V8_PG			11      //GPB3
#define 	VR_P0V9_PG			12      //GPB4
#define 	P3V3_EN				13      //GPB5
#define 	VR_P1V8_EN			14      //GPB6
#define 	VR_P0V9_EN			15      //GPB7



//************************************************
// Power Telemetry ISL28022
//************************************************

#define RSHUNT				0.001
#define VSHUNT_LSB			0.01
#define VBUS_LSB			0.004
#define CURRENT_LSB			0.00122
#define POWER_LSB			4.883E-06

#define SIZE_BYTE			1
#define SIZE_WORD			2

//******** AVERAGE CALCULATION ********
#define MOVAVG_SIZE 60



// bits Registers define
#define SYS_STATUS__SYS_OK_BIT			0
#define SYS_STATUS__SYS_FAIL_BIT		1

#define PWR_STATUS__PWR_OK_BIT			0
#define PWR_STATUS__PWR_FAIL_BIT		1

#define SYS_REFCLK__REFCLK_ON_BIT		0

#define SW_MODE__LOCAL_BIT				0
#define SW_MODE__REMOTE_BIT				1

#define PWR_EN_REG__P3V3_EN_BIT			0
#define PWR_EN_REG__P5V_VR_EN_BIT		1
#define PWR_EN_REG__VR_P0V9_EN_BIT		2
#define PWR_EN_REG__VR_P1V8_EN_BIT		3

#define PWR_PG_REG__P3V3_PG_BIT			0
#define PWR_PG_REG__P5V_VR_PG_BIT		1
#define PWR_PG_REG__VR_P0V9_PG_BIT		2
#define PWR_PG_REG__VR_P1V8_PG_BIT		3

#define FSM_PWR_CTRL__PWR_EN_BIT		0
#define FSM_PWR_CTRL__EPO_BIT			1

#define FSM_PWR_STATUS__PWR_OK_BIT		0
#define FSM_PWR_STATUS__PWR_FAULT_BIT	1

#define TEMP_ALERT_REG__VR_HOT_BIT		0
#define TEMP_ALERT_REG__I2C_ALERT_BIT	1

#define SLT_TYP_CFG__SLOT1_TYPE_BIT	0
#define SLT_TYP_CFG__SLOT2_TYPE_BIT	1
#define SLT_TYP_CFG__SLOT3_TYPE_BIT	2
#define SLT_TYP_CFG__SLOT4_TYPE_BIT	3
#define SLT_TYP_CFG__SLOT5_TYPE_BIT	4
#define SLT_TYP_CFG__SLOT6_TYPE_BIT	5
#define SLT_TYP_CFG__SLOT7_TYPE_BIT	6


#define SLT_PRSNT__SLOT1_PRSNT_BIT	0
#define SLT_PRSNT__SLOT2_PRSNT_BIT	1
#define SLT_PRSNT__SLOT3_PRSNT_BIT	2
#define SLT_PRSNT__SLOT4_PRSNT_BIT	3
#define SLT_PRSNT__SLOT5_PRSNT_BIT	4
#define SLT_PRSNT__SLOT6_PRSNT_BIT	5
#define SLT_PRSNT__SLOT7_PRSNT_BIT	6

#define SLT_CLK_CFG__SLOT1_CLK_EN_BIT	0
#define SLT_CLK_CFG__SLOT2_CLK_EN_BIT	1
#define SLT_CLK_CFG__SLOT3_CLK_EN_BIT	2
#define SLT_CLK_CFG__SLOT4_CLK_EN_BIT	3
#define SLT_CLK_CFG__SLOT5_CLK_EN_BIT	4
#define SLT_CLK_CFG__SLOT6_CLK_EN_BIT	5
#define SLT_CLK_CFG__SLOT7_CLK_EN_BIT	6

#define MSC_CLK_CFG__PEX_CLK_EN_BIT		0


#define SLT_RST_DIR__SLOT1_PERST_DIR_BIT	0
#define SLT_RST_DIR__SLOT2_PERST_DIR_BIT	1
#define SLT_RST_DIR__SLOT3_PERST_DIR_BIT	2
#define SLT_RST_DIR__SLOT4_PERST_DIR_BIT	3
#define SLT_RST_DIR__SLOT5_PERST_DIR_BIT	4
#define SLT_RST_DIR__SLOT6_PERST_DIR_BIT	5
#define SLT_RST_DIR__SLOT7_PERST_DIR_BIT	6

#define SYS_PERST_CTRL__SYS_PERST_MODE_BIT 	0
#define SYS_PERST_CTRL__SYS_PERST_BIT 		0

#define SLT_RST_STATUS__SLOT1_PERST_N_BIT	0
#define SLT_RST_STATUS__SLOT2_PERST_N_BIT	1
#define SLT_RST_STATUS__SLOT3_PERST_N_BIT	2
#define SLT_RST_STATUS__SLOT4_PERST_N_BIT	3
#define SLT_RST_STATUS__SLOT5_PERST_N_BIT	4
#define SLT_RST_STATUS__SLOT6_PERST_N_BIT	5
#define SLT_RST_STATUS__SLOT7_PERST_N_BIT	6

#define MSC_RST_STATUS__PEX_PERST_N_BIT	0


//******** SOCKET UNINX: SERVER ********
#define INFO_SERVER_PATH "/var/unix_socket"



//******************** MACRO ***********************
#define SetBit(a,b)			a |= (1 << b) 	//Set Bit
#define ClrBit(a,b) 		a &= ~(1 << b) 	//Clear Bit

#define GetBit(a,b,c)		a = (b >> c) & 0x01
#define Get2Bit(a,b,c)		a = (b >> c) & 0x03



#endif