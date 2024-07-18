/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_POWER_REFCLK_RESET.h
*************************************************************************************************************/
//  Description  : Power, Reclock, Reset PCIe control
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 10/05/2024: creation

*************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SLD_POWER_REFCLK_RESET_H
#define __SLD_POWER_REFCLK_RESET_H

//Functions
void SYS_Power_CTRL(void);
void SYS_Refclk_CTRL(void);
void SYS_Reset_CTRL(void);

void PERST_Assert(void);
void PERST_Deassert(void);
void Set_PERST_MODE_to_EEP(uint16_t, uint8_t*, uint8_t);
void Get_PERST_MODE_from_EEP(uint16_t, uint8_t*, uint8_t);

#endif /* __SLD_POWER_REFCLK_RESET */
