#ifndef FINGERPRINT_PRIVATE_H
#define FINGERPRINT_PRIVATE_H

/* ثوابت حزمة البيانات (Packet Constants) */
#define FINGERPRINT_START_CODE_1      0xEF
#define FINGERPRINT_START_CODE_2      0x01
#define FINGERPRINT_ADDRESS           0xFFFFFFFF  /* العنوان الافتراضي */

/* أنواع حزم البيانات (Packet Identifiers) */
#define FINGERPRINT_COMMAND_PACKET    0x01
#define FINGERPRINT_DATA_PACKET       0x02
#define FINGERPRINT_ACK_PACKET        0x07
#define FINGERPRINT_END_DATA_PACKET   0x08

/* أوامر التشغيل (Instruction Codes) */
#define FINGERPRINT_CMD_GEN_IMG       0x01 /* التقاط صورة البصمة */
#define FINGERPRINT_CMD_IMG_2_TZ      0x02 /* تحويل الصورة لكود في الـ Buffer */
#define FINGERPRINT_CMD_MATCH         0x03 /* مقارنة بين 2 Buffers */
#define FINGERPRINT_CMD_SEARCH        0x04 /* البحث عن البصمة في الذاكرة */
#define FINGERPRINT_CMD_REG_MODEL     0x05 /* دمج كودين لتكوين بصمة نهائية */
#define FINGERPRINT_CMD_STORE         0x06 /* حفظ البصمة في الذاكرة */
#define FINGERPRINT_CMD_EMPTY         0x0D /* مسح كل البصمات من الذاكرة */

/* أكواد تأكيد الاستلام (Confirmation Codes) */
#define FINGERPRINT_ACK_SUCCESS       0x00
#define FINGERPRINT_ACK_PACKET_ERR    0x01
#define FINGERPRINT_ACK_NO_FINGER     0x02
#define FINGERPRINT_ACK_FAIL_ENROLL   0x03
#define FINGERPRINT_ACK_NOT_FOUND     0x09

#endif