#ifndef HAL_KPD_KPD_CONFIG_H_
#define HAL_KPD_KPD_CONFIG_H_


#define KPD_COL_PORT     DIO_PORTC

#define KPD_COL0         DIO_PIN4  // C1
#define KPD_COL1         DIO_PIN5  // C2
#define KPD_COL2         DIO_PIN6  // C3
#define KPD_COL3         DIO_PIN7  // C4


#define KPD_ROW0_PORT    DIO_PORTC // A1
#define KPD_ROW0_PIN     DIO_PIN3

#define KPD_ROW1_PORT    DIO_PORTC // A2
#define KPD_ROW1_PIN     DIO_PIN2

#define KPD_ROW2_PORT    DIO_PORTD // A3
#define KPD_ROW2_PIN     DIO_PIN2

#define KPD_ROW3_PORT    DIO_PORTD // A4
#define KPD_ROW3_PIN     DIO_PIN3


#define KPD_NO_PRESSED_KEY 255

#endif
