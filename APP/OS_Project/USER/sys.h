#ifndef __SYS_H__
#define __SYS_H__
#include "t5los8051.h"

//�����ض���
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned long   u32;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;
typedef signed char     s8;
typedef signed short    s16;
typedef signed long     s32;

//���Ź��궨��
#define	WDT_ON()				MUX_SEL|=0x02		//�������Ź�
#define	WDT_OFF()				MUX_SEL&=0xFD		//�رտ��Ź�
#define	WDT_RST()				MUX_SEL|=0x01		//ι��

//ϵͳ��Ƶ��1ms��ʱ��ֵ����
#define FOSC     				206438400UL
#define T1MS    				(65536-FOSC/12/1000)

 


//��������
void sys_init(void);
void sys_delay_about_ms(u16 ms);
void sys_delay_about_us(u8 us);
void sys_delay_ms(u16 ms);
void sys_read_vp(u16 addr,u8* buf,u16 len);
void sys_write_vp(u16 addr,u8* buf,u16 len);

#endif


