#ifndef FINGERPRINT_INTERFACE_H
#define FINGERPRINT_INTERFACE_H

#define FINGERPRINT_OK  0x00
#define FINGERPRINT_NOT_FOUND  0x09

/* ========================================================= */
/*                 Fingerprint Sensor APIs                   */
/* ========================================================= */

u8 Fingerprint_u8TakeImage(void);
u8 Fingerprint_u8GenerateTemplate(u8 Copy_u8BufferID);
u8 Fingerprint_u8MergeTemplates(void);
u8 Fingerprint_u8StoreTemplate(u16 Copy_u16LocationID);
u8 Fingerprint_u8Search(u16 *Copy_pu16ReturnID);
u8 Fingerprint_u8EmptyDatabase(void);
u8 Fingerprint_u8DeleteTemplate(u16 Copy_u16TemplateID);

#endif