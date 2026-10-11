#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "EEPROM_interface.h"
#include "DataBase.h"
#include "../../MCAL/I2C/I2C_interface.h"
#include "FreeRTOS.h"
#include "task.h"
  #define F_CPU 8000000UL
#include <util/delay.h>

void EEPROM_voidWriteByte(u8 Copy_u8SlaveAddress,u8 Copy_u8EEPROMAdress, u8 Copy_u8Data)
{
	I2C_u8SendStartCondition();
	I2C_u8SendSlaveAddressWithWrite(Copy_u8SlaveAddress);
	I2C_u8MasterWriteDataByte(Copy_u8EEPROMAdress);
	I2C_u8MasterWriteDataByte(Copy_u8Data);
	I2C_voidSendStopCondition();
	
}

u8 EEPROM_u8ReadByte(u8 Copy_u8SlaveAddress,u8 Copy_u8EEPROMAdress,u8* Copy_puData)
{
	u8 Local_u8ErrorState=1;
	if(Copy_puData!=NULL)
	{
		I2C_u8SendStartCondition();
	    I2C_u8SendSlaveAddressWithWrite(Copy_u8SlaveAddress);
	    I2C_u8MasterWriteDataByte(Copy_u8EEPROMAdress);
	    I2C_u8SendRepeatedStart();
	    I2C_u8SendSlaveAddressWithRead(Copy_u8SlaveAddress);
	    I2C_u8MasterReadDataByte(Copy_puData);
		I2C_voidSendStopCondition();
		Local_u8ErrorState=0;	
	}
	return Local_u8ErrorState;
}

