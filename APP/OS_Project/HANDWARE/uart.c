#include "uart.h"

RS485 rs485;
u8 p_flag;

/**
 * @brief	串口4调用结构体 rs485 初始化
 *
 * @param   
 *
 * @return  void
**/
void Uart2_Send_Statu_Init( void )
{
    rs485.RX2_rev_end_Flag = 0;
    rs485.TX2_buf[128] = 0;
    rs485.RX2_buf[128] = 0;
    rs485.TX2_send_bytelength = 0;
    rs485.TX2_send_cnt = 0;
    rs485.RX2_rev_timeout = 0;
    rs485.RX2_rev_cnt = 0;
    //DR_485 = 0;
}

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

void Tim0_Init( void )      //1ms
{
    TMOD &= 0xfc;           //设置为16bit的定时器工作模式
    TMOD |= 0x00;           //T0_M1 = 0 T0_M0 = 0: 16位自动重装载模式 

    TH0 = (u8)(T0_PERIOD_1MS>>8);
    TL0 = (u8)T0_PERIOD_1MS;        

    ET0 = 1;                //使能中断
    EA = 1;                 //总中断开关
    TR0 = 1;                //启动T0定时器
    //TF0 = 0;              //硬件自动清0
}

void Tim1_Init( void )      //1ms
{
    TMOD &= 0xcf;           //设置为16bit的定时器工作模式
    TMOD |= 0x00;           //T1_M1 = 0 T1_M0 = 0: 16位自动重装载模式 

    TH1 = (u8)(T0_PERIOD_1MS>>8);
    TL1 = (u8)T0_PERIOD_1MS;        

    ET1 = 1;                //使能中断
    EA = 1;                 //总中断开关
    TR1 = 1;                //启动T1定时器
    //TF1 = 0;              //硬件自动清0
}

void Tim0_Isr( void ) interrupt 1
{
    static u16 delay_cnt = 0;

    delay_cnt++;

    if(delay_cnt == 1000)
    {
        p_flag = 1;
        delay_cnt = 0;
    }
}

void Tim1_Isr( void ) interrupt 3
{
    /* 1, 如果接收未超时                                             */
    if ( rs485.RX2_rev_timeout != 0 )  
    {
        rs485.RX2_rev_timeout--;
        /* 2, 如果接收超时                                          */
        if( rs485.RX2_rev_timeout == 0 )  
        {
            if( rs485.RX2_rev_cnt > 0 )  
            {   
                 /* 3, 接收完毕标志位亮起并初始化接收缓冲区         */
                rs485.RX2_rev_end_Flag = 1;    
            }
        }
    } 
}


/**
 * @brief	串口4中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart2_ISR() interrupt 4 
{   

    /* 1, 检测到S4TI置1，即发送完毕                       */
    if( TI0 == 1)          //
    {
        /* 2, 软件将S4TI清零，等待发送标志位重置，可继续发送    */
        TI0 = 0;         
        /* 3, 依次将TX2_buf中数据送出（写SBUF0操作即为发送）    */
        if( rs485.TX2_send_bytelength != 0 )
        {
            SBUF0 = rs485.TX2_buf[rs485.TX2_send_cnt++];
            rs485.TX2_send_bytelength--;
        }else
        {
            rs485.TX2_send_cnt = 0;
            //DR_485 = 0;
        }
    }
    
    /* 1, 检测到S2RI置1，即接收完毕                       */
    if( RI0 == 1 )
    {
        /* 2, 软件将S2RI清零，等待接收标志位重置，可继续发送    */
        RI0 = 0;
        LED = !LED;
        /* 3, 判断数据包是否接收完毕                           */
        if( !rs485.RX2_rev_end_Flag )
        {
            /* 4, 数据包大于RX_buf 则从头计数                  */
            if( rs485.RX2_rev_cnt > 128 )
            {
                rs485.RX2_rev_cnt = 0;
            }

            /* 5, 依次将RX2_buf中数据接收（读S2BUF操作即为接收）*/
            rs485.RX2_buf[rs485.RX2_rev_cnt] = SBUF0;
            rs485.RX2_rev_cnt++;
        }
        /* 6, 重置接收完毕判断时间                              */
        rs485.RX2_rev_timeout = 5;
    }
}

void Modbus_Event( void )
{
    // uint16_t crc,rccrc;
    
    /*1.接收完毕                                           */
    if( rs485.RX2_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485.RX2_rev_end_Flag = 0;

        // /*3.CRC校验                                         */
        // crc = MODBUS_CRC16(rs485.RX2_buf, rs485.RX2_rev_cnt-2);
        // rccrc = (rs485.RX2_buf[rs485.RX2_rev_cnt-1]) | (rs485.RX2_buf[rs485.RX2_rev_cnt-2]<<8);

        /*4.清空接收计数                                    */
        rs485.RX2_rev_cnt = 0; 
        Modbus_Fun6();
        // /*5.CRC校验通过，进行地址域校验                      */
        // if( crc == rccrc )
        // {
        //     /*6.地址域校验通过，进入相应功能函数进行处理      */
        //     if( rs485.RX2_buf[0] == 0x01 )
        //     {
        //         switch ( rs485.RX2_buf[1] )
        //         {
        //             case 0x06:
                        
        //                 break;  

        //             default:
        //                 break;
        //         }
        //     }
        // }
    }
}

void Modbus_Fun6( void )
{
    // memcpy(rs485.TX2_buf,rs485.RX2_buf,8);

    // rs485.TX2_send_bytelength = 8;

    // TI0 = 1;                              //开始发送
    // sys_delay_about_ms(1);
    rs485.TX2_buf[0] = 0x00;
    rs485.TX2_buf[1] = 0x01;
    rs485.TX2_buf[2] = 0x02;
    rs485.TX2_buf[3] = 0x03;
    rs485.TX2_buf[4] = 0x04;
    rs485.TX2_buf[5] = 0x05;

    rs485.TX2_send_bytelength = 6;
    TI0 = 1;
}

// void uart2_send_byte( u8 byte )
// {
//     ES0 = 0;
//     SBUF0 = byte;
//     while (!TI0);
//     TI0 = 0;
//     ES0 = 1;
// }

// void uart2_send_str( u8 *str,u16 len )
// {
//     u16 i;

//     for(i=0;i<len;i++)
//     {
//         uart2_send_byte(str[i]);
//     }
// }

// char putchar(char c)
// {
//     uart2_send_byte(c);
//     return c;
// }


/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}
