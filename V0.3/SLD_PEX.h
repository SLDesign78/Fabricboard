/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_PEX.h
*************************************************************************************************************/
//  Description  : Switch PCIe BroadcomPEX88096 
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 25/06/2024: creation
*************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PEX_H_
#define __PEX_H_

int PEX_I2C_Access(uint8_t verb);
uint32_t PEX_I2CRead32 (int fd, uint32_t RegAddress) ; 
uint32_t PEX_I2CWrite32 (int fd, uint32_t RegAddress, uint32_t RegData);
uint16_t PEX_get_RawTemp(int fd);
float PEX_get_Temp( uint16_t RawTemp);
uint32_t PEX_get_ARM_FW_Version(int fd, uint8_t *PEX_VERS_DATA);
uint32_t PEX_get_FW_Revision(int fd, uint8_t *PEX_VERS_DATA);

#endif /* __PEX_H */
