/*************************************************************************************************************
   ____  _     ____  _____ ____  _  ____  __    _
  / ___|| |   |  _ \|  ___/ ___|| |/ ___||  \  | |
   (__  | |   | | \ | |_   (__  | | / __ | | \ | |
  \___ \| |   | | | |  _| \___ \| | ||_ \| |\ \| |
   __ ) | |___| |_/ | |___ __ ) | | \__| | | \ | |
  |____/|_____|____/|_____|____/|_|\____/|_|  \__|

  Copyright (c) 2022

  ISL28022.c
*************************************************************************************************************/
//  Description  : ISL28022 Digital Power monitor (HAL Driver)
//  Author(s)    : S.LORET
//  Target(s)    : Raspberry Pi ARM Cortex CPU Core
/*************************************************************************************************************
Rev 0.1 - 01/07/2024: creation

*************************************************************************************************************/

#include "fabricboard.h"


/*___________________________________________________________________________*/
/*!
*\fn uint16_t Get_Reg_ISL28022(int fd, uint16_t Addr, uint8_t Reg, uint8_t Size)
*\brief Get Register Value from ISL28022 Power Monitor
*\
*\ Parameter: I2C slave address
*\ 			  Register command
*\ 			  Size Amount of data to be sent
*\ Return: (uint16_t) Get Register value
*\ 		   (uint16_t) 0xFF if I2C bus failed
*/
uint16_t Get_Reg_ISL28022(int fd, uint8_t Reg, uint8_t Size) {

	uint8_t I2C_BUFF[Size];

	if (SLD_PiI2CReadBlock (fd, Reg, I2C_BUFF, Size) < 0) 
		return LOW;
	else {
		if (Size > 1 ) 
			return ( (I2C_BUFF[0] << 8) | I2C_BUFF[1] );
		else 
			return I2C_BUFF[0];
	}	
}





/*___________________________________________________________________________*/
/*!
*\fn uint16_t Set_Reg_ISL28022(int fd, uint16_t Addr, uint8_t Reg, uint8_t Size, uint16_t Reg_Data )
*\brief Set Register Value to ISL28022 Power Monitor
*\
*\ Parameter: I2C slave address
*\ 			  Register command
*\ 			  Register Data
*\ 		      Size Amount of data to be sent
*\ Return: (uint8_t) 0xFF if I2C bus failed
*\
*/

uint16_t Set_Reg_ISL28022(int fd, uint8_t Reg, uint8_t Size, uint16_t Reg_Data ) {

	uint8_t I2C_BUFF[Size];
	U16toArr(Reg_Data, I2C_BUFF);

	if( SLD_PiI2CWriteBlock (fd, Reg, I2C_BUFF, Size) < 0 )
		return LOW;
	else 
		return HIGH;	
}






/*___________________________________________________________________________*/
/*!
*\fn void Init_ISL28022()
*\brief Initialization ISL28022 Power monitoring Sensor
*\brief
*/

/*---------------------------- CONFIGURATION REGISTER -----------------------------

	Voltage Range: 16V
	Shunt Voltage: 40mV
	BADC: 12bits
	SADC: 12bits
	MODE: Shunt and bus, continuous

 ---------------------------- CALIBRATION REGISTER -----------------------------

 *** EQU1 - FULL Scale Current ***
	Vshunt_FS (V) :	0,040
	Rshunt (ohms) :	0,001
	Current_FS (A):	40,00

 *** EQU2 - Current LSB ***
	Current_FS (A):	40,00
	ADC res:		4096
	Current_LSB (A):0,01

 *** EQU3 - CalRegVal ***
	Math_res*Vshunt_LSB:	0,04096
	Current_LSB*Rshunt:	9,7656E-06
	CalRegVal (DEC)	4194
	CalRegVal (HEX)	1062

----------------------------------------------------------------------------------*/

void Init_ISL28022(int fd) {

	Set_Reg_ISL28022(fd, ISL_RG_CFG, SIZE_WORD, 0x0007);
	Set_Reg_ISL28022(fd, ISL_RG_CAL, SIZE_WORD, 0x8312);	//0x1062 ADCres=4096(12bit)

}





/*___________________________________________________________________________*/
/*!
*\fn uint32_t GetAvgVal_ISL28022(int fd, uint8_t AvgSize, uint8_t Reg)
*\brief Get Average Value from ISL28022 Power monitoring Sensor
*\brief
*/
uint32_t GetAvgVal_ISL28022(int fd, uint8_t AvgSize, uint8_t Reg) {

	uint32_t sum = 0;
	uint16_t Val;


	for(uint8_t i=0; i<AvgSize; i++) {
		Val=Get_Reg_ISL28022(fd, Reg, SIZE_WORD);
		sum += Val;
	}
	return sum / AvgSize;

}




/*___________________________________________________________________________*/
/*!
*\fn float Get_ISL_Shunt_Voltage(int fd)
*\brief Get global Shunt Voltage from ISL28022 Power monitoring Sensor
*\brief
*/
float Get_ISL_Shunt_Voltage(int fd) {
	uint16_t Val=GetAvgVal_ISL28022(fd, MOVAVG_SIZE, ISL_RG_SHUNT_VLT);
	return (float)Val*VSHUNT_LSB;
}




/*___________________________________________________________________________*/
/*!
*\fn float Get_ISL_Voltage(int fd)
*\brief Get global Voltage from ISL28022 Power monitoring Sensor
*\brief
*/
float Get_ISL_Voltage(int fd) {
	uint16_t Val=GetAvgVal_ISL28022(fd, MOVAVG_SIZE, ISL_RG_BUS_VLT);
	return (float)(Val>>3)*VBUS_LSB;
}



/*___________________________________________________________________________*/
/*!
*\fn float Get_ISL_Current(int fd)
*\brief Get global current from ISL28022 Power monitoring Sensor
*\brief
*/
float Get_ISL_Current(int fd) {
	uint16_t Val=GetAvgVal_ISL28022(fd, MOVAVG_SIZE, ISL_RG_CURRENT);
	return (float)Val*CURRENT_LSB;
}



/*___________________________________________________________________________*/
/*!
*\fn float Get_ISL_Power(int fd)
*\brief Get global Power from ISL28022 Power monitoring Sensor
*\brief
*/
float Get_ISL_Power(int fd) {
	uint16_t Val=GetAvgVal_ISL28022(fd, MOVAVG_SIZE, ISL_RG_POWER);
	return (float)Val*POWER_LSB*5000;
}
