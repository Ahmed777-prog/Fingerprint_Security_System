#ifndef MCAL_UART_UART_CONFIG_H_
#define MCAL_UART_UART_CONFIG_H_

/* Options:
 * UART_ASYNC_MODE,
 * UART_SYNC_MODE */
#define UART_MODE                   UART_ASYNC_MODE

/* Options:
 *  UART_PARITY_DISABLED
 *  UART_PARITY_EVEN
 *  UART_PARITY_ODD */
#define UART_PARITY                 UART_PARITY_DISABLED

/* Options:
 *  UART_STOP_1_BIT
 * UART_STOP_2_BIT */
#define UART_STOP_BITS              UART_STOP_1_BIT

/* Options:
 * UART_DATA_5_BIT,
 * UART_DATA_6_BIT,
 * UART_DATA_7_BIT,
 * UART_DATA_8_BIT */
#define UART_DATA_SIZE              UART_DATA_8_BIT

/* Options:
 * UART_NORMAL_SPEED,
 * UART_DOUBLE_SPEED */
#define UART_SPEED_MODE             UART_DOUBLE_SPEED

/* Options:
 * UART_TX_DISABLE,
 * UART_TX_ENABLE */
#define UART_TX_STATE               UART_TX_ENABLE

/* Options:
 * UART_RX_DISABLE,
 * UART_RX_ENABLE */
#define UART_RX_STATE               UART_RX_ENABLE

/* Value from Datasheet based on F_CPU & Baud Rate (e.g., 51 for 8MHz & 9600 bps) */
#define UART_UBRR_VALUE             16

#define I2C_TIMEOUT_MAX   50000UL

#endif /* MCAL_UART_UART_CONFIG_H_ */
