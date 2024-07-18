/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SLD_GPIO.h
*************************************************************************************************************/
//  Description  : SLD_GPIO Interface
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 17/07/2023: creation

*************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


int GPIO_WR(uint16_t line_num, char *chipname, uint8_t val);
uint8_t GPIO_RD(uint16_t line_num, char *chipname); 
void GPIO_EXT_WR_OFF(); 
void GPIO_EXT_WR_ON();

void GPIO_I2CEXP_WR(uint8_t EXP, int pin, int val);
int GPIO_I2CEXP_RD(uint8_t EXP, int pin); 

void GPIO_I2CEXP_DirWR(uint8_t EXP, int pin, int val); 
int GPIO_I2CEXP_DirRD(uint8_t EXP, int pin); 
 
 
 #ifdef __cplusplus
}
#endif