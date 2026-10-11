#ifndef HAL_LED_COMPONENT_LED_INTERFACE_H_
#define HAL_LED_COMPONENT_LED_INTERFACE_H_

#define LED_PORTA 0
#define LED_PORTB 1
#define LED_PORTC 2
#define LED_PORTD 3

u8 LED_u8Init(u8 Copy_u8Port,u8 Copy_u8Pin);
u8 LED_u8TurnOn(u8 Copy_u8Port,u8 Copy_u8Pin);
u8 LED_u8TurnOff(u8 Copy_u8Port,u8 Copy_u8Pin);
u8 LED_u8Toggle(u8 Copy_u8Port,u8 Copy_u8Pin);

#endif
