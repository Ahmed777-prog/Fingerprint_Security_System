#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "LED_interface.h"
#include "LED_config.h"

u8 LED_u8Init(u8 Copy_u8Port,u8 Copy_u8Pin)
{
	u8 Local_u8ErrorState = 0;
	Local_u8ErrorState=DIO_u8SetPinDirection(Copy_u8Port,Copy_u8Pin,DIO_OUTPUT);
	return Local_u8ErrorState;
}

u8 LED_u8TurnOn(u8 Copy_u8Port,u8 Copy_u8Pin)
{
	u8 Local_u8ErrorState = 0;
	Local_u8ErrorState=DIO_u8SetPinValue(Copy_u8Port,Copy_u8Pin,DIO_HIGH);
	return Local_u8ErrorState;
}

u8 LED_u8TurnOff(u8 Copy_u8Port,u8 Copy_u8Pin)
{
	u8 Local_u8ErrorState = 0;
	Local_u8ErrorState=DIO_u8SetPinValue(Copy_u8Port,Copy_u8Pin,DIO_LOW);
	return Local_u8ErrorState;
}

u8 LED_u8Toggle(u8 Copy_u8Port,u8 Copy_u8Pin)
{
	u8 Local_u8ErrorState = 0;
	u8 Local_u8PinState = 0;
	Local_u8ErrorState=DIO_u8GetPinValue(Copy_u8Port,Copy_u8Pin,&Local_u8PinState);
	if(Local_u8ErrorState==0)
	{
		if(Local_u8PinState==DIO_HIGH)
		{
			Local_u8ErrorState=DIO_u8SetPinValue(Copy_u8Port,Copy_u8Pin,DIO_LOW);
		}
		else if(Local_u8PinState==DIO_LOW)
		{
			Local_u8ErrorState=DIO_u8SetPinValue(Copy_u8Port,Copy_u8Pin,DIO_HIGH);
		}
	}
	return Local_u8ErrorState;
}
