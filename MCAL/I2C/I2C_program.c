#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "I2C_private.h"
#include "I2C_config.h"
#include "I2C_interface.h"

void I2C_voidMasterInit()
{
	TWSR &=0xfc;
	TWSR |=I2C_PRESCALER_VALUE;
	TWBR =I2C_BIT_RATE_VALUE;
	TWAR |=(I2C_NODE_ADDRESS<<1)|(I2C_GENERAL_CALL_STATE);
	TWCR =(1<<TWCR_TWEN);
}

u8 I2C_u8SendStartCondition()
{
	u32 Local_u32TimeoutCounter = 0;
	TWCR =(1<<TWCR_TWSTA)|(1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	while(GET_BIT(TWCR,TWCR_TWINT)==0)
	{
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}
	if((TWSR & TWI_STATUS_MASK) ==TWI_START_CONDITION_ACK)
	{
		return 1;
	}
	return 0;
}

u8 I2C_u8SendRepeatedStart()
{
	u32 Local_u32TimeoutCounter = 0;
	TWCR =(1<<TWCR_TWSTA)|(1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	while(GET_BIT(TWCR,TWCR_TWINT)==0)
	{
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}
		if((TWSR & TWI_STATUS_MASK) ==TWI_REP_START_CONDITION_ACK)
		{
			return 1;
		}
		return 0;
}

u8 I2C_u8SendSlaveAddressWithWrite(u8 Copy_u8SlaveAddress)
{
	u32 Local_u32TimeoutCounter = 0;
	TWDR=(Copy_u8SlaveAddress<<1);
	TWDR &=~(1);
	TWCR =(1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	while(GET_BIT(TWCR,TWCR_TWINT)==0)
	{
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}
		if((TWSR & TWI_STATUS_MASK) ==TWI_SLAVE_ADDRESS_WRITE_ACK)
		{
			return 1;
		}
		return 0;

}

u8 I2C_u8SendSlaveAddressWithRead(u8 Copy_u8SlaveAddress)
{
	u32 Local_u32TimeoutCounter = 0;
	TWDR=(Copy_u8SlaveAddress<<1);
	TWDR |=(1);
	TWCR =(1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	while(GET_BIT(TWCR,TWCR_TWINT)==0)
	{
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}
		if((TWSR & TWI_STATUS_MASK) ==TWI_SLAVE_ADDRESS_READ_ACK)
		{
			return 1;
		}
		return 0;

}

u8 I2C_u8MasterWriteDataByte(u8 Copy_u8DataByte)
{
	u32 Local_u32TimeoutCounter = 0;
	TWDR=(Copy_u8DataByte);
	TWCR =(1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	while(GET_BIT(TWCR,TWCR_TWINT)==0)
	{
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}
	     if((TWSR & TWI_STATUS_MASK) ==TWI_MASTER_DATA_TRANSMIT_ACK)
		{
			return 1;
		}
		return 0;
}

u8 I2C_u8MasterReadDataByte(u8 *Copy_pu8RxData)
{
	u32 Local_u32TimeoutCounter = 0;
   if(Copy_pu8RxData!=NULL)
   {
	   TWCR = (1<<TWCR_TWINT) | (1<<TWCR_TWEN);
	   while(GET_BIT(TWCR,TWCR_TWINT)==0)
	   {
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	   }
	   if((TWSR & TWI_STATUS_MASK) == 0x58)
	   {
		  *Copy_pu8RxData=TWDR;
		  return 1;
	   }
   }
   return 0;
}

void I2C_voidSendStopCondition()
{
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWSTO) | (1 << TWCR_TWEN);
}

void I2C_voidSlaveInit(u8 Copy_u8SlaveAddress)
{
	TWAR = (Copy_u8SlaveAddress << 1) | (I2C_GENERAL_CALL_STATE);
	TWCR = (1 << TWCR_TWEN) | (1 << TWCR_TWEA);
}

u8 I2C_u8SlaveWriteDataByte(u8 Copy_u8DataByte)
{
	u32 Local_u32TimeoutCounter = 0;
    TWDR = Copy_u8DataByte;
    TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN) | (1 << TWCR_TWEA);
    while(GET_BIT(TWCR, TWCR_TWINT) == 0)
	{
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}

    return 1;
}

u8 I2C_u8SlaveListen(void)
{
	u32 Local_u32TimeoutCounter = 0;
	TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN) | (1 << TWCR_TWEA);
	while(GET_BIT(TWCR, TWCR_TWINT) == 0)
	{
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}
	return (TWSR & TWI_STATUS_MASK);
}

u8 I2C_u8SlaveReadDataByte(u8 *Copy_pu8RxData)
{
	u32 Local_u32TimeoutCounter = 0;
    if(Copy_pu8RxData != 0)
    {
        /* Clear TWINT, Enable TWI, Enable ACK to receive */
        TWCR = (1 << TWCR_TWINT) | (1 << TWCR_TWEN) | (1 << TWCR_TWEA);

        /* Wait until TWINT flag is raised */
        while(GET_BIT(TWCR, TWCR_TWINT) == 0)
		{
			Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
		}

        /* Read Data from TWDR */
        *Copy_pu8RxData = TWDR;
        return 1;
    }
    return 0;
}
