#ifndef __COMMUNICTION_H__
#define __COMMUNICTION_H__

#include "uart.h"
#include "tim.h"

#define S4TI    0X01
#define S4RI    0X01

#define S5TI    0X01
#define S5RI    0X01

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
}RS485_2;

typedef struct 
{
    uint8_t     RX4_rev_end_Flag;       //数据包接收完毕标志
    uint8_t     TX4_buf[128];           //SBUF TI缓冲区
    uint8_t     RX4_buf[128];           //SBUF RI缓冲区
    uint8_t     TX4_send_bytelength;    //发送字节数
    uint8_t     TX4_send_cnt;           //发送计数
    uint16_t    RX4_rev_timeout;        //接收超时
    uint8_t     RX4_rev_cnt;            //接收计数
    uint8_t     DR_Flag;                //DR
}RS485_4;

typedef struct 
{
    uint8_t     RX5_rev_end_Flag;       //数据包接收完毕标志
    uint8_t     TX5_buf[128];           //SBUF TI缓冲区
    uint8_t     RX5_buf[128];           //SBUF RI缓冲区
    uint8_t     TX5_send_bytelength;    //发送字节数
    uint8_t     TX5_send_cnt;           //发送计数
    uint16_t    RX5_rev_timeout;        //接收超时
    uint8_t     RX5_rev_cnt;            //接收计数
    uint8_t     DR_Flag;                //DR
}RS485_5;

extern RS485_2 rs485_2;
extern RS485_4 rs485_4;
extern RS485_5 rs485_5;

void Uart2_Send_Statu_Init( void );
void Uart4_Send_Statu_Init( void );
void Uart5_Send_Statu_Init( void );
void uart2_send_byte( uint8_t byte );
void uart2_send_str( uint8_t *str,uint16_t len );
#endif