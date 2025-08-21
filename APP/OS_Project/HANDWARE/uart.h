#ifndef __UART_H__
#define __UART_H__

#include "sys.h"
#include <stdio.h>

#define T0_PERIOD_1MS 0xBCCD

extern u8 p_flag;

void Uart2_Init( u32 baud );
void uart2_send_byte( u8 byte );
void uart2_send_str( u8 *str,u16 len );
void Tim0_Init( void );

#endif