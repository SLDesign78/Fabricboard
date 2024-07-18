/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  SLD_SLOT.h
*************************************************************************************************************/
//  Description  : Slots configuration
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 31/12/2021: creation

*************************************************************************************************************/

#ifndef INC_SLD_SLOT_H_
#define INC_SLD_SLOT_H_

uint8_t Get_SLOT_TYPE_from_file(char* file_name, uint8_t verb);
void SLOT_CFG_DFLT();
void Init_SW_CFG();
void Get_SW_MODE();
void Init_SLOT_CFG(uint8_t verb);
void Get_SLOT_CFG();
void Check_SYS_PCIe_RESET_MODE();
void Check_SLOT_PRSNT_STATUS();
void Check_CARD_PRSNT_STATUS();

#endif /* INC_SLD_SLOT_H_ */
