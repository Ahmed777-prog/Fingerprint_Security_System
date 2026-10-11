// =============================================================================
// Project: Fingerprint Security System with FreeRTOS (Role-Based Access Control)
// Microcontroller: ATmega32 @ 8 MHz
// Peripherals: LCD (PORTB & PA1..3), Keypad, R307, EEPROM, LEDs (PD6, PD7), Servo (PD5)
// =============================================================================

// LIB LAYER
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// MCAL LAYER
#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/UART/UART_interface.h"
#include "../MCAL/I2C/I2C_interface.h"
#include "../MCAL/TMRS/TMR1/TMR1_interface.h"

// HAL LAYER
#include "../HAL/EEPROM/EEPROM_interface.h"
#include "../HAL/Fingerprint/Fingerprint_interface.h"
#include "../HAL/LCD_component/LCD_interface.h"
#include "../HAL/KPD_component/KPD_interface.h"
#include "../HAL/SERVO/SERVO_interface.h"

// APP LAYER
#include "DataBase.h"

// OS LAYER
#include "../OS/FREERTOS/include/FreeRTOS.h"
#include "../OS/FREERTOS/include/task.h"
#include "../OS/FREERTOS/include/queue.h"

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/pgmspace.h>

volatile u8 G_u8SystemMode = 0;

TaskHandle_t MonitorTask_Handle = NULL;
TaskHandle_t LogicTask_Handle   = NULL;
TaskHandle_t AdminUITask_Handle = NULL;
QueueHandle_t FingerprintQueue  = NULL;

void Task_SensorMonitor(void *pvParameters);
void Task_SecurityLogic(void *pvParameters);
void Task_AdminUI(void *pvParameters);


/* ========================================================================= */
/* دوال إشارات الليدات المساعدة                                              */
/* ========================================================================= */
void LED_voidBlinkGreen(u8 Copy_u8Times)
{
    for (u8 i = 0; i < Copy_u8Times; i++)
    {
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);
        vTaskDelay(150);
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
        vTaskDelay(150);
    }
}

void LED_voidBlinkRed(u8 Copy_u8Times)
{
    for (u8 i = 0; i < Copy_u8Times; i++)
    {
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH);
        vTaskDelay(150);
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);
        vTaskDelay(150);
    }
}

int main(void)
{
    UART_voidInit();
    DIO_u8SetPinDirection(DIO_PORTD, DIO_PIN0, DIO_INPUT);  // RX
    DIO_u8SetPinDirection(DIO_PORTD, DIO_PIN1, DIO_OUTPUT); // TX

    I2C_voidMasterInit();
    LCD_voidInitHardware();
    KPD_voidInit();

    /* 1. تهيئة الليدات على البورت D */
    DIO_u8SetPinDirection(DIO_PORTD, DIO_PIN7, DIO_OUTPUT); // أخضر (PD7)
    DIO_u8SetPinDirection(DIO_PORTD, DIO_PIN6, DIO_OUTPUT); // أحمر (PD6)
    DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
    DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);

    /* 2. تهيئة السيرفو على طرف PD5 (Timer1 OC1A) */
    DIO_u8SetPinDirection(DIO_PORTD, DIO_PIN5, DIO_OUTPUT); // OC1A
    TMR1_voidInit();
    Servo_voidSetAngle(0); // إغلاق الباب مبدئياً على زاوية 0

    /* 3. فحص قاعدة البيانات */
    u8 BootState = DB_u8BootInitialization();
    if (BootState == DB_BOOT_FAILED)
    {
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH);
        LCD_voidPrint_P(PSTR("System Error!"));
        while (1);
    }
    else
    {
        LCD_voidPrint_P(PSTR("System Ready!"));
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH);
        _delay_ms(1000);
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);
    }

    FingerprintQueue = xQueueCreate(3, sizeof(u16));

    xTaskCreate(Task_SensorMonitor, "Monitor", 170, NULL, 3, &MonitorTask_Handle);
    xTaskCreate(Task_SecurityLogic, "Logic",   150, NULL, 2, &LogicTask_Handle);
    xTaskCreate(Task_AdminUI, "Admin", 220, NULL, 1, &AdminUITask_Handle);

    vTaskSuspend(AdminUITask_Handle);

    vTaskStartScheduler();

    while (1);
    return 0;
}

