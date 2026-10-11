#ifndef MCAL_TMRS_TMR1_TMR1_PRIVATE_H_
#define MCAL_TMRS_TMR1_TMR1_PRIVATE_H_

#define TCCR1A         *((volatile u8*)0x4F)

#define TCCR1A_COM1A1  7
#define TCCR1A_COM1A0  6
#define TCCR1A_COM1B1  5
#define TCCR1A_COM1B0  4
#define TCCR1A_FOC1A   3
#define TCCR1A_FOC1B   2
#define TCCR1A_WGM11   1
#define TCCR1A_WGM10   0


#define TCCR1B         *((volatile u8*)0x4E)

#define TCCR1B_ICNC1   7
#define TCCR1B_ICES1   6

#define TCCR1B_WGM13   4
#define TCCR1B_WGM12   3
#define TCCR1B_CS12    2
#define TCCR1B_CS11    1
#define TCCR1B_CS10    0


#define TCNT1          *((volatile u16*)0x4C)

#define OCR1A          *((volatile u16*)0x4A)

#define OCR1B          *((volatile u16*)0x48)

#define ICR1           *((volatile u16*)0x46)


#define TIMSK          *((volatile u8*)0x59)

#define TIMSK_TICIE1   5
#define TIMSK_OCIE1A   4
#define TIMSK_OCIE1B   3
#define TIMSK_TOIE1    2

#define TIFR           *((volatile u8*)0x58)

#define TIFR_ICF1      5
#define TIFR_OCF1A     4
#define TIFR_OCF1B     3
#define TIFR_TOV1      2

/* ================== Timer1 Compare Output Modes =================== */
#define TMR1_OC_DISCONNECTED                        0
#define TMR1_OC_TOGGLE                              1
#define TMR1_OC_CLEAR_NON_INVERTING                 2
#define TMR1_OC_SET_INVERTING                       3

/* ================== Timer1 Waveform Generation Modes ============== */
#define TMR1_NORMAL_MODE                            0
#define TMR1_PWM_PHASE_CORRECT_8BIT_MODE            1
#define TMR1_PWM_PHASE_CORRECT_9BIT_MODE            2
#define TMR1_PWM_PHASE_CORRECT_10BIT_MODE           3
#define TMR1_CTC_OCR1A_MODE                         4
#define TMR1_FAST_PWM_8BIT_MODE                     5
#define TMR1_FAST_PWM_9BIT_MODE                     6
#define TMR1_FAST_PWM_10BIT_MODE                    7
#define TMR1_PWM_PHASE_FREQ_CORRECT_ICR1_MODE       8
#define TMR1_PWM_PHASE_FREQ_CORRECT_OCR1A_MODE      9
#define TMR1_PWM_PHASE_CORRECT_ICR1_MODE            10
#define TMR1_PWM_PHASE_CORRECT_OCR1A_MODE           11
#define TMR1_CTC_ICR1_MODE                          12
/* Mode 13 is Reserved */
#define TMR1_FAST_PWM_ICR1_MODE                     14
#define TMR1_FAST_PWM_OCR1A_MODE                    15

/* ================== Timer1 Clock Select (Prescaler) =============== */
#define TMR1_NO_CLOCK                               0
#define TMR1_PRESCALER_1                            1
#define TMR1_PRESCALER_8                            2
#define TMR1_PRESCALER_64                           3
#define TMR1_PRESCALER_256                          4
#define TMR1_PRESCALER_1024                         5
#define TMR1_EXT_CLOCK_FALLING_EDGE                 6
#define TMR1_EXT_CLOCK_RISING_EDGE                  7

#endif
