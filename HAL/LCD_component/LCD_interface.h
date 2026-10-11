#ifndef HAL_LCD_COMPONENT_LCD_INTERFACE_H_
#define HAL_LCD_COMPONENT_LCD_INTERFACE_H_



/****************************************************************/
/*                   LCD Standard Commands                      */
/****************************************************************/

/* 1. Clear & Home Commands */
#define LCD_CMD_CLEAR_DISPLAY           0x01    /* ���� ������ ����� ������ ������� */
#define LCD_CMD_RETURN_HOME             0x02    /* ���� ������ ������� ���� ��� ������ */

/* 2. Entry Mode Set */
#define LCD_CMD_ENTRY_MODE_INC          0x06    /* ���� �� ������ ������ (����� �������) */
#define LCD_CMD_ENTRY_MODE_DEC          0x04    /* ���� �� ������ ������ */
#define LCD_CMD_ENTRY_MODE_INC_SHIFT    0x07    /* ���� �� ������ ������ �� ����� ������ */

/* 3. Display ON/OFF Control */
#define LCD_CMD_DISPLAY_OFF             0x08    /* ���� ������ ������ */
#define LCD_CMD_DISPLAY_ON_CURSOR_OFF   0x0C    /* ���� ������ ����� ������ */
#define LCD_CMD_DISPLAY_ON_CURSOR_ON    0x0E    /* ���� ������ ����� ������ ���� */
#define LCD_CMD_DISPLAY_ON_CURSOR_BLINK 0x0F    /* ���� ������ ����� ������ ���� ����� */

/* 4. Cursor / Display Shift */
#define LCD_CMD_SHIFT_CURSOR_LEFT       0x10    /* ���� ������ ���� ������ */
#define LCD_CMD_SHIFT_CURSOR_RIGHT      0x14    /* ���� ������ ���� ������ */
#define LCD_CMD_SHIFT_DISPLAY_LEFT      0x18    /* ���� ������ ���� ���� ������ */
#define LCD_CMD_SHIFT_DISPLAY_RIGHT     0x1C    /* ���� ������ ���� ���� ������ */

/* 5. Function Set (Initialization Setup) */
#define LCD_CMD_FUNC_8BIT_2LINES        0x38    /* ����� ������: 8-�� ���ǡ ����� �� 5x8 */
#define LCD_CMD_FUNC_4BIT_2LINES        0x28    /* ����� ������: 4-�� ���ǡ ����� �� 5x8 */

/* 6. Memory Addresses Commands */
#define LCD_CMD_SET_CGRAM_ADDRESS       0x40    /* ����� ������� ������ ������ ������ ������� */
#define LCD_CMD_SET_DDRAM_ADDRESS       0x80    /* ����� ������� ������ ���� ������ ��� ������ */

/* 7. DDRAM Row Addresses (For 2x16 LCD) */
#define LCD_ROW0_START_ADDRESS          0x00    /* ����� ����� ����� ����� */
#define LCD_ROW1_START_ADDRESS          0x40    /* ����� ����� ����� ������ */




void LCD_voidSendCommand(u8 Copy_u8Cmd);
void LCD_voidSendData(u8 Copy_u8Data);
void LCD_voidPrint_P(const char *Copy_pFlashStr);
void LCD_voidInitHardware(void);
void LCD_voidGoToXY(u8 Copy_u8Row, u8 Copy_u8Col);
void LCD_voidClear(void);

#endif
