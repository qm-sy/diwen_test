#ifndef __SYS_H__
#define __SYS_H__

#include "t5los8051.h"
#include <STDIO.H>
#include <string.h>

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;
typedef signed char     s8;
typedef signed short    s16;
typedef signed long     s32;

sbit LED = P1^7;

#define	WDT_ON()				MUX_SEL|=0x02		
#define	WDT_OFF()				MUX_SEL&=0xFD		
#define	WDT_RST()				MUX_SEL|=0x01		


#define FOSC     				206438400UL
#define T1MS    				(65536-FOSC/12/1000)

void sys_init(void);
void delay_ms(uint16_t ms);
void delay_us(uint16_t us);
void sys_delay_ms(uint16_t ms);
void sys_read_vp(uint16_t addr,uint8_t* buf,uint16_t len);
void sys_write_vp(uint16_t addr,uint8_t* buf,uint16_t len);
void Write_Dgusii_Vp_byChar(uint16_t addr,uint8_t *buf,uint16_t len);
void Write_Dgus(uint16_t Dgus_Addr,uint16_t Val);
uint16_t Read_Dgus(uint16_t Dgus_Addr);

#endif


