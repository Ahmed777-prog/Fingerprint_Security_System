#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "../../MCAL/TMRS/TMR1/TMR1_interface.h"
#include "SERVO_interface.h"


void Servo_voidSetAngle(u8 Copy_u8Angle)
{
    u16 Local_u16Ticks=0;

    Local_u16Ticks = (((u32)Copy_u8Angle *1000UL ) / 180UL) + 1000UL;
    TMR1_voidSetCompareMatchValue(Local_u16Ticks,TMR1_CHANNEL_A );


}

