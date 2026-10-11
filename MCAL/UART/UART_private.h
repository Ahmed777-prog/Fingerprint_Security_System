#ifndef MCAL_UART_UART_PRIVATE_H_
#define MCAL_UART_UART_PRIVATE_H_

#define UDR         *((volatile u8*)0x2C)
#define UCSRA       *((volatile u8*)0x2B)
#define UCSRB       *((volatile u8*)0x2A)
#define UCSRC       *((volatile u8*)0x40)
#define UBRRH       *((volatile u8*)0x40)
#define UBRRL       *((volatile u8*)0x29)

#define UCSRA_RXC       7
#define UCSRA_TXC       6
#define UCSRA_UDRE      5
#define UCSRA_FE        4
#define UCSRA_DOR       3
#define UCSRA_PE        2
#define UCSRA_U2X       1
#define UCSRA_MPCM      0

#define UCSRB_RXCIE     7
#define UCSRB_TXCIE     6
#define UCSRB_UDRIE     5
#define UCSRB_RXEN      4
#define UCSRB_TXEN      3
#define UCSRB_UCSZ2     2
#define UCSRB_RXB8      1
#define UCSRB_TXB8      0

#define UCSRC_URSEL     7
#define UCSRC_UMSEL     6
#define UCSRC_UPM1      5
#define UCSRC_UPM0      4
#define UCSRC_USBS      3
#define UCSRC_UCSZ1     2
#define UCSRC_UCSZ0     1
#define UCSRC_UCPOL     0

#define UBRRH_URSEL     7

#define UART_ASYNC_MODE             (0 << 6)
#define UART_SYNC_MODE              (1 << 6)

#define UART_PARITY_DISABLED        (0 << 4)
#define UART_PARITY_EVEN            (2 << 4)
#define UART_PARITY_ODD             (3 << 4)

#define UART_STOP_1_BIT             (0 << 3)
#define UART_STOP_2_BIT             (1 << 3)

#define UART_DATA_5_BIT             (0 << 1)
#define UART_DATA_6_BIT             (1 << 1)
#define UART_DATA_7_BIT             (2 << 1)
#define UART_DATA_8_BIT             (3 << 1)

#define UART_NORMAL_SPEED           (0 << 1)
#define UART_DOUBLE_SPEED           (1 << 1)

#define UART_MPCM_DISABLED          (0 << 0)
#define UART_MPCM_ENABLED           (1 << 0)

#define UART_RX_DISABLE             (0 << 4)
#define UART_RX_ENABLE              (1 << 4)

#define UART_TX_DISABLE             (0 << 3)
#define UART_TX_ENABLE              (1 << 3)

#endif
