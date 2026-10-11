#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "DIO_interface.h"
#include "DIO_private.h"
#include "DIO_config.h"

u8 DIO_u8SetPortDirection(u8 Copy_u8Port,u8 Copy_u8Direction)
{
	u8 Local_u8ErrorState = 0;
	switch(Copy_u8Port)
	{
	 case DIO_PORTA:DDRA=Copy_u8Direction;break;
	 case DIO_PORTB:DDRB=Copy_u8Direction;break;
	 case DIO_PORTC:DDRC=Copy_u8Direction;break;
	 case DIO_PORTD:DDRD=Copy_u8Direction;break;
	 default:Local_u8ErrorState = 1;
	}
	return Local_u8ErrorState;
}

u8 DIO_u8SetPortValue(u8 Copy_u8Port,u8 Copy_u8val)
{
	u8 Local_u8ErrorState = 0;
	switch(Copy_u8Port)
	{
	 case DIO_PORTA:PORTA=Copy_u8val;break;
	 case DIO_PORTB:PORTB=Copy_u8val;break;
	 case DIO_PORTC:PORTC=Copy_u8val;break;
	 case DIO_PORTD:PORTD=Copy_u8val;break;
	 default:Local_u8ErrorState = 1;
	}
	return Local_u8ErrorState;
}

u8 DIO_u8SetPinDirection(u8 Copy_u8Port,u8 Copy_u8Pin,u8 Copy_u8Direction)
{
   u8 Local_u8ErrorState = 0;
   if(Copy_u8Pin<=DIO_PIN7)
   {
	  if(Copy_u8Direction==DIO_INPUT)
	  {
		  switch(Copy_u8Port)
		  {
		   case DIO_PORTA:CLR_BIT(DDRA,Copy_u8Pin);break;
		   case DIO_PORTB:CLR_BIT(DDRB,Copy_u8Pin);break;
		   case DIO_PORTC:CLR_BIT(DDRC,Copy_u8Pin);break;
		   case DIO_PORTD:CLR_BIT(DDRD,Copy_u8Pin);break;
		   default:Local_u8ErrorState = 1;
		  }
	  }
	  else if(Copy_u8Direction==DIO_OUTPUT)
	  {
		  switch(Copy_u8Port)
		  {
		   case DIO_PORTA:SET_BIT(DDRA,Copy_u8Pin);break;
		   case DIO_PORTB:SET_BIT(DDRB,Copy_u8Pin);break;
		   case DIO_PORTC:SET_BIT(DDRC,Copy_u8Pin);break;
		   case DIO_PORTD:SET_BIT(DDRD,Copy_u8Pin);break;
		   default:Local_u8ErrorState = 1;
		  }
	  }
	  else
	  {
		  Local_u8ErrorState = 1;
	  }

   }
   else
   {
	   Local_u8ErrorState = 1;
   }

   return Local_u8ErrorState;
}

u8 DIO_u8SetPinValue(u8 Copy_u8Port,u8 Copy_u8Pin,u8 Copy_u8val)
{
   u8 Local_u8ErrorState = 0;
   if(Copy_u8Pin<=DIO_PIN7)
   {
	  if(Copy_u8val==DIO_LOW)
	  {
		  switch(Copy_u8Port)
		  {
		   case DIO_PORTA:CLR_BIT(PORTA,Copy_u8Pin);break;
		   case DIO_PORTB:CLR_BIT(PORTB,Copy_u8Pin);break;
		   case DIO_PORTC:CLR_BIT(PORTC,Copy_u8Pin);break;
		   case DIO_PORTD:CLR_BIT(PORTD,Copy_u8Pin);break;
		   default:Local_u8ErrorState = 1;
		  }
	  }
	  else if(Copy_u8val==DIO_HIGH)
	  {
		  switch(Copy_u8Port)
		  {
		   case DIO_PORTA:SET_BIT(PORTA,Copy_u8Pin);break;
		   case DIO_PORTB:SET_BIT(PORTB,Copy_u8Pin);break;
		   case DIO_PORTC:SET_BIT(PORTC,Copy_u8Pin);break;
		   case DIO_PORTD:SET_BIT(PORTD,Copy_u8Pin);break;
		   default:Local_u8ErrorState = 1;
		  }
	  }
	  else
	  {
		  Local_u8ErrorState = 1;
	  }

   }
   else
   {
	   Local_u8ErrorState = 1;
   }

   return Local_u8ErrorState;
}

u8 DIO_u8GetPinValue(u8 Copy_u8Port,u8 Copy_u8Pin,u8 *Pu8Pin_val)
{
	u8 Local_u8ErrorState = 0;
	if ((Pu8Pin_val != 0)&&(Copy_u8Pin <= DIO_PIN7))
	{
		switch (Copy_u8Port)
		{
		 case DIO_PORTA:*Pu8Pin_val=GET_BIT(PINA, Copy_u8Pin);break;
		 case DIO_PORTB:*Pu8Pin_val=GET_BIT(PINB, Copy_u8Pin);break;
		 case DIO_PORTC:*Pu8Pin_val=GET_BIT(PINC, Copy_u8Pin);break;
		 case DIO_PORTD:*Pu8Pin_val=GET_BIT(PIND, Copy_u8Pin);break;
		 default:Local_u8ErrorState = 1;
		}
	}
	else
	{
		Local_u8ErrorState = 1;
	}

	return Local_u8ErrorState;
}

u8 DIO_u8GetPortValue(u8 Copy_u8Port,u8 *Pu8Port_val)
{
	u8 Local_u8ErrorState = 0;
	if (Pu8Port_val != 0)
	{
		switch (Copy_u8Port)
		{
		 case DIO_PORTA:*Pu8Port_val=PINA;break;
		 case DIO_PORTB:*Pu8Port_val=PINB;break;
		 case DIO_PORTC:*Pu8Port_val=PINC;break;
		 case DIO_PORTD:*Pu8Port_val=PIND;break;
		 default:Local_u8ErrorState = 1;
		}
	}
	else
	{
		Local_u8ErrorState = 1;
	}

	return Local_u8ErrorState;
}

u8 DIO_u8TogglePinValue(u8 Copy_u8Port,u8 Copy_u8Pin)
{
	u8 Local_u8ErrorState = 0;
	if(Copy_u8Pin<=7)
	{
		switch(Copy_u8Port)
		{
		 case DIO_PORTA:TOG_BIT(PORTA,Copy_u8Pin);break;
		 case DIO_PORTB:TOG_BIT(PORTB,Copy_u8Pin);break;
		 case DIO_PORTC:TOG_BIT(PORTC,Copy_u8Pin);break;
		 case DIO_PORTD:TOG_BIT(PORTD,Copy_u8Pin);break;
		 default:Local_u8ErrorState = 1;
		}
	}
	else
    {
    	Local_u8ErrorState = 1;
    }

	return Local_u8ErrorState;
}