/* ========================================================================= */
/* مهمة مراقبة الحساس والكيباد (Task_SensorMonitor)                          */
/* ========================================================================= */
void Task_SensorMonitor(void *pvParameters)
{
    u8 Local_u8ImageStatus = 0;
    u8 Local_u8SearchStatus = 0;
    u16 Local_u16FingerID = 0;
    u8 Local_u8PressedKey = 0;

    LCD_voidPrint_P(PSTR("Scan Finger..."));

    while (1)
    {
        /* 1. فحص ضغطة زرار A للدخول كأدمن */
        Local_u8PressedKey = KPD_u8GetPressedKey();
        
        if (Local_u8PressedKey == 'A')
        {
            while (KPD_u8GetPressedKey() != KPD_NO_PRESSED_KEY) { vTaskDelay(50); }

            /* صمام أمان الطوارئ: هل الأدمن الرئيسي (ID = 0) موجود أصلاً في قاعدة البيانات؟ */
            u8 Local_u8DummyPriv = 0;
            if (DB_u8UserSearch(0, &Local_u8DummyPriv) == DB_USER_NOT_FOUND)
            {
                /* طوارئ: لا يوجد أدمن مسجل، تحويل فوري للوحة التحكم لإعداد الأدمن */
                LCD_voidPrint_P(PSTR("No Admin! Setup"));
                vTaskDelay(1500);

                xQueueReset(FingerprintQueue);
                vTaskResume(AdminUITask_Handle);
                vTaskSuspend(NULL);
            }
            else
            {
                /* الأدمن موجود: طلب بصمة المسؤول للتحقق */
                LCD_voidPrint_P(PSTR("Place Admin..."));
                
                u8 Local_u8Timeout = 0;
                Local_u8ImageStatus = 1; // Not OK

                /* مهلة 5 ثوانٍ لانتظار بصمة الأدمن */
                while (Local_u8Timeout < 50)
                {
                    Local_u8ImageStatus = Fingerprint_u8TakeImage();
                    if (Local_u8ImageStatus == FINGERPRINT_OK)
                    {
                        break;
                    }
                    vTaskDelay(100);
                    Local_u8Timeout++;
                }

                if (Local_u8ImageStatus == FINGERPRINT_OK)
                {
                    Fingerprint_u8GenerateTemplate(1);
                    Local_u8SearchStatus = Fingerprint_u8Search(&Local_u16FingerID);

                    if (Local_u8SearchStatus == FINGERPRINT_OK)
                    {
                        u8 Local_u8Privilege = 0;
                        DB_u8UserSearch(Local_u16FingerID, &Local_u8Privilege);

                        /* التحقق: هل صاحب البصمة يحمل صلاحية الأدمن (1)؟ */
                        if (Local_u8Privilege == 1 || Local_u16FingerID == 0)
                        {
                            LCD_voidPrint_P(PSTR("Admin Granted!"));
                            vTaskDelay(1000);

                            while (Fingerprint_u8TakeImage() == FINGERPRINT_OK) { vTaskDelay(100); }

                            xQueueReset(FingerprintQueue);
                            vTaskResume(AdminUITask_Handle);
                            vTaskSuspend(NULL); // تجميد المراقبة
                        }
                        else
                        {
                            /* مستخدم عادي حاول دخول الأدمن */
                            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH); // أحمر
                            LCD_voidPrint_P(PSTR("Not An Admin!"));
                            vTaskDelay(2000);
                            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);

                            while (Fingerprint_u8TakeImage() == FINGERPRINT_OK) { vTaskDelay(100); }
                            LCD_voidPrint_P(PSTR("Scan Finger..."));
                        }
                    }
                    else
                    {
                        /* بصمة غير مسجلة أساساً */
                        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH);
                        LCD_voidPrint_P(PSTR("Unknown Finger!"));
                        vTaskDelay(2000);
                        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);

                        while (Fingerprint_u8TakeImage() == FINGERPRINT_OK) { vTaskDelay(100); }
                        LCD_voidPrint_P(PSTR("Scan Finger..."));
                    }
                }
                else
                {
                    /* انتهاء المهلة دون وضع بصمة */
                    LCD_voidPrint_P(PSTR("Timeout!"));
                    vTaskDelay(1000);
                    LCD_voidPrint_P(PSTR("Scan Finger..."));
                }
            }
        }

        /* 2. الفحص العادي للبصمة لفتح الباب للمستخدمين */
        Local_u8ImageStatus = Fingerprint_u8TakeImage();

        if (Local_u8ImageStatus == FINGERPRINT_OK)
        {
            Fingerprint_u8GenerateTemplate(1);
            Local_u8SearchStatus = Fingerprint_u8Search(&Local_u16FingerID);

            if (Local_u8SearchStatus == FINGERPRINT_OK)
            {
                xQueueSend(FingerprintQueue, &Local_u16FingerID, 10);

                while (Fingerprint_u8TakeImage() == FINGERPRINT_OK) { vTaskDelay(100); }
                vTaskDelay(3500); // مهلة كافية لانتهاء حركة السيرفو بالكامل

                LCD_voidPrint_P(PSTR("Scan Finger..."));
            }
            else
            {
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH); // أحمر
                LCD_voidPrint_P(PSTR("Unknown Finger!"));
                vTaskDelay(2000); 
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);

                while (Fingerprint_u8TakeImage() == FINGERPRINT_OK) { vTaskDelay(100); }
                
                LCD_voidPrint_P(PSTR("Scan Finger..."));
            }
        }
        else
        {
            vTaskDelay(50);
        }
    }
}

