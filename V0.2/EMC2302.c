/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  EMC2302.c
*************************************************************************************************************/
//  Description  : EMC2302 FANS COntroller
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 29/04/2024: creation

*************************************************************************************************************/
/*
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Define.h"
#include "SLD_PiI2C.h"
#include "SLD_MISC_FUNC.h"
#include "EMC2302.h"
*/


#include "fabricboard.h"



/*
 * Set FAN CONFIG 1 ():
 *********************************************************************************
*/
void EMC2302_SetFanConfig1(int fd, uint8_t Fan, int data) {
	
	switch(Fan)	{
		case 1:	//FAN 1
			SLD_PiI2CWriteRegByte (fd, EMC2302_FAN1_CONFIG1, data);
		break;
		
		case 2:	//FAN2
			SLD_PiI2CWriteRegByte (fd, EMC2302_FAN2_CONFIG1, data);
		break;
	}
}



/*
 * Get FANx RPM:
 *********************************************************************************
*/
uint16_t EMC2302_GetFanRPM(int fd, uint8_t Fan, uint8_t multiplier) {

	uint8_t Data_byte[2];

	switch(Fan)	{
		case 1:	//FAN 1
			SLD_PiI2CReadBlock (fd, EMC2302_TACH1_READ_HB, Data_byte, 2);
		break;

		case 2:	//FAN 2
			SLD_PiI2CReadBlock (fd, EMC2302_TACH2_READ_HB, Data_byte, 2);
		break;
	}
	
	return ((RPM_FACTOR*multiplier)/(((Data_byte[0] << 8) | Data_byte[1]) >> 3));

}





/*
 * Set FANx PWM ():
 *********************************************************************************
	FxSP]7:0]: Fan Speed Setting, where “x” represents PWM1 through 5 based on device features. 
			   PWM duty cycle is calculated => Drive = (DATA/255) * 100%
*/
void EMC2302_SetFanPWM(int fd, uint8_t Fan, int pwm_data) {
	
	int data = (255 * pwm_data)/100;
	
	switch(Fan)	{
		case 1:	//FAN 1
			SLD_PiI2CWriteRegByte (fd, EMC2302_FAN1_SETTING, data);
		break;
		
		case 2:	//FAN2
			SLD_PiI2CWriteRegByte (fd, EMC2302_FAN2_SETTING, data);
		break;
	}
}





/*
 * Get FANx PWM ():
 *********************************************************************************
*/
int EMC2302_GetFanPWM(int fd, uint8_t Fan) {
	
	int data;
	
	switch(Fan)	{
		case 1:	//FAN 1
			data=SLD_PiI2CReadRegByte (fd, EMC2302_FAN1_SETTING);
		break;
		
		case 2:	//FAN2
			data=SLD_PiI2CReadRegByte (fd, EMC2302_FAN2_SETTING);
		break;
	}

	return ((int)round(((float)data*100.0)/255.0));
}



/*
 * Set FANx PWMx Freq Divide ():
 *********************************************************************************
*/
void EMC2302_SetFanFreqDivide(int fd, uint8_t Fan, int data) {

	switch(Fan)	{
		case 1:	//FAN 1
			SLD_PiI2CWriteRegByte (fd, EMC2302_PWM1_DIVIDE, data);
		break;
		
		case 2:	//FAN2
			SLD_PiI2CWriteRegByte (fd, EMC2302_PWM2_DIVIDE, data);
		break;
	}
}






/*
 * Get Manufacturer ID():
 *********************************************************************************
 */
uint8_t EMC2302_GetManufactID(int fd) {
	return (uint8_t)(SLD_PiI2CReadRegByte (fd, EMC2302_MANUFACT_ID));
}


/*
 * Get Product ID():
 *********************************************************************************
 */
uint8_t EMC2302_GetProductID(int fd) {
	return (uint8_t)(SLD_PiI2CReadRegByte (fd, EMC2302_PRODUCT_ID));
}


/*
 * Get Revision():
 *********************************************************************************
 */
uint8_t EMC2302_GetRevision(int fd) {
	return (uint8_t)(SLD_PiI2CReadRegByte (fd, EMC2302_REV));
}



/*
 * Clear Error():
 *********************************************************************************
 */
void EMC2302_ClearError(int fd) {
	SLD_PiI2CReadRegByte (fd, EMC2302_FAN_STATUS);
	SLD_PiI2CReadRegByte (fd, EMC2302_FAN_SPIN_STATUS);
	SLD_PiI2CReadRegByte (fd, EMC2302_FAN_STALL_STATUS);
	SLD_PiI2CReadRegByte (fd, EMC2302_DRIVE_FAIL_STATUS);
}



/*
 * Set Interrupt Enable():
 *********************************************************************************
 */
