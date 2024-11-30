/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  MCP23017.c
*************************************************************************************************************/
//  Description  : MCP23017 Interface
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 20/01/2023: creation

*************************************************************************************************************/

#include <stdio.h>
#include <stdint.h>

#include "Define.h"
#include "SLD_PiI2C.h"
#include "MCP23017.h"


/*
 * MCP23017_DigitalWrite:
 *********************************************************************************
 */
void MCP23017_DigitalWrite (int fd, int pin, int value)
{
	int bit,old,gpio;
	
	//bit Identify
	bit = 1 << (pin & 7);

	//Bank Identify
	if (pin < 8)	// Bank A			
		gpio=MCP23x17_GPIOA;
    else			// Bank B
		gpio=MCP23x17_GPIOB;

	//Save old value
	old = SLD_PiI2CReadRegByte (fd, gpio);
	
	//Update the new value
	if (value == LOW)
      old &= (~bit);
    else
      old |= bit;
	
	//Apply the new value
    SLD_PiI2CWriteRegByte (fd, gpio, old);
  
	//printf("\npin: %d, bit:0x%X, old:0x%X\n\r",pin, bit, old);
}




/*
 * MCP23017_DigitalRead:
 *********************************************************************************
 */
int MCP23017_DigitalRead (int fd, int pin)
{
  int mask, value, gpio;

  if (pin < 8)		// Bank A
    gpio  = MCP23x17_GPIOA;
  else				// Bank B
  {
    gpio  = MCP23x17_GPIOB;
    pin  &= 0x07;
  }

  mask  = 1 << pin ;
  value = SLD_PiI2CReadRegByte (fd, gpio);

  if ((value & mask) == 0)
    return LOW;
  else 
    return HIGH;
}




/*
 * MCP23017_DirWrite:
  *********************************************************************************
 * I/O DIRECTION
 * 0: output
 * 1: input
 */
void MCP23017_DirWrite (int fd, int pin, int value)
{
	int bit,old,gpio;
	
	//bit Identify
	bit = 1 << (pin & 7);

	//Bank Identify
	if (pin < 8)	// Bank A			
		gpio=MCP23x17_IODIRA;
    else			// Bank B
		gpio=MCP23x17_IODIRB;

	//Save old value
	old = SLD_PiI2CReadRegByte (fd, gpio);
	
	//Update the new value
	if (value == LOW)
      old &= (~bit);
    else
      old |= bit;
	
	//Apply the new value
    SLD_PiI2CWriteRegByte (fd, gpio, old);

}




/*
 * MCP23017_DirRead:
 *********************************************************************************
 * I/O DIRECTION
 * 0: output
 * 1: input
 */
int MCP23017_DirRead (int fd, int pin)
{
  int mask, value, gpio;

  if (pin < 8)		// Bank A
    gpio  = MCP23x17_IODIRA;
  else				// Bank B
  {
    gpio  = MCP23x17_IODIRB;
    pin  &= 0x07;
  }

  mask  = 1 << pin ;
  value = SLD_PiI2CReadRegByte (fd, gpio);

  if ((value & mask) == 0)
    return LOW;
  else 
    return HIGH;
}





/*
 * MCP23017_WriteREGx:
 *********************************************************************************
 */
int MCP23017_WriteREGx (int fd, int reg, int val) {	
	SLD_PiI2CWriteRegByte (fd, reg, val);
	return LOW;	
}


/*
 * MCP23017_ReadREGx:
 *********************************************************************************
 */
int MCP23017_ReadREGx (int fd, int reg) {	
	int val=SLD_PiI2CReadRegByte (fd, reg);
	return val;	
}




/*
 * MCP23017_Setup:
 *********************************************************************************
 */

