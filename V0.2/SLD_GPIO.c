/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SLD_GPIO.c
*************************************************************************************************************/
//  Description  : GPIO Interface, 
//					Simplified I2C access routines
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 17/07/2023: creation

*************************************************************************************************************/
/*
 * Notes:
 *	The Linux I2C code is actually the same (almost) as the SMBus code.
 *	SMBus is System Management Bus - and in essentially I2C with some
 *	additional functionality added, and stricter controls on the electrical
 *	specifications, etc. however I2C does work well with it and the
 *	protocols work over both.
 *
 *	I'm directly including the SMBus functions here as some Linux distros
 *	lack the correct header files, and also some header files are GPLv2
 *	rather than the LGPL that SD_I2C is released under - presumably because
 *	originally no-one expected I2C/SMBus to be used outside the kernel -
 *	however enter the Raspberry Pi with people now taking directly to I2C
 *	devices without going via the kernel...
 *
 *********************************************************************************
 */

#include "fabricboard.h"


/*********************************************************************************
 * GPIO_WR: Pin write
 *	
 *********************************************************************************
 */
  
 
 int GPIO_WR(uint16_t line_num, char *chipname, uint8_t val) {
 	
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int ret;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

	line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		perror("Get line failed\n");
		goto close_chip;
	}

	ret = gpiod_line_request_output(line, CONSUMER, 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		goto release_line;
	}

	ret = gpiod_line_set_value(line, val);
	if (ret < 0) {
		perror("Set line output failed\n");
		goto release_line;
	}

	release_line:
		gpiod_line_release(line);
	
	close_chip:
		gpiod_chip_close(chip);
	
	end:
		return 0;
}	



/*********************************************************************************
 * GPIO_RD: Pin Read
 *
 *********************************************************************************
 */

uint8_t GPIO_RD(uint16_t line_num, char *chipname) {

	struct gpiod_chip *chip;
	struct gpiod_line *line;
	uint8_t val;
	int ret;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

	line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		perror("Get line failed\n");
		goto close_chip;
	}

	ret = gpiod_line_request_input(line, CONSUMER);
	if (ret < 0) {
		perror("Request line as input failed\n");
		goto release_line;
	}

	val = gpiod_line_get_value(line);
	if (val < 0) {
		perror("Read line input failed\n");
		goto release_line;
	}

	release_line:
		gpiod_line_release(line);
	
	close_chip:
		gpiod_chip_close(chip);
	
	end:
		return val;
}


/*
 * GPIO_EXT_WR_OFF:
 * All GPIOs Ext turn OFF
 *********************************************************************************
 */

void GPIO_EXT_WR_OFF () {
	//Group0
	GPIO_WR(GPIO_G0_EXT0, CHIP0, LOW);
	GPIO_WR(GPIO_G0_EXT1, CHIP1, LOW);
	GPIO_WR(GPIO_G0_EXT2, CHIP1, LOW);
	GPIO_WR(GPIO_G0_EXT3, CHIP0, LOW);
	
	//Group1
	GPIO_WR(GPIO_G1_EXT4, CHIP1, LOW);
	GPIO_WR(GPIO_G1_EXT5, CHIP1, LOW);
	GPIO_WR(GPIO_G1_EXT6, CHIP1, LOW);
	GPIO_WR(GPIO_G1_EXT7, CHIP1, LOW);		
	
	//Group2
	GPIO_WR(GPIO_G2_EXT8, CHIP1, LOW);
	//GPIO_WR(GPIO_G2_EXT9, CHIP1, LOW);	
			
}



/*
 * GPIO_EXT_WR_ON:
 * All GPIOs Ext turn ON
 *********************************************************************************
 */

void GPIO_EXT_WR_ON () {
	//Group0
	GPIO_WR(GPIO_G0_EXT0, CHIP0, HIGH);
	GPIO_WR(GPIO_G0_EXT1, CHIP1, HIGH);
	GPIO_WR(GPIO_G0_EXT2, CHIP1, HIGH);
	GPIO_WR(GPIO_G0_EXT3, CHIP0, HIGH);
	
	//Group1
	GPIO_WR(GPIO_G1_EXT4, CHIP1, HIGH);
	GPIO_WR(GPIO_G1_EXT5, CHIP1, HIGH);
	GPIO_WR(GPIO_G1_EXT6, CHIP1, HIGH);
	GPIO_WR(GPIO_G1_EXT7, CHIP1, HIGH);		
	
	//Group2
	GPIO_WR(GPIO_G2_EXT8, CHIP1, HIGH);
	//GPIO_WR(GPIO_G2_EXT9, CHIP1, HIGH);	
}


 
/*
* GPIO_I2CEXP_WR:
*	Write I2C Expander GPIO pins
*********************************************************************************
*/
 
void GPIO_I2CEXP_WR(uint8_t EXP, int pin, int val) 
{
	if(!EXP) MCP23017_DigitalWrite(fd_MCP23017_EXP0_i2c, pin, val);
	else MCP23017_DigitalWrite(fd_MCP23017_EXP1_i2c, pin, val);
}	 

	 
	 
/*
* GPIO_I2CEXP_RD:
*	Read I2C Expander GPIO pins
*********************************************************************************
*/
 
 int GPIO_I2CEXP_RD(uint8_t EXP, int pin)
 {
	if(!EXP) return MCP23017_DigitalRead(fd_MCP23017_EXP0_i2c, pin);
	else return MCP23017_DigitalRead(fd_MCP23017_EXP1_i2c, pin);
 }
 
 
 
 /*
 * GPIO_I2CEXP_DirWR:
 *	Write I2C Expander I/O DIR pins
 *********************************************************************************
 * I/O DIRECTION
 * 0: output
 * 1: input
 */
 
 void GPIO_I2CEXP_DirWR(uint8_t EXP, int pin, int val) 
 {
	if(!EXP) MCP23017_DirWrite(fd_MCP23017_EXP0_i2c, pin, val);
	else MCP23017_DirWrite(fd_MCP23017_EXP1_i2c, pin, val);
} 



 /*
 * GPIO_I2CEXP_DirRD:
 *	Write I2C Expander I/O DIR pins
 *********************************************************************************
 * I/O DIRECTION
 * 0: output
 * 1: input
 */
 
 int GPIO_I2CEXP_DirRD(uint8_t EXP, int pin)
 {
	if(!EXP) return MCP23017_DirRead(fd_MCP23017_EXP0_i2c, pin);
	else return MCP23017_DirRead(fd_MCP23017_EXP1_i2c, pin);
 }