/* ========================================================================= */
/* مهمة المعالجة الأمنية (فتح الباب بالسيرفو والليد)                          */
/* ========================================================================= */
void Task_SecurityLogic(void *pvParameters)
{
    u16 Local_u16ReceivedID = 0;
    u8 Local_u8SearchStatus = 0;
    u8 Local_u8Privilege = 0;

    while (1)
    {
        if (xQueueReceive(FingerprintQueue, &Local_u16ReceivedID, portMAX_DELAY) == pdPASS)
        {
            Local_u8SearchStatus = DB_u8UserSearch(Local_u16ReceivedID, &Local_u8Privilege);

            if (Local_u8SearchStatus == DB_USER_FOUND)
            {
                /* 1. الترحيب وإضاءة الليد الأخضر */
                LCD_voidPrint_P(PSTR("Welcome User!"));
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);
                
                /* 2. فتح السيرفو (زاوية 90) */
                Servo_voidSetAngle(180);

                /* 3. مهلة فتح الباب (3 ثوانٍ) */
                vTaskDelay(3000);

                /* 4. غلق السيرفو (زاوية 0) وإطفاء الليد */
                Servo_voidSetAngle(0);
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
            }
            else
            {
                /* البصمة غير مسجلة في قاعدة البيانات */
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH); // أحمر
                LCD_voidPrint_P(PSTR("ID Not In DB!"));
                vTaskDelay(2000);
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);

                /* استعادة شاشة الاستعداد فوراً */
                LCD_voidPrint_P(PSTR("Scan Finger..."));
            }
        }
    }
}

