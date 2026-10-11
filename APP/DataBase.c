#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DataBase.h"
#include "../HAL/EEPROM/EEPROM_interface.h"
#include "../HAL/Fingerprint/Fingerprint_interface.h"
#define F_CPU 8000000UL
#include <util/delay.h>



static UserRecord_t DB_Users[DB_MAX_USERS];

static u8 DB_UserCounter=0;

u8 DB_u8BootInitialization(void)
{
    u8 Byte1,Byte2,Byte3;
    u16 User_Adress;
    u8 LocalStateError;
    for(u8 i=0;i<DB_MAX_USERS;i++)
    {
        User_Adress=i*3;

        LocalStateError=EEPROM_u8ReadByte(0x50,User_Adress++,&Byte1);
        if(LocalStateError==1)return DB_BOOT_FAILED;

        LocalStateError= EEPROM_u8ReadByte(0x50,User_Adress++,&Byte2);
        if(LocalStateError==1)return DB_BOOT_FAILED;

        LocalStateError=EEPROM_u8ReadByte(0x50,User_Adress,&Byte3);
        if(LocalStateError==1)return DB_BOOT_FAILED;

        DB_Users[i].User_Id = (((u16)Byte1 << 8) | Byte2);
        DB_Users[i].user_privilege=Byte3;

        if(DB_Users[i].User_Id != 0xFFFF)
        {
            DB_UserCounter++;
        }
 
    }
    return DB_BOOT_SUCCESS ;
}

u8 DB_u8UserSearch(u16 Copy_u16TargetID, u8* Copy_pu8Privilege)
{
    if (Copy_pu8Privilege == NULL) return DB_USER_NOT_FOUND;

    for (u8 i = 0; i < DB_MAX_USERS; i++)
    {
        /* فحص الخانات المسجلة (التي لا تساوي 0xFFFF) */
        if (DB_Users[i].User_Id != 0xFFFF)
        {
            if (DB_Users[i].User_Id == Copy_u16TargetID)
            {
                *Copy_pu8Privilege = DB_Users[i].user_privilege;
                return DB_USER_FOUND; // وجد المستخدم بنجاح
            }
        }
    }

    return DB_USER_NOT_FOUND; // غير موجود
}

u8 DB_u8AddUser(u16 Copy_u16NewUserId, u8 Copy_u8NewUserPrivilege)
{
    u8 Byte1, Byte2, Byte3;
    u16 User_Address = 0;

    if (DB_UserCounter == DB_MAX_USERS) return DB_FULL;

    for (u8 i = 0; i < DB_MAX_USERS; i++)
    {
        if (DB_Users[i].User_Id == 0xFFFF)
        {
            /* 1. الحفظ في الـ RAM */
            DB_Users[i].User_Id = Copy_u16NewUserId;
            DB_Users[i].user_privilege = Copy_u8NewUserPrivilege;

            /* 2. تفكيك البيانات */
            Byte1 = (u8)((Copy_u16NewUserId >> 8) & 0xFF); 
            Byte2 = (u8)(Copy_u16NewUserId & 0xFF);
            Byte3 = Copy_u8NewUserPrivilege;
            
            User_Address = i * 3;

            /* 3. الحفظ في الـ EEPROM مع التأخير الإجباري لكل بايت */
            EEPROM_voidWriteByte(0x50, User_Address, Byte1);
            _delay_ms(10); // انتظار دورة الكتابة الداخلية للبايت الأول

            EEPROM_voidWriteByte(0x50, User_Address + 1, Byte2);
            _delay_ms(10); // انتظار دورة الكتابة الداخلية للبايت الثاني

            EEPROM_voidWriteByte(0x50, User_Address + 2, Byte3);
            _delay_ms(10); // انتظار دورة الكتابة الداخلية للبايت الثالث

            DB_UserCounter++;
            return DB_ADD_SUCCESS;
        }   
    }

    return DB_ADD_FAILED;
}

u8 DB_u8DeleteUser(u16 Copy_u16TargetID)
{
    u16 User_Adress = 0;
    
    for (u8 i = 0; i < DB_MAX_USERS; i++)
    {
        if (DB_Users[i].User_Id == Copy_u16TargetID)
        {
            DB_Users[i].User_Id = 0xFFFF;
            DB_Users[i].user_privilege = 0xFF;

            User_Adress = i * 3;
            EEPROM_voidWriteByte(0x50, User_Adress++, 0xFF);
            EEPROM_voidWriteByte(0x50, User_Adress++, 0xFF);
            EEPROM_voidWriteByte(0x50, User_Adress, 0xFF);

            if (DB_UserCounter > 0)
            {
                DB_UserCounter--;
            }
            return DB_USER_FOUND; 
        }
    }
    return DB_USER_NOT_FOUND; 
}


 

u16 DB_u16GetNextFreeID(void)
{
    /* البحث عن أول خانة فارغة فعلية في مصفوفة المستخدمين */
    for (u16 Local_u16ID = 0; Local_u16ID < DB_MAX_USERS; Local_u16ID++)
    {         
        if (DB_Users[Local_u16ID].User_Id == 0xFFFF)
        {
            return Local_u16ID; // أول ID متاح
        }
    }

    return DB_FULL; 
}

void DB_voidFactoryReset(void)
{
    /* 1. مسح فلاشة حساس البصمة R307 بالكامل */
    Fingerprint_u8EmptyDatabase();

    /* 2. تصفير الـ RAM */
    DB_UserCounter = 0;
    for (u8 i = 0; i < DB_MAX_USERS; i++)
    {
        DB_Users[i].User_Id = 0xFFFF;
        DB_Users[i].user_privilege = 0xFF;
    }

    /* 3. مسح الـ EEPROM وكتابة 0xFF في جميع العناوين مع التأخير الإجباري */
    u16 Local_u16Address = 0;
    for (u8 i = 0; i < DB_MAX_USERS; i++)
    {
        Local_u16Address = i * 3;

        EEPROM_voidWriteByte(0x50, Local_u16Address,     0xFF);
        _delay_ms(10); // تأخير دورة الكتابة الفيزيائية

        EEPROM_voidWriteByte(0x50, Local_u16Address + 1, 0xFF);
        _delay_ms(10);

        EEPROM_voidWriteByte(0x50, Local_u16Address + 2, 0xFF);
        _delay_ms(10);
    }
}

/* البحث اللحظي عن المستخدم التالي (تجاوز الخانات الفارغة والأدمن 0) */
u16 DB_u16GetNextActiveUser(u16 Copy_u16CurrentID)
{
    /* 1. البحث للأمام عن أول مستخدم بعد الرقم الحالي */
    for (u8 i = 0; i < DB_MAX_USERS; i++)
    {
        if (DB_Users[i].User_Id != 0xFFFF && DB_Users[i].User_Id != 0)
        {
            if (DB_Users[i].User_Id > Copy_u16CurrentID)
            {
                return DB_Users[i].User_Id;
            }
        }
    }

    /* 2. إذا وصلنا لآخر مستخدم، نلتف ونرجع لأول مستخدم مسجل */
    for (u8 i = 0; i < DB_MAX_USERS; i++)
    {
        if (DB_Users[i].User_Id != 0xFFFF && DB_Users[i].User_Id != 0)
        {
            return DB_Users[i].User_Id;
        }
    }

    return 0xFFFF; // لا يوجد أي مستخدمين مسجلين غير الأدمن
}
