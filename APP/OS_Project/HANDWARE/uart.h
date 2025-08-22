#ifndef __UART_H__
#define __UART_H__

#include "sys.h"
#include <stdio.h>
#include <STRING.H>
sbit LED = P1^7;

#define T0_PERIOD_1MS 0xBCCD

extern u8 p_flag;

typedef struct 
{
    uint8_t     RX2_rev_end_Flag;       //数据包接收完毕标志
    uint8_t     TX2_buf[128];           //SBUF TI缓冲区
    uint8_t     RX2_buf[128];           //SBUF RI缓冲区
    uint8_t     TX2_send_bytelength;    //发送字节数
    uint8_t     TX2_send_cnt;           //发送计数
    uint16_t    RX2_rev_timeout;        //接收超时
    uint8_t     RX2_rev_cnt;            //接收计数
    uint8_t     DR_Flag;                //DR
}RS485;

extern RS485 rs485;

void Uart2_Init( u32 baud );
void uart2_send_byte( u8 byte );
void uart2_send_str( u8 *str,u16 len );
void Tim0_Init( void );
void Tim1_Init( void );
void Modbus_Fun6( void );
void Modbus_Event( void );
void Uart2_Send_Statu_Init( void );
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

#endif