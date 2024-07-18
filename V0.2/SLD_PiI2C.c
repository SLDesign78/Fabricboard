/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SLD_PiI2C.c
*************************************************************************************************************/
//  Description  : I2C Interface, 
//					Simplified I2C access routines
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 08/05/2023: creation

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



#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>


#include "Define.h"
#include "SLD_PiI2C.h"
#include "SLD_MISC_FUNC.h"

uint8_t fdToSlaveAddress[1024] = { 0xFF };


static inline int i2c_smbus_access (int fd, char rw, uint8_t command, int size, union i2c_smbus_data *data)
{
  struct i2c_smbus_ioctl_data args ;

  args.read_write = rw ;
  args.command    = command ;
  args.size       = size ;
  args.data       = data ;
  return ioctl (fd, I2C_SMBUS, &args) ;
}



/*********************************************************************************
 * SLD_PiI2CRead:
 *	Simple device read
 *********************************************************************************
 */

int SLD_PiI2CRead (int fd)
{
  union i2c_smbus_data data ;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
    return -1 ;
  else
    return data.byte & 0xFF ;
}


/*********************************************************************************
 * SLD_PiI2CReadRegByte:
 *	Read an byte (8-bit) value from a regisiter on the device
 *********************************************************************************
 */

int SLD_PiI2CReadRegByte (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
    return -1 ;
  else
    return data.byte & 0xFF ;
}





/*********************************************************************************
 * SLD_PiI2CReadRegWord:
 *	Read an word (16-bit) value from a regisiter on the device
 *********************************************************************************
 */

int SLD_PiI2CReadRegWord (int fd, int reg)
{
  union i2c_smbus_data data;

  if (i2c_smbus_access (fd, I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data))
    return -1 ;
  else
    return data.word & 0xFFFF ;
}








/*********************************************************************************
 * SLD_PiI2CReadBlock:
 *	Read values from consecutive regisiters on the device
 *********************************************************************************
 */
int SLD_PiI2CReadBlock (int fd, int reg, uint8_t *buff, int size)
{
	struct i2c_rdwr_ioctl_data	i2c;
	struct i2c_msg 			msgs[2];

	uint8_t reg_addr[1] = { reg };

	msgs[0].addr	= fdToSlaveAddress[fd];
	msgs[0].flags	= 0;
	msgs[0].len	= 1;
	msgs[0].buf	= reg_addr;

	msgs[1].addr	= fdToSlaveAddress[fd];
	msgs[1].flags	= I2C_M_RD;
	msgs[1].len	= size;
	msgs[1].buf	= buff;

	i2c.msgs	= msgs;
	i2c.nmsgs	= 2;

	return ioctl( fd, I2C_RDWR, &i2c );
}



/*********************************************************************************
 * SLD_PiI2CReadBlockData:
 *	Read values from consecutive registers on the device
 *********************************************************************************
 */
int SLD_PiI2CReadBlockData (int fd, uint8_t *buff, int size)
{
	struct i2c_rdwr_ioctl_data	i2c;
	struct i2c_msg 			msgs[1];

	msgs[0].addr	= fdToSlaveAddress[fd];
	msgs[0].flags	= I2C_M_RD;
	msgs[0].len	= size;
	msgs[0].buf	= buff;

	i2c.msgs	= msgs;
	i2c.nmsgs	= 1;

	return ioctl( fd, I2C_RDWR, &i2c );
}



/*
 * SLD_PiI2CWrite:
 *	Simple device write
 *********************************************************************************
 */

int SLD_PiI2CWrite (int fd, int data)
{
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL) ;
}


/*
 * SLD_PiI2CWriteRegByte: wiringPiI2CWriteReg16:
 *	Write an Byte 8-bit value to the given register
 *********************************************************************************
 */

int SLD_PiI2CWriteRegByte (int fd, int reg, int value)
{
  union i2c_smbus_data data ;

  data.byte = value ;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data) ;
}




/*
 * SLD_PiI2CWriteRegByte: wiringPiI2CWriteReg16:
 *	Write an word 16-bit value to the given register
 *********************************************************************************
 */

int SLD_PiI2CWriteRegWord (int fd, int reg, int value)
{
  union i2c_smbus_data data ;

  data.word = value ;
  return i2c_smbus_access (fd, I2C_SMBUS_WRITE, reg, I2C_SMBUS_WORD_DATA, &data) ;
}


/*
 * SLD_PiI2CWrite32Block:
 *	Write values from consecutive registers to the given register 32bits
 *********************************************************************************
 */

