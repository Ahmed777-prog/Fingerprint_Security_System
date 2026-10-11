#ifndef HAL_EEPROM_EEPROM_INTERFACE_H_
#define HAL_EEPROM_EEPROM_INTERFACE_H_

void EEPROM_voidWriteByte(u8 Copy_u8SlaveAddress,u8 Copy_u8EEPROMAdress, u8 Copy_u8Data);
u8 EEPROM_u8ReadByte(u8 Copy_u8SlaveAddress,u8 Copy_u8EEPROMAdress,u8* Copy_puData);


#endif
