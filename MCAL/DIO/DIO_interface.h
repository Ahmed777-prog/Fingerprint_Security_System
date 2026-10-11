#ifndef MCAL_DIO_DIO_INTERFACE_H_
#define MCAL_DIO_DIO_INTERFACE_H_

#define DIO_PORTA  0
#define DIO_PORTB  1
#define DIO_PORTC  2
#define DIO_PORTD  3

#define DIO_PIN0   0
#define DIO_PIN1   1
#define DIO_PIN2   2
#define DIO_PIN3   3
#define DIO_PIN4   4
#define DIO_PIN5   5
#define DIO_PIN6   6
#define DIO_PIN7   7

#define DIO_INPUT  0
#define DIO_OUTPUT 1

#define DIO_LOW    0
#define DIO_HIGH   1

u8 DIO_u8SetPortDirection(u8 Copy_u8Port,u8 Copy_u8Direction);
u8 DIO_u8SetPortValue(u8 Copy_u8Port,u8 Copy_u8val);
u8 DIO_u8SetPinDirection(u8 Copy_u8Port,u8 Copy_u8Pin,u8 Copy_u8Direction);
u8 DIO_u8SetPinValue(u8 Copy_u8Port,u8 Copy_u8Pin,u8 Copy_u8val);
u8 DIO_u8GetPinValue(u8 Copy_u8Port,u8 Copy_u8Pin,u8 *Pu8Pin_val);
u8 DIO_u8GetPortValue(u8 Copy_u8Port,u8 *Pu8Port_val);
u8 DIO_u8TogglePinValue(u8 Copy_u8Port,u8 Copy_u8Pin);
#endif
