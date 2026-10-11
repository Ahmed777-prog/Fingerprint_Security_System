#ifndef MCAL_I2C_I2C_INTERFACE_H_
#define MCAL_I2C_I2C_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

void I2C_voidMasterInit(void);
void I2C_voidSlaveInit(u8 Copy_u8SlaveAddress);

u8 I2C_u8SendStartCondition(void);
u8 I2C_u8SendRepeatedStart(void);

u8 I2C_u8SendSlaveAddressWithWrite(u8 Copy_u8SlaveAddress);
u8 I2C_u8SendSlaveAddressWithRead(u8 Copy_u8SlaveAddress);

u8 I2C_u8SlaveListen(void);

u8 I2C_u8MasterWriteDataByte(u8 Copy_u8DataByte);
u8 I2C_u8MasterReadDataByte(u8 *Copy_pu8RxData);

u8 I2C_u8SlaveWriteDataByte(u8 Copy_u8DataByte);
u8 I2C_u8SlaveReadDataByte(u8 *Copy_pu8RxData);

void I2C_voidSendStopCondition(void);

#endif
