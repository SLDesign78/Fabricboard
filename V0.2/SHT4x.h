/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SHT4x.h
*************************************************************************************************************/
//  Description  : SHT4x Interface
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 08/05/2023: creation

*************************************************************************************************************/


/* all measurement commands return T (CRC) RH (CRC) */
#define SHT4X_CMD_MEASURE_HPM 0xFD
#define SHT4X_CMD_MEASURE_MPM 0xF6
#define SHT4X_CMD_MEASURE_LPM 0xE0
#define SHT4X_CMD_READ_SERIAL 0x89
#define SHT4X_CMD_SOFT_RESET 0x94

#define SHT4X_CMD_ACT_HEATER_200mW_1s 		0x39
#define SHT4X_CMD_ACT_HEATER_200mW_100ms 	0x32
#define SHT4X_CMD_ACT_HEATER_110mW_1s 		0x2F
#define SHT4X_CMD_ACT_HEATER_110mW_100ms 	0x24
#define SHT4X_CMD_ACT_HEATER_20mW_1s 		0x1E
#define SHT4X_CMD_ACT_HEATER_20mW_100ms 	0x15


#ifdef __cplusplus
extern "C" {
#endif

uint8_t crc8(uint8_t *data, uint16_t length);
uint32_t SHT4x_GetSerialNumber(int fd);

void SHT4x_GetTemp_Humidity(int fd, uint8_t CMD, float* Temp, float* RH);
void SHT4x_GetTemp_Humidity_HPM(int fd, float* Temp, float* RH);
void SHT4x_GetTemp_Humidity_MPM(int fd, float* Temp, float* RH);
void SHT4x_GetTemp_Humidity_LPM(int fd, float* Temp, float* RH);


#ifdef __cplusplus
}
#endif