int SLD_PiI2CWrite32Block (int fd, uint32_t reg, uint8_t *buff, int size)
{
	uint8_t temp[50];
	
	temp[3] = reg & 0xff;
    temp[2] = (reg >> 8) & 0xff;
    temp[1] = (reg >> 16) & 0xff;
    temp[0] = (reg >> 24) & 0xff;
	
	memcpy(&temp[4], buff, size);

	struct i2c_rdwr_ioctl_data	i2c;
	struct i2c_msg			msgs;

	msgs.addr	= fdToSlaveAddress[fd];
	msgs.flags	= 0;
	msgs.len	= size + 4;
	msgs.buf	= temp;

	i2c.msgs	= &msgs;
	i2c.nmsgs	= 1;

	return ioctl( fd, I2C_RDWR, &i2c );
}


/*
 * SLD_PiI2CWriteBlock:
 *	Write values from consecutive registers to the given register 8bits
 *********************************************************************************
 */
 
int SLD_PiI2CWriteBlock (int fd, int reg, uint8_t *buff, int size)
{
	uint8_t temp[50];
	temp[0] = reg;
	memcpy(&temp[1], buff, size);

	struct i2c_rdwr_ioctl_data	i2c;
	struct i2c_msg			msgs;

	msgs.addr	= fdToSlaveAddress[fd];
	msgs.flags	= 0;
	msgs.len	= size + 1;
	msgs.buf	= temp;

	i2c.msgs	= &msgs;
	i2c.nmsgs	= 1;

	return ioctl( fd, I2C_RDWR, &i2c );
}




/*
 * SLD_PiI2CWriteBlockData:
 *	Write values from consecutive registers on the device
 *********************************************************************************
 */
 
int SLD_PiI2CWriteBlockData (int fd, uint8_t *buff, int size)
{
	struct i2c_rdwr_ioctl_data	i2c;
	struct i2c_msg			msgs;

	msgs.addr	= fdToSlaveAddress[fd];
	msgs.flags	= 0;
	msgs.len	= size + 1;
	msgs.buf	= buff;

	i2c.msgs	= &msgs;
	i2c.nmsgs	= 1;

	return ioctl( fd, I2C_RDWR, &i2c );
}





/*
 * SLD_PiI2CSetupInterface:
 *	Open the I2C device, and register the target device
 *********************************************************************************
 */

int SLD_PiI2CSetupInterface (const char *device, int devId)
{
	int fd;
	
	if ((fd = open (device, O_RDWR)) < 0) {
		fprintf (stdout, "Unable to open I2C device: %s\n", strerror (errno));
	}

	if (ioctl (fd, I2C_SLAVE, devId) < 0) {
		fprintf (stdout, "Unable to select I2C device: %s\n", strerror (errno));
	}
	
	fdToSlaveAddress[fd] = devId;

	return fd;
}


/*
 * SLD_PiI2CSetup:
 *	Open the I2C device, and regisiter the target device
 *********************************************************************************
 */
/*
int SLD_PiI2CSetup (const int devId)
{
	int model, rev, mem, maker, overVolted ;
	const char *device = NULL;

	piBoardId (&model, &rev, &mem, &maker, &overVolted) ;

	switch(model)	{
	case MODEL_ODROID_C1:
	case MODEL_ODROID_C2:
		if (cmpKernelVersion(KERN_NUM_TO_MAJOR, 4))
			device = "/dev/i2c-0";
		else
			device = "/dev/i2c-1";
	break;
	case MODEL_ODROID_XU3:
		if (cmpKernelVersion(KERN_NUM_TO_MAJOR, 5))
			device = "/dev/i2c-0";
		else
			device = "/dev/i2c-1";
	break;
	case MODEL_ODROID_N1:
		device = "/dev/i2c-4";
	break;
	case MODEL_ODROID_N2:
	case MODEL_ODROID_C4:
	case MODEL_ODROID_HC4:
	case MODEL_BANANAPI_M5:
	case MODEL_BANANAPI_M2PRO:
		if (cmpKernelVersion(KERN_NUM_TO_REVISION, 4, 9, 230))
			device = "/dev/i2c-0";
		else
			device = "/dev/i2c-2";
	break;
	case MODEL_BANANAPI_M2S:
	case MODEL_BANANAPI_CM4:
	case MODEL_BANANAPI_RPICM4:
		device = "/dev/i2c-2";
	break;
	}

	return SLD_PiI2CSetupInterface (device, devId) ;
}
*/

