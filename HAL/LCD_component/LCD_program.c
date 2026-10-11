#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "LCD_private.h"
#include "LCD_interface.h"
#include "LCD_config.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>


void LCD_voidSendCommand(u8 Copy_u8Cmd)
{
    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN1, DIO_LOW); // RS = 0
    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN2, DIO_LOW); // RW = 0
    DIO_u8SetPortValue(DIO_PORTB, Copy_u8Cmd);

    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN3, DIO_HIGH); // E = 1
    _delay_us(10);
    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN3, DIO_LOW);  // E = 0
    _delay_us(10);

    _delay_us(50);
}

void LCD_voidSendData(u8 Copy_u8Data)
{
    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN1, DIO_HIGH); // RS = 1
    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN2, DIO_LOW);  // RW = 0
    DIO_u8SetPortValue(DIO_PORTB, Copy_u8Data);

    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN3, DIO_HIGH); // E = 1
    _delay_us(10);
    DIO_u8SetPinValue(DIO_PORTA, DIO_PIN3, DIO_LOW);  // E = 0
    _delay_us(10);

    _delay_us(50);
}

/* دالة GoToXY مصححة هاردوير بالكامل */
void LCD_voidGoToXY(u8 Copy_u8Row, u8 Copy_u8Col)
{
    if (Copy_u8Col < 16)
    {
        if (Copy_u8Row == 0)
        {
            LCD_voidSendCommand(0x80 + Copy_u8Col);
        }
        else if (Copy_u8Row == 1)
        {
            LCD_voidSendCommand(0xC0 + Copy_u8Col);
        }
        _delay_ms(2); // مهلة استقرار المؤشر
    }
}

void LCD_voidInitHardware(void)
{
    DIO_u8SetPortDirection(DIO_PORTB, 0xff);
    DIO_u8SetPinDirection(LCD_CTR_PORT, LCD_RS_PIN, DIO_OUTPUT); // RS
    DIO_u8SetPinDirection(LCD_CTR_PORT, LCD_RW_PIN, DIO_OUTPUT); // RW
    DIO_u8SetPinDirection(LCD_CTR_PORT, LCD_E_PIN, DIO_OUTPUT); // E

    _delay_ms(50);

    LCD_voidSendCommand(0x38);
    _delay_ms(5);
    LCD_voidSendCommand(0x38);
    _delay_us(150);
    LCD_voidSendCommand(0x38);
    _delay_us(150);

    LCD_voidSendCommand(0x0C); // إخفاء المؤشر
    _delay_us(50);
    LCD_voidSendCommand(0x01); // مسح ابتدائي
    _delay_ms(5);
    LCD_voidSendCommand(0x06); // حركة لليمين
    _delay_us(50);
}


/* طباعة نصوص الفلاش مباشرة دون استهلاك بايت واحد من الرام */
void LCD_voidPrint_P(const char *Copy_pFlashStr)
{
    LCD_voidSendCommand(0x80); // توجيه المؤشر لأول خانة
    _delay_us(50);

    u8 i = 0;
    char c;
    while (i < 16)
    {
        c = pgm_read_byte(Copy_pFlashStr + i); // قراءة الحرف مباشرة من الفلاش (LPM)
        if (c == '\0') break;
        LCD_voidSendData((u8)c);
        i++;
    }

    /* مسح باقي الخانات بمسافات لضمان عدم بقاء نصوص قديمة */
    while (i < 16)
    {
        LCD_voidSendData(' ');
        i++;
    }
}

void LCD_voidClear(void)
{

    LCD_voidSendCommand(0x01);
    _delay_ms(2); 
    
}
