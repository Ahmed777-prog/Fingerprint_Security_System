#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/UART/UART_private.h"
#include "../../MCAL/UART/UART_interface.h"
#include "Fingerprint_interface.h"
#include "Fingerprint_private.h"

#define F_CPU 8000000UL
#include <util/delay.h>

/* ========================================================= */
/*                   Private Helper Functions                */
/* ========================================================= */

static void Fingerprint_voidSendCommand(u8 Copy_u8Command, u8* Copy_pu8Params, u8 Copy_u8ParamLength)
{
    u16 Local_u16Checksum = 0;
    u16 Local_u16Length = Copy_u8ParamLength + 3; 
    
    /* 1. Send Header & Address */
    UART_u8TransmitByte(FINGERPRINT_START_CODE_1);
    UART_u8TransmitByte(FINGERPRINT_START_CODE_2);
    UART_u8TransmitByte(0xFF);
    UART_u8TransmitByte(0xFF);
    UART_u8TransmitByte(0xFF);
    UART_u8TransmitByte(0xFF);
    
    /* 2. Send Packet Type & Length */
    UART_u8TransmitByte(FINGERPRINT_COMMAND_PACKET);
    UART_u8TransmitByte((u8)(Local_u16Length >> 8));   
    UART_u8TransmitByte((u8)(Local_u16Length & 0xFF)); 
    
    /* 3. Send Command & Params */
    UART_u8TransmitByte(Copy_u8Command);
    for(u8 i = 0; i < Copy_u8ParamLength; i++)
    {
        UART_u8TransmitByte(Copy_pu8Params[i]);
    }
    
    /* 4. Calculate & Send Checksum */
    Local_u16Checksum = FINGERPRINT_COMMAND_PACKET + (Local_u16Length >> 8) + (Local_u16Length & 0xFF) + Copy_u8Command;
    for(u8 i = 0; i < Copy_u8ParamLength; i++)
    {
        Local_u16Checksum += Copy_pu8Params[i];
    }
    UART_u8TransmitByte((u8)(Local_u16Checksum >> 8));
    UART_u8TransmitByte((u8)(Local_u16Checksum & 0xFF));
}

static u8 Fingerprint_u8ReceiveAck(void)
{
    u8 Local_u8ReceivedByte = 0;
    u8 Local_u8AckCode = 0xFF;
    
    /* Standard ACK packet is 12 bytes. Byte 9 contains the status code */
    for(u8 i = 0; i < 12; i++)
    {
        Local_u8ReceivedByte = UART_u8ReceiveByte(); 
        
        if(i == 9)
        {
            Local_u8AckCode = Local_u8ReceivedByte;
        }
    }
    
    return Local_u8AckCode;
}

/* ========================================================= */
/*                          Public APIs                      */
/* ========================================================= */

u8 Fingerprint_u8TakeImage(void)
{
    Fingerprint_voidSendCommand(FINGERPRINT_CMD_GEN_IMG, NULL, 0);
    return Fingerprint_u8ReceiveAck();
}

u8 Fingerprint_u8GenerateTemplate(u8 Copy_u8BufferID)
{
    u8 Local_u8Param = Copy_u8BufferID; 
    Fingerprint_voidSendCommand(FINGERPRINT_CMD_IMG_2_TZ, &Local_u8Param, 1);
    return Fingerprint_u8ReceiveAck();
}

u8 Fingerprint_u8MergeTemplates(void)
{
    Fingerprint_voidSendCommand(FINGERPRINT_CMD_REG_MODEL, NULL, 0);
    return Fingerprint_u8ReceiveAck();
}

u8 Fingerprint_u8StoreTemplate(u16 Copy_u16LocationID)
{
    u8 Local_u8Params[3];
    
    Local_u8Params[0] = 0x01; /* Default Buffer 1 */
    Local_u8Params[1] = (u8)(Copy_u16LocationID >> 8);
    Local_u8Params[2] = (u8)(Copy_u16LocationID & 0xFF);
    
    Fingerprint_voidSendCommand(FINGERPRINT_CMD_STORE, Local_u8Params, 3);
    return Fingerprint_u8ReceiveAck();
}

u8 Fingerprint_u8Search(u16 *Copy_pu16ReturnID)
{
    u8 Local_u8Ack = 0xFF;
    u8 Local_u8ReceivedByte = 0;
    u8 Local_u8Params[5];
    
    /* Search in all 1000 locations (0x0000 to 0x03E7) */
    Local_u8Params[0] = 0x01; /* Buffer 1 */
    Local_u8Params[1] = 0x00; /* Start Page High */
    Local_u8Params[2] = 0x00; /* Start Page Low */
    Local_u8Params[3] = 0x03; /* End Page High */
    Local_u8Params[4] = 0xE7; /* End Page Low */
    
    Fingerprint_voidSendCommand(FINGERPRINT_CMD_SEARCH, Local_u8Params, 5);
    
    /* Search response is 16 bytes. Byte 9 is status, Bytes 10 & 11 are matched ID */
    for(u8 i = 0; i < 16; i++)
    {
        Local_u8ReceivedByte = UART_u8ReceiveByte(); 
        
        if(i == 9)  Local_u8Ack = Local_u8ReceivedByte;
        if(i == 10) *Copy_pu16ReturnID = (u16)(Local_u8ReceivedByte << 8);
        if(i == 11) *Copy_pu16ReturnID |= Local_u8ReceivedByte;
    }
    
    return Local_u8Ack;
}

u8 Fingerprint_u8DeleteTemplate(u16 Copy_u16TemplateID)
{
    /* 
     * البارامترات: 
     * Byte 0: البايت العالي من الـ ID
     * Byte 1: البايت المنخفض من الـ ID
     * Byte 2 & 3: عدد البصمات اللي هنمسحها (1 بس)
     */
    u8 Local_u8Params[4];
    Local_u8Params[0] = (u8)(Copy_u16TemplateID >> 8);
    Local_u8Params[1] = (u8)(Copy_u16TemplateID);
    Local_u8Params[2] = 0x00;
    Local_u8Params[3] = 0x01; 
    
    Fingerprint_voidSendCommand(0x0C, Local_u8Params, 4);
    
    return Fingerprint_u8ReceiveAck(); /* هترجع 0x00 لو اتمسحت بنجاح */
}

u8 Fingerprint_u8EmptyDatabase(void)
{
    Fingerprint_voidSendCommand(FINGERPRINT_CMD_EMPTY, NULL, 0);
    return Fingerprint_u8ReceiveAck();
}