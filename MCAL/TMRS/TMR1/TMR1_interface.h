#ifndef MCAL_TMRS_TMR1_TMR1_INTERFACE_H_
#define MCAL_TMRS_TMR1_TMR1_INTERFACE_H_

#define TMR1_CHANNEL_A          1
#define TMR1_CHANNEL_B          2

#define TMR1_ICU_FALLING_EDGE       0
#define TMR1_ICU_RISING_EDGE        1

void TMR1_voidInit(void);

void TMR1_voidSetPreloadValue(u16 Copy_u16Preload);

void TMR1_voidSetCompareMatchValue(u16 Copy_u16CompareValue, u8 Copy_u8Channel);

void TMR1_voidSetICRValue(u16 Copy_u16ICRValue);

u16 TMR1_u16GetTimerValue(void);

void TMR1_voidSetInputCaptureEdge(u8 Copy_u8Edge);

u16 TMR1_u16GetInputCaptureValue(void);

void TMR1_voidClearInputCaptureFlag(void);

#endif
