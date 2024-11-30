/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2023

  SLD_MISC_FUNC.c
*************************************************************************************************************/
//  Description  : Miscallaenous functions, 
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry/BPI Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 16/12/2023: creation

*************************************************************************************************************/

#include "fabricboard.h"


/*
 * delay_ms:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */

void delay_ms (unsigned int howLong)
{
  struct timespec sleeper, dummy;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}
 
 
   /*
 * StringComp:
 *	String compare
 *********************************************************************************
 */
 bool StringComp(char* StrData, const char* Str) {
  bool res;

  //printf("STR_USER: %s %ld\r\n", StrData, strlen(StrData));
  //printf("STR_CMD: %s %ld\r\n", Str, strlen(Str));

  if (strlen(StrData) > 0 && 
      strlen(StrData) == strlen(Str) &&
      !strncmp((const char*)StrData, Str, strlen(Str)))  
     
  res = TRUE;
  else res = FALSE;
  return res;
}


   /*
 * Echo_LFCR
 *	Lead feed and Retirn Carriage
 *********************************************************************************
 */
  
 void Echo_LFCR() {
	printf("\n\r");
 } 
 
 
 /*______________________________________________________________________________________*/
/*!
 * \brief Convert unsigned long value to 4 bytes Array Binary
 *
 */
/*______________________________________________________________________________________*/

void U32toArr(uint32_t val, uint8_t *Buff) {
	Buff[0] = (uint8_t)((val & 0xFF000000) >> 24);
	Buff[1] = (uint8_t)((val & 0x00FF0000) >> 16);
	Buff[2] = (uint8_t)((val & 0x0000FF00) >> 8);
	Buff[3] = (uint8_t)(val & 0x000000FF);
}


/*______________________________________________________________________________________*/
/*!
 * \brief Convert unsigned long value to 2 bytes Array Binary
 *
 */
/*______________________________________________________________________________________*/

void U16toArr(uint16_t val, uint8_t *Buff) {
	Buff[0] = (uint8_t)((val & 0xFF00) >> 8);
	Buff[1] = (uint8_t)(val & 0x00FF);
}

 
 
 
