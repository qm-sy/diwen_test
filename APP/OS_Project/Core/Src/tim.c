#include "tim.h"


void Tim0_Init( void )      //1ms
{
    TMOD &= 0xfc;           //设置为16bit的定时器工作模式
    TMOD |= 0x00;           //T0_M1 = 0 T0_M0 = 0: 16位自动重装载模式 

    TH0   = (uint8_t)(T0_PERIOD_1MS>>8);
    TL0   = (uint8_t)T0_PERIOD_1MS;        

    ET0   = 1;              //使能中断
    EA    = 1;              //总中断开关
    TR0   = 1;              //启动T0定时器
    //TF0 = 0;              //硬件自动清0
}

void Tim1_Init( void )      //1ms
{
    TMOD &= 0xcf;           //设置为16bit的定时器工作模式
    TMOD |= 0x00;           //T1_M1 = 0 T1_M0 = 0: 16位自动重装载模式 

    TH1   = (uint8_t)(T0_PERIOD_1MS>>8);
    TL1   = (uint8_t)T0_PERIOD_1MS;        

    ET1   = 1;              //使能中断
    EA    = 1;              //总中断开关
    TR1   = 1;              //启动T1定时器
    //TF1 = 0;              //硬件自动清0
}

void Tim0_Isr( void ) interrupt 1
{
	if(T_O2 > 0)
    {
        T_O2--;
    }
}

