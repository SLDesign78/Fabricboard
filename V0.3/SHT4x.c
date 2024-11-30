/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SHT4x.c
*************************************************************************************************************/
//  Description  : SHT4x Interface
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 08/05/2023: creation

*************************************************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "Define.h"
#include "SLD_PiI2C.h"
#include "SLD_MISC_FUNC.h"
#include "SHT4x.h"



/*********************************************************************************
 * CRC-8 Calculation:
 *  	polynomial: 0x31
 *  	Initial Value: 0xFF
 * 		Final Xor Value: 0x00
 *********************************************************************************
 */
uint8_t crc8(uint8_t *data, uint16_t length)
{
    const uint8_t polynomial = 0x31;
    uint8_t crc = 0xFF;
    uint16_t i, j;
    
    for (i = 0; i < length; i++)
    {
        crc ^= data[i];
         for (j = 0; j < 8; j++)
        {
            if (crc & 0x80) crc = (crc << 1) ^ polynomial;
            else crc <<= 1;
        }
    }
    return crc;
}




/*
 * GetSerialNumber():
 *********************************************************************************
 */

uint32_t SHT4x_GetSerialNumber(int fd)
{
	
	uint8_t SN_Data[6], CRC_data[2], data[2];
	
	
	SLD_PiI2CWrite(fd, SHT4X_CMD_READ_SERIAL);
	delay_ms(1);
	SLD_PiI2CReadBlockData(fd, SN_Data, 6);
	
	/*
	for (int i = 0 ; i < 6 ; ++i) {
		printf("SN[%d]:%X\n\r", i, SN_Data[i]);
	}
	*/
	//Get CRC_1
	for (int i = 0 ; i < 2 ; ++i) data[i]=SN_Data[i];
	CRC_data[0]=crc8(data, sizeof(data));
	//printf("CRC_1:%X\n\r", CRC_data[0]);
	
	//Get CRC_2
	for (int i = 0 ; i < 2 ; ++i) data[i]=SN_Data[3+i];
	CRC_data[1]=crc8(data, sizeof(data));
	//printf("CRC_2:%X\n\r", CRC_data[1]);
	
	//Check CRC_1 & CRC_2
	if(CRC_data[0]==SN_Data[2] && CRC_data[1]==SN_Data[5]) {
		return (SN_Data[4] << 24) | (SN_Data[3] << 16) | (SN_Data[1] << 8) | SN_Data[0];
	}
	else return -1;
}




/*
 * GetTemp_Humidity()
 * Measure Temmperature & Relative Humidity for any precision
 *
 * return Array[2]: RH, TEMP
 *********************************************************************************
 */

void SHT4x_GetTemp_Humidity(int fd, uint8_t CMD, float* Temp, float* RH) {

	uint16_t t_ticks, rh_ticks;
	uint8_t I2C_data[6];

	// Get Temp & RH
	SLD_PiI2CWrite(fd, CMD);
	delay_ms(50);
	SLD_PiI2CReadBlockData(fd, I2C_data, 6);
					
	/* Debug
	for (int i = 0 ; i < 6 ; ++i) {
		printf("TEMP_RH DATA[%d]:%X\n\r", i, I2C_data[i]);
	}
	*/
					
	t_ticks = (I2C_data[0] << 8) + I2C_data[1];
	//uint8_t checksum_t = I2C_data[2];
	rh_ticks = (I2C_data[3] << 8) + I2C_data[4];
	//uint8_t checksum_rh = I2C_data[5];
					
	*Temp =  (float)(((21875 * t_ticks) >> 13) - 45000)/1000.0;
	*RH = (float)(((15625 * rh_ticks) >> 13) - 6000)/1000.0;
					
	//RH filter				
	if (*RH > 100.0) *RH = 100.0;
	if (*RH < 0.0) *RH = 0.0;

}



/*
 * GetTemp_Humidity_HPM()
 * Measure Temmperature & Relative Humidity with High precision
 *
 *********************************************************************************
 */

void SHT4x_GetTemp_Humidity_HPM(int fd, float* Temp, float* RH) {
	SHT4x_GetTemp_Humidity(fd, SHT4X_CMD_MEASURE_HPM, Temp, RH);
}


/*
 * GetTemp_Humidity_MPM()
 * Measure Temmperature & Relative Humidity with Medium precision
 *
 *********************************************************************************
 */

void SHT4x_GetTemp_Humidity_MPM(int fd, float* Temp, float* RH) {
	SHT4x_GetTemp_Humidity(fd, SHT4X_CMD_MEASURE_MPM, Temp, RH);
}


/*
 * GetTemp_Humidity_LPM()
 * Measure Temmperature & Relative Humidity with Low precision
 *
 *********************************************************************************
 */

void SHT4x_GetTemp_Humidity_LPM(int fd, float* Temp, float* RH) {
	SHT4x_GetTemp_Humidity(fd, SHT4X_CMD_MEASURE_LPM, Temp, RH);
}

