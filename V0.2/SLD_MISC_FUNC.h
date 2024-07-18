/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SLD_MISC_FUNC.h
*************************************************************************************************************/
//  Description  : Miscannalleous Functions
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 16/12/2023: creation

*************************************************************************************************************/

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void delay_ms (unsigned int howLong);
bool StringComp(char* StrData, const char* Str);
void Echo_LFCR();
void U32toArr(uint32_t val, uint8_t *Buff);
void U32toArrInv(uint32_t val, uint8_t *Buff);
void U16toArr(uint16_t val, uint8_t *Buff);

 
 #ifdef __cplusplus
}
#endif