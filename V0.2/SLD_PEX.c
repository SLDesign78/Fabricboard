/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_PEX.c
*************************************************************************************************************/
//  Description  : Switch PCIe BroadcomPEX88096 
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 25/06/2024: creation
*************************************************************************************************************/

#include "fabricboard.h"


/*___________________________________________________________________________*/
/*!
*\brief Intialization PEX I2C Interface
*/

void PEX_I2C_Access(uint8_t verb) {
	
	if ((fd_PEX_i2c = SLD_PiI2CSetupInterface (i2cDevice, PEX88096_AD)) < 0) { 
		printf ("Error: Unable to access to PEX88096 I2C: %s\n", strerror (errno));
	}
	else if ( PEX_I2CRead32(fd_PEX_i2c, 0x00000000) == PEX_ERROR ) { 	//Check Device Address	
		printf ("Error: Unable to access to PEX88096 I2C: %s\n", strerror (errno));
	}
	else {
		delay_ms(10);
		if(verb) printf ("=> Access to PEX88096 I2C: OK\r\n"); 
	}
}



/*___________________________________________________________________________*/
/*!
*\brief Read a PEX 32bits Registers
*\
  * @param  fd: File Device structure that contains 
  * 		the configuration information for I2C module
  * @param  RegAddress Internal memory address 32-bit
  * @retval Dataread
*/

uint32_t PEX_I2CRead32(int fd, uint32_t RegAddress) {

	uint8_t ADDR_BYTE[4];
	uint32_t DataRead=0;

	//Convert  Ulong Addrress to Array
	U32toArr(RegAddress, ADDR_BYTE);

	if( SLD_PiI2CWriteBlockData(fd, ADDR_BYTE, 4) < 0)
		return PEX_ERROR;
	else if( SLD_PiI2CReadBlockData (fd, ADDR_BYTE, 4) <0)
		return PEX_ERROR;
	else 
		DataRead = (uint32_t)(ADDR_BYTE[0] << 24 | ADDR_BYTE[1] << 16 | ADDR_BYTE[2] << 8 | ADDR_BYTE[3]);

	return DataRead;
}



/*___________________________________________________________________________*/
/*!
*\brief Write to PEX 32bits Registers
*\
  * @param  fd: File Device structure that contains 
  * 		the configuration information for I2C module
  * @param  RegAddress Internal memory address 32-bit
  * @param  Register Data Internal memory address 32-bit 
 * @retval  Data status
*/

uint32_t PEX_I2CWrite32(int fd, uint32_t RegAddress, uint32_t RegData) { 

	uint8_t DATA_BYTE[4];

	//Convert  Ulong Data to Array
	U32toArr(RegData, DATA_BYTE);

	if( SLD_PiI2CWrite32Block(fd, RegAddress, DATA_BYTE, 4) < 0)
		return PEX_ERROR;
	else 
		return HIGH;

}



/*___________________________________________________________________________*/
/*!
*\brief Read an AXI Register
*\
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  RegAddress Internal memory address 32-bit
  * @retval Data read
*/

uint32_t AxiRegRead(int fd, uint32_t RegAddress ) {

		uint32_t DataRead=0;

		//Set the pointer to register offset RegAddress
		if (PEX_I2CWrite32(fd, 0x03583C40, RegAddress) == PEX_ERROR) goto Status_Err;
		
		//Initiate a read to register offset RegAddress
		if (PEX_I2CWrite32(fd, 0x03583C42, 0x00000002) == PEX_ERROR) goto Status_Err;

		//Read CHIME_AXI_CSR_DATA register (offset 0x001F0104)
		DataRead = PEX_I2CRead32(fd, 0x04583C41);
		if (DataRead == PEX_ERROR) goto Status_Err;

		//Get Value from Register
		return DataRead;

		//I2C Status Error
		Status_Err:	return PEX_ERROR;
}



/*___________________________________________________________________________*/
/*!
*\brief Write an AXI Register
*\
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  RegAddress Internal memory address 32-bit
  * @param  DataWrite Data to write 32-bit 
  * @retval Data Status
*/


uint32_t AxiRegWrite(int fd, uint32_t RegAddress, uint32_t DataWrite) {

	//Set the pointer to register offset RegAddress
	if (PEX_I2CWrite32(fd, 0x03583C40, RegAddress) == PEX_ERROR) goto Status_Err;

	//Initiate a write of DataWrite to register offset RegAddress
	if (PEX_I2CWrite32(fd, 0x03583C41, DataWrite) == PEX_ERROR) goto Status_Err;

	//Write 0x01 to CHIME_AXI_CSR_CONTROL register (offset 0x001F0108)
	if (PEX_I2CWrite32(fd, 0x03583C42, 0x00000001) == PEX_ERROR) goto Status_Err;

	return HIGH;

	//I2C Status Error
	Status_Err: return PEX_ERROR;

}




