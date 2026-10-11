#ifndef LCD_PRIVATE_H_
#define LCD_PRIVATE_H_

/****************************************************************/
/*                   LCD Standard Commands                      */
/****************************************************************/

/* 1. Clear & Home Commands */
#define LCD_CMD_CLEAR_DISPLAY           0x01    /* םדÓÍ ÇבÔÇÔÉ זםÑÌÚ ÇבדÄÔÑ בבÈÏÇםÉ */
#define LCD_CMD_RETURN_HOME             0x02    /* םÑÌÚ ÇבדÄÔÑ בבÈÏÇםÉ ÈÏזה דÓÍ ÇבÔÇÔÉ */

/* 2. Entry Mode Set */
#define LCD_CMD_ENTRY_MODE_INC          0x06    /* ם‗ÊÈ דה ÇבÔדÇב בבםדםה (ÇבזÖÚ ÇבØÈםÚם) */
#define LCD_CMD_ENTRY_MODE_DEC          0x04    /* ם‗ÊÈ דה Çבםדםה בבÔדÇב */
#define LCD_CMD_ENTRY_MODE_INC_SHIFT    0x07    /* ם‗ÊÈ דה ÇבÔדÇב בבםדםה דÚ ÊÔÝםÊ ÇבÔÇÔÉ */

/* 3. Display ON/OFF Control */
#define LCD_CMD_DISPLAY_OFF             0x08    /* םÞÝב ÇבÔÇÔÉ ÊדÇדÇנ */
#define LCD_CMD_DISPLAY_ON_CURSOR_OFF   0x0C    /* םÔÛב ÇבÔÇÔÉ זםÎÝם ÇבדÄÔÑ */
#define LCD_CMD_DISPLAY_ON_CURSOR_ON    0x0E    /* םÔÛב ÇבÔÇÔÉ זםÙוÑ ÇבדÄÔÑ ËÇÈÊ */
#define LCD_CMD_DISPLAY_ON_CURSOR_BLINK 0x0F    /* םÔÛב ÇבÔÇÔÉ זםÎבם ÇבדÄÔÑ םהזÑ זםØÝם */

/* 4. Cursor / Display Shift */
#define LCD_CMD_SHIFT_CURSOR_LEFT       0x10    /* םÍÑ‗ ÇבדÄÔÑ ÎØזÉ בבÔדÇב */
#define LCD_CMD_SHIFT_CURSOR_RIGHT      0x14    /* םÍÑ‗ ÇבדÄÔÑ ÎØזÉ בבםדםה */
#define LCD_CMD_SHIFT_DISPLAY_LEFT      0x18    /* םÍÑ‗ ÇבÔÇÔÉ ‗בוÇ ÎØזÉ בבÔדÇב */
#define LCD_CMD_SHIFT_DISPLAY_RIGHT     0x1C    /* םÍÑ‗ ÇבÔÇÔÉ ‗בוÇ ÎØזÉ בבםדםה */

/* 5. Function Set (Initialization Setup) */
#define LCD_CMD_FUNC_8BIT_2LINES        0x38    /* ÊוםÆÉ ÇבÔÇÔÉ: 8-ÈÊ ÏÇÊÇ¡ ÓØÑםה¡ ÎØ 5x8 */
#define LCD_CMD_FUNC_4BIT_2LINES        0x28    /* ÊוםÆÉ ÇבÔÇÔÉ: 4-ÈÊ ÏÇÊÇ¡ ÓØÑםה¡ ÎØ 5x8 */

/* 6. Memory Addresses Commands */
#define LCD_CMD_SET_CGRAM_ADDRESS       0x40    /* ÇבÃדÑ ÇבÃÓÇÓם בבזÕזב בÐÇ‗ÑÉ ÇבÍÑזÝ ÇבדÎÕÕÉ */
#define LCD_CMD_SET_DDRAM_ADDRESS       0x80    /* ÇבÃדÑ ÇבÃÓÇÓם בÊÍÏםÏ ד‗Çה ÇבדÄÔÑ Úבל ÇבÔÇÔÉ */

/* 7. DDRAM Row Addresses (For 2x16 LCD) */
#define LCD_ROW0_START_ADDRESS          0x00    /* ÈÏÇםÉ ÚהזÇה ÇבÓØÑ ÇבÃזב */
#define LCD_ROW1_START_ADDRESS          0x40    /* ÈÏÇםÉ ÚהזÇה ÇבÓØÑ ÇבËÇהם */

#endif /* LCD_PRIVATE_H_ */