int MCP23017_Setup (int fd) {

	
	int wr_data=0x00; /*, read_data;*/
	
	//-------------------------------------------------
	// *** IOCON: I/O EXPANDER CONFIGURATION REGISTER ***
	/*	
	INTPOL: This bit sets the polarity of the INT output pin
		1 = Active-high
		0 = Active-low
	ODR: Configures the INT pin as an open-drain output
		1 = Open-drain output (overrides the INTPOL bit.)
		0 = Active driver output (INTPOL bit sets the polarity.)
	HAEN: Hardware Address Enable bit (MCP23S17 only) (Note 1)
		1 = Enables the MCP23S17 address pins.
		0 = Disables the MCP23S17 address pins.
	DISSLW: Slew Rate control bit for SDA output
		1 = Slew rate disabled
		0 = Slew rate enabled
	SEQOP: Sequential Operation mode bit
		1 = Sequential operation disabled, address pointer does not increment.
		0 = Sequential operation enabled, address pointer increments.
	MIRROR: INT Pins Mirror bit
		1 = The INT pins are internally connected
		0 = The INT pins are not connected. INTA is associated with PORTA and INTB is associated with PORTB
	BANK: Controls how the registers are addressed
		1 = The registers associated with each port are separated into different banks.
		0 = The registers are in the same bank (addresses are sequential).
	*/
	
	wr_data = 
			//IOCON_INTPOL		|	//INT Active-low
			//IOCON_ODR		    |	//INT Pin Active driver output
			//IOCON_HAEN		|	//unused for MCP23017
			//IOCON_DISSLW	    |	//SDA Slew rate enabled
			//IOCON_SEQOP		|	//Sequential operation enabled
			//IOCON_BANK_MODE	|	//The registers are in the same bank
			IOCON_MIRROR; 	    	//INT Pin internally connected
			
	SLD_PiI2CWriteRegByte (fd, MCP23x17_IOCON, wr_data);
	
	
	
	//-------------------------------------------------
	// *** IODIR: I/O DIRECTION REGISTER ***
	// BANK A (IODIRA)	
	//wr_data = 0b00000000;	// <PA7:PA0>: outputs
	//SLD_PiI2CWriteRegByte(fd, MCP23x17_IODIRA, wr_data);

	
	// BANK B (IODIRB)
	//wr_data = 0b11111111;	//<PB7:PB0>: inputs
	//SLD_PiI2CWriteRegByte(fd, MCP23x17_IODIRB, wr_data);
	
	
	
	//-------------------------------------------------
	// *** IPOL: INPUT POLARITY REGISTER ***
	
	// 1 = GPIO register bit reflects the opposite logic state of the input pin.
	// 0 = GPIO register bit reflects the same logic state of the input pin.
	
	// BANK A (IPOLA)	
	wr_data = 0b00000000;	// <PA7:PA0>: same logic state
	SLD_PiI2CWriteRegByte(fd, MCP23x17_IPOLA, wr_data);


	// BANK B (IPOLB)
	wr_data = 0b00000000;	// <PB7:PAB>: same logic state
	SLD_PiI2CWriteRegByte(fd, MCP23x17_IPOLB, wr_data);	



	//-------------------------------------------------
	// *** GPINTEN: INTERRUPT-ON-CHANGE PINS ***

	// 1 = Enables GPIO input pin for interrupt-on-change event.
	// 0 = Disables GPIO input pin for interrupt-on-change event.

	// BANK A (GPINTENA)
	wr_data = 0b00000000;	// <PA7:PA0>: 
	SLD_PiI2CWriteRegByte(fd, MCP23x17_GPINTENA, wr_data);
	
	// BANK B (GPINTENB)	
	wr_data = 0b00000000;	// <PB7:PB0>: PB3 enabled
	SLD_PiI2CWriteRegByte(fd, MCP23x17_GPINTENB, wr_data);


	
	//-------------------------------------------------
	// *** DEFVAL: DEFAULT VALUE REGISTER ***
	
	// DEFx<7:0>: Sets the compare value for pins configured for interrupt-on-change from defaults <7:0>
	// (Refer to INTCON)
	// If the associated pin level is the opposite from the register bit, an interrupt occurs. 
	// Refer to INTCON and GPINTEN.

	// BANK A (DEFVALA)
	wr_data = 0b00000000;	//DEFA<7:0>
	SLD_PiI2CWriteRegByte(fd, MCP23x17_DEFVALA, wr_data);


	// BANK B (DEFVALB)
	wr_data = 0b00000000;	//DEFB<7:0>
	SLD_PiI2CWriteRegByte(fd, MCP23x17_DEFVALB, wr_data);


	//-------------------------------------------------
	// *** INTCON: INTERRUPT-ON-CHANGE CONTROL REGISTER ***
	
	// 1 = Pin value is compared against the associated bit in the DEFVAL register.
	// 0 = Pin value is compared against the previous pin value.

	// BANK A (INTCONA)
	wr_data = 0b00000000;	//<PA7:PA0>
	SLD_PiI2CWriteRegByte(fd, MCP23x17_INTCONA, wr_data);

	// BANK B (INTCONB)
	wr_data = 0b00000000;	//<PB7:PB0>
	SLD_PiI2CWriteRegByte(fd, MCP23x17_INTCONB, wr_data);


	//-------------------------------------------------
	// *** GPPU: GPIO PULL-UP RESISTOR REGISTER ***

	// 1 = Pull-up (100K) enabled
	// 0 = Pull-up disabled

	// BANK A (GPPUA)
	wr_data = 0b00000000;	//<PA7:PA0>
	SLD_PiI2CWriteRegByte(fd, MCP23x17_GPPUA, wr_data);

	// BANK B (GPPUB)
	wr_data = 0b00000000;	//<PB7:PB0>
	SLD_PiI2CWriteRegByte(fd, MCP23x17_GPPUB, wr_data);


	//-------------------------------------------------
	// *** INTCAP: INTERRUPT CAPTURED REGISTER ***

	// BANK A (INTCAPA) => Cleared via this read
	SLD_PiI2CReadRegByte(fd, MCP23x17_INTCAPA);
	
	// BANK B (INTCAPB) => Cleared via this read
	SLD_PiI2CReadRegByte(fd, MCP23x17_INTCAPB);


	return LOW;	
}	



