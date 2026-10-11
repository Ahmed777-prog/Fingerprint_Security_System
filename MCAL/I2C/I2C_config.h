#ifndef MCAL_I2C_I2C_CONFIG_H_
#define MCAL_I2C_I2C_CONFIG_H_

/*
 * Choose the Prescaler value:
 * Options:
 * 1- TWI_PRESCALER_1
 * 2- TWI_PRESCALER_4
 * 3- TWI_PRESCALER_16
 * 4- TWI_PRESCALER_64
 */
#define I2C_PRESCALER_VALUE         TWI_PRESCALER_1

/*
 * Set the Bit Rate Register (TWBR) value
 * Example: To get 400kHz with 8MHz CPU and Prescaler 1:
 * TWBR = ((8000000 / 400000) - 16) / (2 * 1) = 2
 */
#define I2C_BIT_RATE_VALUE          2

/*
 * Set the I2C Node Address (For Slave Mode)
 * Note: Range from 1 to 119 (Don't use reserved addresses)
 * We shifted it by 1 because the 7-bit address sits in the upper 7 bits of TWAR
 */
#define I2C_NODE_ADDRESS            0x50

/*
 * Enable or Disable General Call Recognition
 * Options:
 * 0 -> Disable (No recognition)
 * 1 -> Enable  (Will respond to 0x00)
 */
#define I2C_GENERAL_CALL_STATE      0

#define I2C_TIMEOUT_MAX   50000UL

#endif
