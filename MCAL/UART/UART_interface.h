#ifndef MCAL_UART_UART_INTERFACE_H_
#define MCAL_UART_UART_INTERFACE_H_

/* ================================================================== */
/* =================== Public Functions Prototypes ================== */
/* ================================================================== */

/*
 * Description: Initializes the UART peripheral based on configurations.
 */
void UART_voidInit(void);

/*
 * Description: Transmits a single byte (char) synchronously.
 */
u8 UART_u8TransmitByte(u8 Copy_u8Data);

/*
 * Description: Receives a single byte (char) synchronously.
 */
u8 UART_u8ReceiveByte(void);

/*
 * Description: Transmits a complete string (Array of chars).
 */
void UART_voidSendString(const u8 *Copy_pu8String);

/*
 * Description: Receives a string until the 'Enter' key is pressed.
 */
void UART_voidReceiveString(u8 *Copy_pu8Buffer);

#endif /* MCAL_UART_UART_INTERFACE_H_ */
