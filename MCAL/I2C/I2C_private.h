#ifndef MCAL_I2C_I2C_PRIVATE_H_
#define MCAL_I2C_I2C_PRIVATE_H_

#define TWBR    *((volatile u8*)0x20)

#define TWCR    *((volatile u8*)0x56)
#define TWCR_TWINT      7
#define TWCR_TWEA       6
#define TWCR_TWSTA      5
#define TWCR_TWSTO      4
#define TWCR_TWWC       3
#define TWCR_TWEN       2
#define TWCR_TWIE       0

#define TWSR    *((volatile u8*)0x21)
#define TWSR_TWPS1      1
#define TWSR_TWPS0      0

#define TWDR    *((volatile u8*)0x23)

#define TWAR    *((volatile u8*)0x22)
#define TWAR_TWGCE      0

#define TWI_START_CONDITION_ACK         0x08 /* Start condition transmitted */
#define TWI_REP_START_CONDITION_ACK     0x10 /* Repeated start condition transmitted */
#define TWI_SLAVE_ADDRESS_WRITE_ACK     0x18 /* SLA+W transmitted, ACK received */
#define TWI_SLAVE_ADDRESS_READ_ACK      0x40 /* SLA+R transmitted, ACK received */
#define TWI_MASTER_DATA_TRANSMIT_ACK    0x28 /* Data transmitted, ACK received */
#define TWI_MASTER_DATA_RECEIVE_ACK     0x50 /* Data received, ACK returned */
#define TWI_MASTER_DATA_RECEIVE_NACK    0x58 /* Data received, NACK returned */

/* Slave Status Codes */
#define TWI_SLAVE_ADD_RCVD_WR_REQ    0x60 /* Own SLA+W received, ACK returned */
#define TWI_SLAVE_ADD_RCVD_RD_REQ    0xA8 /* Own SLA+R received, ACK returned */

#define TWI_STATUS_MASK         0xF8

#define TWI_PRESCALER_1         0
#define TWI_PRESCALER_4         1
#define TWI_PRESCALER_16        2
#define TWI_PRESCALER_64        3

#endif
