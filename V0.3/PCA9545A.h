/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2024

  PCA9545A.h
*************************************************************************************************************/
//  Description  : PCA9545A Interface
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 01/04/2024: creation

*************************************************************************************************************/


// PCA9545A Control Register

#define	PCA9545A_NOCH		0x00
#define	PCA9545A_CHAN0		0x01
#define	PCA9545A_CHAN1		0x02
#define	PCA9545A_CHAN2		0x04
#define	PCA9545A_CHAN3		0x08
#define	PCA9545A_ALLCHAN	0x07

#define	PCA9545A_INT_CHAN0	0x1F
#define	PCA9545A_INT_CHAN1	0x2F
#define	PCA9545A_INT_CHAN2	0x4F
#define	PCA9545A_INT_CHAN3	0x8F


#ifdef __cplusplus
extern "C" {
#endif

int PCA9545A_GetChannelStatus (int fd);
int PCA9545A_SetChannel (int fd, int Channel);

#ifdef __cplusplus
}
#endif

