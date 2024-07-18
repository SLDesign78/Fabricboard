/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  ISL28022.h
*************************************************************************************************************/
//  Description  : ISL28022 Digital Power monitor (HAL Driver)
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 01/07/2024: creation

*************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ISL28022_H
#define __ISL28022_H


#define ISL_RG_CFG				0x00	//Power-on reset, bus and shunt ranges, ADC acquisition times, mode configuration
#define ISL_RG_SHUNT_VLT		0x01	//Shunt voltage measurement value
#define ISL_RG_BUS_VLT			0x02	//Bus voltage measurement value
#define ISL_RG_POWER			0x03	//Power measurement value
#define ISL_RG_CURRENT			0x04	//Current measurement value
#define ISL_RG_CAL				0x05	//Register used to enable current and power measurements.
#define ISL_RG_SHUNT_VLT_THRSH	0x06	//Min/Max shunt thresholds
#define ISL_RG_BUS_VLT_THRSH	0x07	//Min/Max VBUS thresholds
#define ISL_RG_DCS_INT_STAT		0x08	//Threshold interrupts
#define ISL_RG_AUX_CTRL			0x09	//Register to control the interrupts and external clock functionality



uint16_t Get_Reg_ISL28022(int fd, uint8_t Reg, uint8_t Size);
uint16_t Set_Reg_ISL28022(int fd, uint8_t Reg, uint8_t Size, uint16_t Reg_Data );
void Init_ISL28022(int fd);
uint32_t GetAvgVal_ISL28022(int fd, uint8_t AvgSize, uint8_t Reg);

float Get_ISL_Shunt_Voltage(int fd);
float Get_ISL_Voltage(int fd);
float Get_ISL_Current(int fd);
float Get_ISL_Power(int fd);

#endif /* __ISL28022_H */
