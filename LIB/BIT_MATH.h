#ifndef BIT_MATH_H
#define BIT_MATH_H

#define SET_BIT(reg,bit_num) ((reg)|=(1<<(bit_num)))
#define CLR_BIT(reg,bit_num) ((reg)&=(~(1<<(bit_num))))
#define TOG_BIT(reg,bit_num) ((reg)^=(1<<(bit_num)))
#define GET_BIT(reg,bit_num) (((reg)>>(bit_num)) &1)

#endif
