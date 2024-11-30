/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SLD_PiI2C.h
*************************************************************************************************************/
//  Description  : SLD_I2C Interface
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 08/05/2023: creation

*************************************************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif


int SLD_PiI2CRead (int fd);
int SLD_PiI2CReadRegByte (int fd, int reg);
int SLD_PiI2CReadRegWord (int fd, int reg);
int SLD_PiI2CReadBlock (int fd, int reg, uint8_t *buff, int size);
int SLD_PiI2CReadBlockData (int fd, uint8_t *buff, int size);

int SLD_PiI2CWrite (int fd, int data);
int SLD_PiI2CWriteRegByte (int fd, int reg, int value);
int SLD_PiI2CWriteRegWord (int fd, int reg, int value);
int SLD_PiI2CWriteBlock (int fd, int reg, uint8_t *buff, int size);
int SLD_PiI2CWrite32Block (int fd, uint32_t reg, uint8_t *buff, int size);

int SLD_PiI2CWriteBlockData (int fd, uint8_t *buff, int size);
int SLD_PiI2CSetupInterface (const char *device, int devId);
//int SLD_PiI2CSetup (const int devId);


#ifdef __cplusplus
}
#endif

