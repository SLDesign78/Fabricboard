/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |__  /|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  PCA9545A.c
*************************************************************************************************************/
//  Description  : PCA9545A Interface
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 01/04/2024: creation

*************************************************************************************************************/

#include <stdio.h>
#include <stdint.h>

#include "Define.h"
#include "SLD_PiI2C.h"
#include "PCA9545A.h"



/*
 * PCA9545A_GetChannelStatus:
 *********************************************************************************
 */
int PCA9545A_GetChannelStatus (int fd) {	
	return SLD_PiI2CRead (fd);
}


/*
 * PCA9545A_SetChannel:
 *********************************************************************************
 */
int PCA9545A_SetChannel (int fd, int Channel) {	
	SLD_PiI2CWrite (fd, Channel);
	return LOW;	
}



