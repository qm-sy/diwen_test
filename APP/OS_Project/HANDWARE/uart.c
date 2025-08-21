#include "uart.h"

u8 p_flag;

void Uart2_Init( u32 baud )
{
    MUX_SEL |= 0X40;        //[6] = 1 时，IO口 P0.4、P0.5作为uart2接口

    P0MDOUT &= 0XCF;        //P0口输出配置
    P0MDOUT |= 0X10;        //P0.4输出开启，P0.5输出关闭

    SCON0 = 0x50;           //uart2控制接口，SM1 = 1, REN0 = 1;
    ADCON = 0x80;           //波特率发生器选择  0x00=T1 定时器，0x80=使用 SREL0H:L。
    PCON &= 0x7f;           //[7] = 1 时，波特率倍频 为0则不倍频

    baud = 1024 - (u16)(3225600.0f/baud);

    SREL0H = (u8)(baud>>8);
    SREL0L = (u8)baud;

    ES0 = 1;                //uart2 中断开启
    EA  = 1;                //总中断开关
}

void Tim0_Init( void )
{
    TMOD &= 0xfc;
    TMOD |= 0x01;


    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void Tim0_Isr( void ) interrupt 1
{
    static idata u16 delay_cnt = 0;

    TH0 = (u8)(T0_PERIOD_1MS>>8);
    TL0 = (u8)T0_PERIOD_1MS;

    delay_cnt++;

    if(delay_cnt == 1000)
    {
        p_flag = 1;
        delay_cnt = 0;
    }
}

void uart2_send_byte( u8 byte )
{
    ES0 = 0;
    SBUF0 = byte;
    while (!TI0);
    TI0 = 0;
    ES0 = 1;
}

void uart2_send_str( u8 *str,u16 len )
{
    u16 i;

    for(i=0;i<len;i++)
    {
        uart2_send_byte(str[i]);
    }
}

char putchar(char c)
{
    uart2_send_byte(c);
    return c;
}