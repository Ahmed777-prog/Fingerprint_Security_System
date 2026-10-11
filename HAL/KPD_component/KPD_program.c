#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "KPD_config.h"
#include "KPD_interface.h"
#define F_CPU 8000000UL
#include <util/delay.h>



u8 KPD_u8GetPressedKey(void)
{
    u8 pressedKey = KPD_NO_PRESSED_KEY;
    u8 rowValue = DIO_HIGH; 

    /* الأعمدة (كلها على PORTC) */
    u8 colArr[] = {KPD_COL0, KPD_COL1, KPD_COL2, KPD_COL3};
    
    /* الصفوف (متوزعة على PORTC و PORTD) */
    u8 rowPortArr[] = {KPD_ROW0_PORT, KPD_ROW1_PORT, KPD_ROW2_PORT, KPD_ROW3_PORT};
    u8 rowPinArr[]  = {KPD_ROW0_PIN,  KPD_ROW1_PIN,  KPD_ROW2_PIN,  KPD_ROW3_PIN};

    u8 switchArr[4][4] = {
        {'7', '8', '9', 'A'}, 
        {'4', '5', '6', 'B'},
        {'1', '2', '3', 'C'},
        {'*', '0', '=', 'D'}  
    };

    for(u8 COL_Index = 0; COL_Index < 4; COL_Index++)
    {
        /* تفعيل العمود الحالي (LOW) */
        DIO_u8SetPinValue(KPD_COL_PORT, colArr[COL_Index], DIO_LOW);

        for(u8 ROW_Index = 0; ROW_Index < 4; ROW_Index++)
        {
            /* قراءة الصف الحالي بناءً على الـ Port المخصص له */
            DIO_u8GetPinValue(rowPortArr[ROW_Index], rowPinArr[ROW_Index], &rowValue);

            if(rowValue == DIO_LOW)
            {
                pressedKey = switchArr[ROW_Index][COL_Index];
                DIO_u8SetPinValue(KPD_COL_PORT, colArr[COL_Index], DIO_HIGH);
                return pressedKey;
            }
        }
        
        DIO_u8SetPinValue(KPD_COL_PORT, colArr[COL_Index], DIO_HIGH);
    }

    return pressedKey;
}

void KPD_voidInit()
{
/* 1. تهيئة الأعمدة C1..C4 كـ Output وتخريج HIGH (بناءً على التوزيعة الجديدة) */
    DIO_u8SetPinDirection(DIO_PORTC, DIO_PIN4, DIO_OUTPUT);
    DIO_u8SetPinDirection(DIO_PORTC, DIO_PIN5, DIO_OUTPUT);
    DIO_u8SetPinDirection(DIO_PORTC, DIO_PIN6, DIO_OUTPUT);
    DIO_u8SetPinDirection(DIO_PORTC, DIO_PIN7, DIO_OUTPUT);

    DIO_u8SetPinValue(DIO_PORTC, DIO_PIN4, DIO_HIGH);
    DIO_u8SetPinValue(DIO_PORTC, DIO_PIN5, DIO_HIGH);
    DIO_u8SetPinValue(DIO_PORTC, DIO_PIN6, DIO_HIGH);
    DIO_u8SetPinValue(DIO_PORTC, DIO_PIN7, DIO_HIGH);

    /* 2. تهيئة الصفوف A1..A4 كـ Input وتفعيل الـ Pull-up resistor */
    DIO_u8SetPinDirection(DIO_PORTC, DIO_PIN3, DIO_INPUT); // A1
    DIO_u8SetPinDirection(DIO_PORTC, DIO_PIN2, DIO_INPUT); // A2
    DIO_u8SetPinDirection(DIO_PORTD, DIO_PIN2, DIO_INPUT); // A3
    DIO_u8SetPinDirection(DIO_PORTD, DIO_PIN3, DIO_INPUT); // A4

    DIO_u8SetPinValue(DIO_PORTC, DIO_PIN3, DIO_HIGH); // Pull-up A1
    DIO_u8SetPinValue(DIO_PORTC, DIO_PIN2, DIO_HIGH); // Pull-up A2
    DIO_u8SetPinValue(DIO_PORTD, DIO_PIN2, DIO_HIGH); // Pull-up A3
    DIO_u8SetPinValue(DIO_PORTD, DIO_PIN3, DIO_HIGH); // Pull-up A4

}