/* ========================================================================= */
/* مهمة لوحة تحكم الأدمن (Task_AdminUI) - نسخة معزولة الذاكرة ومؤمنة الـ Stack */
/* ========================================================================= */
void Task_AdminUI(void *pvParameters)
{
    u8 Local_u8PressedKey = 0;
    u8 Local_u8ImageStatus = 0;
    u8 Local_u8MergeStatus = 0;
    u16 Local_u16NewUserID = 0;

    while (1)
    {
        /* مؤشر الأدمن: الأخضر منور باستمرار */
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);
        DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);
        
        LCD_voidPrint_P(PSTR("1:Add 2:Del 3:Ex"));

        /* انتظار ضغط أي زر من الكيباد */
        do
        {
            Local_u8PressedKey = KPD_u8GetPressedKey();
            vTaskDelay(50);
        } while (Local_u8PressedKey == KPD_NO_PRESSED_KEY);

        while (KPD_u8GetPressedKey() != KPD_NO_PRESSED_KEY) { vTaskDelay(50); }

        /* ==================== [ خيار 1: إضافة مستخدم ] ==================== */
        if (Local_u8PressedKey == '1')
        {
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH);
            
            Local_u16NewUserID = DB_u16GetNextFreeID();

            if (Local_u16NewUserID == DB_FULL || Local_u16NewUserID >= DB_MAX_USERS)
            {
                LCD_voidPrint_P(PSTR("Database Full!"));
                vTaskDelay(2000);
            }
            else
            {
                LCD_voidPrint_P(PSTR("Place Finger 1.."));

                do {
                    Local_u8ImageStatus = Fingerprint_u8TakeImage();
                    vTaskDelay(50);
                } while (Local_u8ImageStatus != FINGERPRINT_OK);

                Fingerprint_u8GenerateTemplate(1);

                LCD_voidPrint_P(PSTR("Remove Finger..."));
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);
                vTaskDelay(200);
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);

                while (Fingerprint_u8TakeImage() == FINGERPRINT_OK) { vTaskDelay(100); }
                vTaskDelay(500);

                LCD_voidPrint_P(PSTR("Place Again..."));
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);
                DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);

                do {
                    Local_u8ImageStatus = Fingerprint_u8TakeImage();
                    vTaskDelay(50);
                } while (Local_u8ImageStatus != FINGERPRINT_OK);

                Fingerprint_u8GenerateTemplate(2);
                Local_u8MergeStatus = Fingerprint_u8MergeTemplates();

                if (Local_u8MergeStatus == FINGERPRINT_OK)
                {
                    LCD_voidPrint_P(PSTR("1:User  2:Admin"));

                    u8 Local_u8PrivKey = 0;
                    u8 Local_u8Privilege = 0;

                    do {
                        Local_u8PrivKey = KPD_u8GetPressedKey();
                        vTaskDelay(50);
                    } while (Local_u8PrivKey != '1' && Local_u8PrivKey != '2');

                    while (KPD_u8GetPressedKey() != KPD_NO_PRESSED_KEY) { vTaskDelay(50); }

                    if (Local_u8PrivKey == '2')
                    {
                        Local_u8Privilege = 1;
                    }
                    else
                    {
                        Local_u8Privilege = 0;
                    }

                    Fingerprint_u8StoreTemplate(Local_u16NewUserID);
                    DB_u8AddUser(Local_u16NewUserID, Local_u8Privilege);

                    if (Local_u8Privilege == 1)
                    {
                        LCD_voidPrint_P(PSTR("Admin Added OK!"));
                    }
                    else
                    {
                        LCD_voidPrint_P(PSTR("User Added OK!"));
                    }

                    DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
                    LED_voidBlinkGreen(5);
                }
                else
                {
                    LCD_voidPrint_P(PSTR("Match Error!"));
                    DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
                    LED_voidBlinkRed(5);
                }
            }

            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);
        }
       /* ==================== 2. استعراض وحذف بدون مصفوفات (Zero-RAM) ==================== */
        else if (Local_u8PressedKey == '2')
        {
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH); // أحمر

            /* جلب أول مستخدم مسجل لحظياً (2 بايت فقط في الذاكرة) */
            u16 Local_u16SelectedID = DB_u16GetNextActiveUser(0);

            if (Local_u16SelectedID == 0xFFFF)
            {
                LCD_voidPrint_P(PSTR("No Users Found!"));
                LED_voidBlinkRed(2);
                vTaskDelay(2000);
            }
            else
            {
                u8 Local_u8Browsing = 1;

                while (Local_u8Browsing)
                {
                    /* طباعة: "Del: 1  (2:> =:D)" */
                    LCD_voidSendCommand(0x80);
                    _delay_us(50);

                    const char *pPrompt1 = PSTR("Del: ");
                    for (u8 p = 0; p < 5; p++) { LCD_voidSendData(pgm_read_byte(pPrompt1 + p)); }

                    if (Local_u16SelectedID >= 10)
                    {
                        LCD_voidSendData('0' + ((Local_u16SelectedID / 10) % 10));
                    }
                    LCD_voidSendData('0' + (Local_u16SelectedID % 10));

                    const char *pPrompt2 = PSTR(" (2:> =:D)");
                    for (u8 p = 0; p < 10; p++) { LCD_voidSendData(pgm_read_byte(pPrompt2 + p)); }

                    /* قراءة الكيباد */
                    u8 Local_u8Key = 0;
                    do {
                        Local_u8Key = KPD_u8GetPressedKey();
                        vTaskDelay(50);
                    } while (Local_u8Key == KPD_NO_PRESSED_KEY);

                    while (KPD_u8GetPressedKey() != KPD_NO_PRESSED_KEY) { vTaskDelay(50); }

                    if (Local_u8Key == '2') /* التالي */
                    {
                        Local_u16SelectedID = DB_u16GetNextActiveUser(Local_u16SelectedID);
                    }
                    else if (Local_u8Key == '=') /* تأكيد الحذف */
                    {
                        Fingerprint_u8DeleteTemplate(Local_u16SelectedID);
                        DB_u8DeleteUser(Local_u16SelectedID);

                        LCD_voidPrint_P(PSTR("Deleted Ok!"));
                        LED_voidBlinkGreen(2);
                        vTaskDelay(1500);
                        Local_u8Browsing = 0;
                    }
                    else if (Local_u8Key == '3' || Local_u8Key == '*' || Local_u8Key == 'C') /* خروج فوري */
                    {
                        Local_u8Browsing = 0;
                    }
                }
            }

            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_HIGH);
        }
        /* ==================== [ خيار 3: الخروج المؤكد ] ==================== */
        else if (Local_u8PressedKey == '3')
        {
            /* تأكيد بصري فوري بإطفاء الأخضر وتشغيل وميض أحمر سريع */
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN7, DIO_LOW);
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_HIGH);
            vTaskDelay(200);
            DIO_u8SetPinValue(DIO_PORTD, DIO_PIN6, DIO_LOW);

            LCD_voidPrint_P(PSTR("Exiting..."));
            vTaskDelay(1000);

            LCD_voidPrint_P(PSTR("Scan Finger..."));

            xQueueReset(FingerprintQueue);
            vTaskResume(MonitorTask_Handle);
            vTaskSuspend(NULL); // تجميد مهمة الأدمن
        }

        /* تصفير المتغير لضمان عدم تكرار أي أمر بالخطأ */
        Local_u8PressedKey = 0;
    }
}