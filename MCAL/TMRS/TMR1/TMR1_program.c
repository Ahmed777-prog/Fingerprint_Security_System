#include "../../../LIB/STD_TYPES.h"
#include "../../../LIB/BIT_MATH.h"
#include "TMR1_private.h"
#include "TMR1_config.h"
#include "TMR1_interface.h"

void TMR1_voidInit(void)
{
    /* 1. تصفير الريجيسترات تماماً */
    TCCR1A = 0;
    TCCR1B = 0;

    /* 3. ضبط وضع Fast PWM (Mode 14) حيث الـ TOP هو ICR1 */
    TCCR1A |= (1 << 1);                  // WGM11 = 1
    TCCR1B |= (1 << 3) | (1 << 4);       // WGM12 = 1, WGM13 = 1

    /* 4. تفعيل قناة A (OC1A) بنظام Non-Inverting */
    TCCR1A |= (1 << 7);                  // COM1A1 = 1

    /* 5. ضبط الـ Prescaler على 8 */
    TCCR1B |= (1 << 1);                  // CS11 = 1

    /* 6. ضبط الـ Period ليكون 20ms بالظبط (تردد 50Hz) */
    ICR1 = 19999;
}

void TMR1_voidSetPreloadValue(u16 Copy_u16Preload)
{
	TCNT1=Copy_u16Preload;

}

void TMR1_voidSetCompareMatchValue(u16 Copy_u16CompareValue, u8 Copy_u8Channel)
{
	if(Copy_u8Channel==TMR1_CHANNEL_A)
	{
		OCR1A=Copy_u16CompareValue;
	}
	else if(Copy_u8Channel==TMR1_CHANNEL_B)
	{
		OCR1B=Copy_u16CompareValue;
	}
}

void TMR1_voidSetICRValue(u16 Copy_u16ICRValue)
{
	ICR1=Copy_u16ICRValue;
}

u16 TMR1_u16GetTimerValue(void)
{
	return TCNT1;
}

void TMR1_voidSetInputCaptureEdge(u8 Copy_u8Edge)
{
    if(Copy_u8Edge == TMR1_ICU_RISING_EDGE)
    {
        SET_BIT(TCCR1B, TCCR1B_ICES1);
    }
    else if(Copy_u8Edge == TMR1_ICU_FALLING_EDGE)
    {
        CLR_BIT(TCCR1B, TCCR1B_ICES1);
    }
}

u16 TMR1_u16GetInputCaptureValue(void)
{
    return ICR1;
}

void TMR1_voidClearInputCaptureFlag(void)
{
    SET_BIT(TIFR, 5);
}