/*
 * MCP23017_CheckRegister:
 *********************************************************************************
 */
int MCP23017_CheckRegister (int fd) {
	
	int read_data;

	
	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_IODIRA);	//Read IODIRA
	printf("IODIRA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_IODIRB);	//Read IODIRB
	printf("IODIRB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_IPOLA);	//Read IPOLA
	printf("IPOLA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_IPOLB);	//Read IPOLB
	printf("IPOLB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_GPINTENA);	//Read GPINTENA
	printf("GPINTENA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_GPINTENB);	//Read GPINTENB
	printf("GPINTENB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_DEFVALA);	//Read DEFVALA
	printf("DEFVALA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_DEFVALB);	//Read DEFVALB
	printf("DEFVALB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_INTCONA);	//Read INTCONA
	printf("INTCONA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_INTCONB);	//Read INTCONB
	printf("INTCONB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_GPPUA);	//Read GPPUA
	printf("GPPUA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_GPPUB);	//Read GPPUB
	printf("GPPUB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_INTFA);	//Read INTFA
	printf("INTFA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_INTFB);	//Read INTFB
	printf("INTFB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_INTCAPA);	//Read INTCAPA
	printf("INTCAPA: 0x%X\n\r", read_data);	
	
	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_INTCAPB);	//Read INTCAPB
	printf("INTCAPB: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_GPIOA);	//Read GPIOA
	printf("GPIOA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_GPIOB);	//Read GPIOB
	printf("GPIOB: 0x%X\n\r", read_data);	
	
	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_OLATA);	//Read OLATA
	printf("OLATA: 0x%X\n\r", read_data);

	read_data = SLD_PiI2CReadRegByte (fd, MCP23x17_OLATB);	//Read OLATB
	printf("OLATB: 0x%X\n\r", read_data);	
	
	
	return LOW;	
}