void EMC2302_SetIntEnable(int fd, int data) {
	SLD_PiI2CWriteRegByte (fd, EMC2302_FAN_INT_EN, data);
}


/*
 * EMC2302_Setup:
 *********************************************************************************
 */

int EMC2302_Setup (int fd) {
	
	int wr_data=0x00; /*, read_data;*/
	
	
	//-------------------------------------------------
	// *** CONFIG: CONFIGURATION REGISTER ***
	/*
	MASK: Blocks the ALERT pin from being asserted
		1 = The ALERT pin is masked and will not be asserted.
		0 = The ALERT pin is unmasked. If any bit in the status registers is set, the ALERT pin will be asserted
			(unless individually masked via the Fan Interrupt Enable Register).
	DIS_TO: Disables the SMBus Time-Out function for the SMBus client (if enabled)
		1 = The SMBus Time-Out function is disabled, allowing the device to be fully I2C compatible.
		0 = The SMBus Time-Out function is enabled.
	WD_EN: Enables the Watchdog Timer (see Section 4.11 “Watchdog Timer”) to operate in	Continuous Mode
		1 = The Watchdog Timer operates continuously.
		0 = The Watchdog Timer does not operate continuously. It will function upon power-up and at no other time.
	DRECK: Enables the internal tachometer clock to be driven out on the CLK pin so that multiple devices
			can be synced to the same source
		1 = The CLK pin acts as a clock output and is a push-pull driver.
		0 = The CLK pin acts as a clock input.
	USECK: Enables the EMC2301/2/3/5 to use a clock present on the CLK pin as the tachometer clock.
			If the DR_EXT_CLK bit is set, then this bit is ignored and the device will use the internal oscillator.
		1 = The EMC2301/2/3/5 will use the oscillator presented on the CLK pin for all tachometer measurements.
		0 = The EMC2301/2/3/5 will use its internal oscillator for all tachometer measurements.
	*/

	wr_data=
			//CONF_USECK	|
			//CONF_DRECK	|
			//CONF_WD_EN	|
			//CONF_MASK		|
			CONF_DIS_TO		|
			0x00;	

	SLD_PiI2CWriteRegByte (fd, EMC2302_CONFIG, wr_data);


	//----------------------------------------------------------------
	// *** FAN INTERRUPT ENABLE - FAN INTERRUPT ENABLE REGISTER ***
	/*
	F2ITEN: Fan 2 Interrupt Enable. Allows Fan 2 to assert the ALERT pin if an error condition is detected
		1 = Fan 2 has an Error condition.
		0 = Fan 2 is operating within limits.
	F1ITEN: Fan 1 Interrupt Enable. Allows Fan 1 to assert the ALERT pin if an error condition is detected
		1 = Fan 1 has an Error condition.
		0 = Fan 1 is operating within limits.
	*/
	
	wr_data=
			//F1ITEN	|
			//F2ITEN	|
			0x00;
	
	EMC2302_SetIntEnable(fd, wr_data);
	//SLD_PiI2CWriteRegByte (fd, EMC2302_FAN_INT_EN, wr_data);


	//--------------------------------------------------------------------
	// *** PWM POLARITY CONFIG - PWM POLARITY CONFIGURATION REGISTER ***
	/*
	PLRITY2: Sets the PWM output polarity
		1 = PWM 2 drive setting of 00h produces 100% duty cycle, drive setting of FFh produces 0% duty cycle.
		0 = PWM 2 drive setting of 00h produces 0% duty cycle, drive setting of FFh produces 100% duty cycle.
	PLRITY1: Sets the PWM output polarity
		1 = PWM 1 drive setting of 00h produces 100% duty cycle, drive setting of FFh produces 0% duty cycle.
		0 = PWM 1 drive setting of 00h produces 0% duty cycle, drive setting of FFh produces 100% duty cycle.
	*/

	wr_data=
			//PLRITY1	|
			//PLRITY2	|
			0x00;

	SLD_PiI2CWriteRegByte (fd, EMC2302_PWM_POL_CONFIG, wr_data);


	//----------------------------------------------------------------
	// *** PWM OUTPUT CONFIG - PWM OUTPUT CONFIGURATION REGISTER ***
	/*
	PMOT2: Sets the PWM output type
		1 = PWM 2 drive is push-pull type.
		0 = PWM 2 drive is open drain type.
	PMOT1: Sets the PWM output type
		1 = PWM 1 drive is push-pull type.
		0 = PWM 1 drive is open drain type.
	*/

	wr_data=
		//PMOT1	|
		//PMOT2	|
		0x00;

	SLD_PiI2CWriteRegByte (fd, EMC2302_PWM_OUT_CONFIG, wr_data);


	//-----------------------------------------------------------------------------
	// *** PWM BASEF123 - PWM 1, 2 AND 3 BASE FREQUENCY CONFIGURATION REGISTER ***
	/*
	PMB2[1:0]: Sets the PWM2 base frequency	(bit 3-2)
		11 = PWM 2 base is 2.441 kHz.
		10 = PWM 2 base is 4.882 kHz.
		01 = PWM 2 base is 19.53 kHz.
		00 = PWM 2 base is 26.00 kHz.
	PMB1[1:0]: Sets the PWM1 base frequency (bit 1-0)
		11 = PWM 1 base is 2.441 kHz.
		10 = PWM 1 base is 4.882 kHz.
		01 = PWM 1 base is 19.53 kHz.
		00 = PWM 1 base is 26.00 kHz.
	*/
	
	wr_data=0b00000000;

	SLD_PiI2CWriteRegByte (fd, EMC2302_PWM_OUT_CONFIG, wr_data);


	//--------------------------------------------------------
	// *** PWM X DIVIDE - PWMX FREQUENCY DIVIDE REGISTER ***
	// PWMxD]7:0]: PWM Frequency Divide value, where “x” represents PWM1 through 5 based on device features.
	
	wr_data=0x01;

	EMC2302_SetFanFreqDivide(fd, FAN1, wr_data);	//FAN1 PWM Freq Divide
	EMC2302_SetFanFreqDivide(fd, FAN2, wr_data);	//FAN1 PWM Freq Divide



	//--------------------------------------------------------
	// READ ALL STATUS Registers
	// => Clear error
	EMC2302_ClearError(fd);
	


	//-----------------------------------------------------------------------------
	// *** FAN CONFIG 1 - FAN CONFIGURATION REGISTERS ***
	/*
	bit 7	ENAGx: Enable Closed Loop algorithm, where “x” is 1 to 5, depending on device features
				1 = Closed Loop algorithm “x” is enabled. Changes to Fan Setting register are ignored.
				0 = Closed Loop algorithm “x” is disabled, and device is placed in Direct Setting mode (see
					Section 4.1 “Fan Control Modes of Operation”). Changes to the Fan Setting register will change the PWM Duty Cycle.
	bit 6-5	RNGx[1:0]: Range. Sets the minimum fan speed measured and reported, where “x” is 1 to 5, depending on device features.
				11 = 4000 RPM minimum, TACH count multiplier = 8.
				10 = 2000 RPM minimum, TACH count multiplier = 4.
				01 = 1000 RPM minimum, TACH count multiplier = 2.
				00 = 500 RPM minimum, TACH count multiplier = 1.
	bit 4-3	EDGx[1:0]: Edges. Sets the number of edges to sample when calculating RPM, where “x” is 1 to 5, depending on device features.
				11 = 9 edges sampled (4 poles) - effective Tach multiplier is 2, based on two pole fans
				10 = 7 edges sampled (3 poles) - effective Tach multiplier is 1.5, based on two pole fans
				01 = 5 edges sampled (2 poles) - effective Tach multiplier is 1, based on two pole fans
				00 = 3 edges sampled (1 pole) - effective Tach multiplier is 0.5, based on two pole fans
	bit 2-0	UDTx[2:0]: Update Time. Sets the PID update rate for closed loop control, where “x” is 1 to 5,
				depending on device features. The Update Time, along with the Fan Step register, is used to control
				the ramp rate of the drive response to provide a cleaner transition of the actual fan operation as the desired fan speed changes.
				111 = 1600 ms update interval
				110 = 1200 ms update interval
				101 = 800 ms update interval
				100 = 500 ms update interval
				011 = 400 ms update interval
				010 = 300 ms update interval
				001 = 200 ms update interval
			000 = 100 ms update interval
*/
	
	/**** FAN 1 ***
	bit7 	 0 = Closed Loop algorithm “x” is disabled
	bit6-5	01 = 1000 RPM minimum, TACH count multiplier = 2.	
	bit4-3	01 = 5 edges sampled (2 poles) - effective Tach multiplier is 1, based on two pole fans
	bit2-0 011 = 400 ms update interval
	*/
	wr_data=0b00101011;	

	EMC2302_SetFanConfig1(fd, FAN1, wr_data);	//FAN1 Config 1
	
	
	/**** FAN 2 ***
	bit7 	 0 = Closed Loop algorithm “x” is disabled
	bit6-5	00 = 500 RPM minimum, TACH count multiplier = 1.	
	bit4-3	01 = 5 edges sampled (2 poles) - effective Tach multiplier is 1, based on two pole fans
	bit2-0 011 = 400 ms update interval
	*/	
	wr_data=0b00001011;	
	
	EMC2302_SetFanConfig1(fd, FAN2, wr_data);	//FAN2 Config 1

	return LOW;	

}





