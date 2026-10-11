#ifndef APP_DATABASE_H_
#define APP_DATABASE_H_

#define DB_MAX_USERS    100
#define EMPTY_SLOT      0xFF

#define DB_USER_NOT_FOUND   0
#define DB_USER_FOUND       1
#define DB_FULL             5

#define DB_BOOT_FAILED      0
#define DB_BOOT_SUCCESS     1

#define DB_ADD_FAILED       0
#define DB_ADD_SUCCESS      1

typedef struct
{ 
    u16 User_Id;
    u8 user_privilege;
} UserRecord_t;

u8 DB_u8BootInitialization(void);

u8 DB_u8UserSearch(u16 Copy_u16TargetID,u8* Copy_pu8Privilege );

u8 DB_u8AddUser(u16 Copy_u16NewUserId,u8 Copy_u8NewUserPrivilege);

u8 DB_u8DeleteUser(u16 Copy_u16TargetID);

u16 DB_u16GetNextFreeID(void);

void DB_voidFactoryReset(void);

u16 DB_u16GetNextActiveUser(u16 Copy_u16CurrentID);




#endif