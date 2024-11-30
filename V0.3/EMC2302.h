/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  EMC2302.h
*************************************************************************************************************/
//  Description  : EMC2302 FanS Controller
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 29/04/2024: creation

*************************************************************************************************************/


/* REGISTERS */
#define EMC2302_CONFIG					0x20
#define EMC2302_FAN_STATUS				0x24
#define EMC2302_FAN_STALL_STATUS		0x25
#define EMC2302_FAN_SPIN_STATUS			0x26
#define EMC2302_DRIVE_FAIL_STATUS		0x27
#define EMC2302_FAN_INT_EN				0x29			
#define EMC2302_PWM_POL_CONFIG			0x2A
#define EMC2302_PWM_OUT_CONFIG			0x2B
#define EMC2302_PWM_BASEF123			0x2D

//FAN1	
#define EMC2302_FAN1_SETTING			0x30
#define EMC2302_PWM1_DIVIDE				0x31
#define EMC2302_FAN1_CONFIG1			0x32
#define EMC2302_FAN1_CONFIG2			0x33
#define EMC2302_GAIN1					0x35
#define EMC2302_FAN1_SPINUP_CONFIG		0x36
#define EMC2302_FAN1_MAX_STEP			0x37
#define EMC2302_FAN1_MIN_DRIVE			0x38
#define EMC2302_FAN1_VALID_TACH_COUNT	0x39
#define EMC2302_FAN1_DRIVE_FAIL_BLB		0x3A
#define EMC2302_FAN1_DRIVE_FAIL_BHB		0x3B
#define EMC2302_TACH1_TARGET_LB			0x3C
#define EMC2302_TACH1_TARGET_HB			0x3D
#define EMC2302_TACH1_READ_HB			0x3E
#define EMC2302_TACH1_READ_LB			0x3F

//FAN2
#define EMC2302_FAN2_SETTING			0x40	
#define EMC2302_PWM2_DIVIDE				0x41
#define EMC2302_FAN2_CONFIG1			0x42
#define EMC2302_FAN2_CONFIG2			0x43
#define EMC2302_GAIN2					0x45
#define EMC2302_FAN2_SPINUP_CONFIG		0x46
#define EMC2302_FAN2_MAX_STEP			0x47
#define EMC2302_FAN2_MIN_DRIVE			0x48
#define EMC2302_FAN2_VALID_TACH_COUNT	0x49
#define EMC2302_FAN2_DRIVE_FAIL_BLB		0x4A
#define EMC2302_FAN2_DRIVE_FAIL_BHB		0x4B
#define EMC2302_TACH2_TARGET_LB			0x4C
#define EMC2302_TACH2_TARGET_HB			0x4D
#define EMC2302_TACH2_READ_HB			0x4E
#define EMC2302_TACH2_READ_LB			0x4F



#define EMC2302_PRODUCT_ID 				0xFD
#define EMC2302_MANUFACT_ID 			0xFE
#define EMC2302_REV 					0xFF


// Bits in the CONFIG register
#define	CONF_USECK		0x01
#define	CONF_DRECK		0x02
#define	CONF_WD_EN		0x20
#define	CONF_DIS_TO		0x40
#define	CONF_MASK		0x80

// Bits in the FAN INTERRUPT ENABLE register
#define	FxITOFF			0x00
#define	F1ITEN			0x01
#define	F2ITEN			0x02


// Bits in the PWM POLARITY CONFIG register
#define	PLRITY1			0x01
#define	PLRITY2			0x02

// Bits in the PWM OUTPUT CONFIG register
#define PMOT1			0x01
#define PMOT2			0x02


// FAN definition
#define FAN1			0x01
#define FAN2			0x02


//Misc
#define RPM_FACTOR		3932160
#define RPM_MULT_X1		1
#define RPM_MULT_X2		2


//Functions
#ifdef __cplusplus
extern "C" {
#endif

void EMC2302_SetFanPWM(int fd, uint8_t Fan, int pwm_data);
int EMC2302_GetFanPWM(int fd, uint8_t Fan);
uint16_t EMC2302_GetFanRPM(int fd, uint8_t Fan, uint8_t multiplier);


void EMC2302_SetFanConfig1(int fd, uint8_t Fan, int data);
void EMC2302_SetFanFreqDivide(int fd, uint8_t Fan, int data);
int EMC2302_Setup (int fd);

uint8_t EMC2302_GetProductID(int fd);
uint8_t EMC2302_GetManufactID(int fd);
uint8_t EMC2302_GetRevision(int fd);

void EMC2302_ClearError(int fd);
void EMC2302_SetIntEnable(int fd, int data);

/*
uint8_t crc8(uint8_t *data, uint16_t length);
uint32_t SHT4x_GetSerialNumber(int fd);
void SHT4x_GetTemp_Humidity(int fd, uint8_t CMD, float* Temp, float* RH);
void SHT4x_GetTemp_Humidity_HPM(int fd, float* Temp, float* RH);
void SHT4x_GetTemp_Humidity_MPM(int fd, float* Temp, float* RH);
void SHT4x_GetTemp_Humidity_LPM(int fd, float* Temp, float* RH);
*/

#ifdef __cplusplus
}
#endif