/*___________________________________________________________________________*/
/*!
*\brief Get Raw Temperature from PEX88096 die
*\
*\ @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
*\ Return: Raw Temperature in Deg C
*\ 		   0x0000FFFF if I2C bus failed
	// STEP 1
	//Set the pointer to register offset 0xFFE78504
	//Write 0xFFE78504 to CHIME_AXI_ADDRESS register (offset 0x001F0100)

	// STEP 2
	//Initiate a write of 0x200653E8 to register offset 0xFFE78504
    //Write 0x200653E8 to CHIME_AXI_CSR_DATA register (offset 0x001F0104)

	// STEP 3
	//Initiate a write of 0x200653E8 to register offset 0xFFE78504
	//Write 0x01 to CHIME_AXI_CSR_CONTROL register (offset 0x001F0108)

	// STEP 4
	//Set the pointer to register offset 0xFFE78538
	//Write 0xFFE78538 to CHIME_AXI_ADDRESS register (offset 0x001F0100)

	// STEP 5
	//Initiate a read to register offset 0xFFE78538
	//Write 0x02 to CHIME_AXI_CSR_CONTROL register (offset 0x001F0108)

	// STEP 6
	//Initiate a read to offset 0xFFE78538
	//Read CHIME_AXI_CSR_DATA register (offset 0x001F0104)
*/

uint16_t PEX_get_RawTemp(int fd) {

	uint32_t RawTemp=0, Reg_04;

	//Save register 0xFFE78504
	Reg_04=AxiRegRead(fd, 0xFFE78504);

	//Initiate a write of 0x200653E8 to register offset 0xFFE78504
	if (AxiRegWrite(fd, 0xFFE78504, 0x200653E8) == PEX_ERROR) goto Status_Err;

	//Initiate a read to register offset 0xFFE78538
	RawTemp=AxiRegRead(fd, 0xFFE78538);
	if (RawTemp == PEX_ERROR) goto Status_Err;

	//Restore register 0xFFE78504
	if (AxiRegWrite(fd, 0xFFE78504, Reg_04) == PEX_ERROR) goto Status_Err;

	//Get Raw Temperature
	return (uint16_t)(RawTemp & 0x0000FFFF);

	//I2C Status Error
	Status_Err:	return (uint16_t)PEX_ERROR;

}



/*___________________________________________________________________________*/
/*!
*\brief Get PEX88096 die Temperature
*\
*\ @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
*\ Return: (float) Temperature in Deg C
*\
*/

float PEX_get_Temp( uint16_t RawTemp) {
	return (float)(RawTemp) / (float)128.0;
}




/*___________________________________________________________________________*/
/*!
*\brief Get PEX's ARM Firmware Version
*/

uint32_t PEX_get_ARM_FW_Version(int fd, uint8_t *PEX_VERS_DATA) {

	uint8_t BUFF_DATA[4];

	//Read PCIe Register 0xA8 (Subsystem ID) to get PEX ARM FW version from LowerByte
	uint32_t Dataread = PEX_I2CRead32(fd, 0x04003C2A);

	//Convert  Ulong (32-bit) to Array
	U32toArr(Dataread, BUFF_DATA);	

	PEX_VERS_DATA[0] = (BUFF_DATA[2] & 0xF0) >> 4;
	PEX_VERS_DATA[1] = BUFF_DATA[2] & 0x0F;
	PEX_VERS_DATA[2] = (BUFF_DATA[3] & 0xF0) >> 4;
	PEX_VERS_DATA[3] = BUFF_DATA[3] & 0x0F;

	return Dataread;
}


/*___________________________________________________________________________*/
/*!
*\brief Get PEX's Firmware Revision
*/

uint32_t PEX_get_FW_Revision(int fd, uint8_t *PEX_VERS_DATA) {

	uint8_t BUFF_DATA[4];

	//Read PCIe Register 0xA8 (Subsystem ID) to get PEX ARM FW version from LowerByte
	uint32_t Dataread = PEX_I2CRead32(fd, 0x04003C2A);
	
	//Convert  Ulong (32-bit) to Array
	U32toArr(Dataread, BUFF_DATA);

	PEX_VERS_DATA[0] = (BUFF_DATA[0] & 0xF0) >> 4;
	PEX_VERS_DATA[1] = BUFF_DATA[0] & 0x0F;
	PEX_VERS_DATA[2] = (BUFF_DATA[1] & 0xF0) >> 4;
	PEX_VERS_DATA[3] = BUFF_DATA[1] & 0x0F;

	return Dataread;
}
