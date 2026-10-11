#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "UART_private.h"
#include "UART_config.h"
#include "UART_interface.h"



/* ================================================================== */
/* =================== Public Functions Implementation ============== */
/* ================================================================== */

void UART_voidInit(void)
{
    /* 1. Set Baud Rate (UBRR) */
    /* Write to UBRRL first, then UBRRH with URSEL = 0 */
    UBRRL = (u8)UART_UBRR_VALUE;
    UBRRH = (0 << UBRRH_URSEL) | (u8)(UART_UBRR_VALUE >> 8);

    /* 2. Set Speed Mode */
    UCSRA = UART_SPEED_MODE;

    /* 3. Set Frame Format (Data size, Parity, Stop bits) */
    /* Note: URSEL must be 1 when writing to UCSRC */
    UCSRC = (1 << UCSRC_URSEL) | UART_MODE | UART_PARITY | UART_STOP_BITS | UART_DATA_SIZE;

    UCSRB |= (UART_TX_STATE | UART_RX_STATE);

    /* Note: Transmitter and Receiver are enabled via UART_voidEnable() */
}


u8 UART_u8TransmitByte(u8 Copy_u8Data)
{
    u32 Local_u32TimeoutCounter = 0;
    /* Wait (Poll) until UDR register is empty (UDRE flag is 1) */
    while(GET_BIT(UCSRA, UCSRA_UDRE) == 0)
    {
		Local_u32TimeoutCounter++;
        if(Local_u32TimeoutCounter == I2C_TIMEOUT_MAX)
        {
            return 0; 
        }
	}

    /* Put the data into UDR to start hardware transmission */
    UDR = Copy_u8Data;
    return 0;
}

u8 UART_u8ReceiveByte(void)
{
  
    /* Wait (Poll) until unread data is present in UDR (RXC flag is 1) */
    while(GET_BIT(UCSRA, UCSRA_RXC) == 0);

    /* Return the received byte from UDR */
    return UDR;
}

void UART_voidSendString(const u8 *Copy_pu8String)
{
    u8 Local_u8Counter = 0;

    /* Loop until the Null Terminator '\0' is found */
    while(Copy_pu8String[Local_u8Counter] != '\0')
    {
        UART_u8TransmitByte(Copy_pu8String[Local_u8Counter]);
        Local_u8Counter++;
    }
}

void UART_voidReceiveString(u8 *Copy_pu8Buffer)
{
    u8 Local_u8Counter = 0;
    u8 Local_u8ReceivedData;

    while(1)
    {
        /* Receive byte by byte */
        Local_u8ReceivedData = UART_u8ReceiveByte();

        /* Check if 'Enter' key is pressed (\r or \n depending on the terminal) */
        if(Local_u8ReceivedData == '\r' || Local_u8ReceivedData == '\n')//NOTE
        {
            Copy_pu8Buffer[Local_u8Counter] = '\0'; /* Terminate the string properly */
            break; /* Exit the loop */
        }

        /* Store the received char in the buffer and increment counter */
        Copy_pu8Buffer[Local_u8Counter] = Local_u8ReceivedData;
        Local_u8Counter++;
    }
}
