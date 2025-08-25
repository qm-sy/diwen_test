#include "communication.h"

RS485_2 rs485_2;
RS485_4 rs485_4;
RS485_5 rs485_5;

/**
 * @brief	串口2调用结构体 rs485_2 初始化
 *
 * @param   
 *
 * @return  void
**/
void Uart2_Send_Statu_Init( void )
{
    rs485_2.RX2_rev_end_Flag = 0;
    rs485_2.TX2_buf[128] = 0;
    rs485_2.RX2_buf[128] = 0;
    rs485_2.TX2_send_bytelength = 0;
    rs485_2.TX2_send_cnt = 0;
    rs485_2.RX2_rev_timeout = 0;
    rs485_2.RX2_rev_cnt = 0;
    //DR_485 = 0;
}

/**
 * @brief	串口4调用结构体 rs485_4 初始化
 *
 * @param   
 *
 * @return  void
**/
void Uart4_Send_Statu_Init( void )
{
    rs485_4.RX4_rev_end_Flag = 0;
    rs485_4.TX4_buf[128] = 0;
    rs485_4.RX4_buf[128] = 0;
    rs485_4.TX4_send_bytelength = 0;
    rs485_4.TX4_send_cnt = 0;
    rs485_4.RX4_rev_timeout = 0;
    rs485_4.RX4_rev_cnt = 0;
    //DR_485 = 0;
}

/**
 * @brief	串口5调用结构体 rs485_5 初始化
 *
 * @param   
 *
 * @return  void
**/
void Uart5_Send_Statu_Init( void )
{
    rs485_5.RX5_rev_end_Flag = 0;
    rs485_5.TX5_buf[128] = 0;
    rs485_5.RX5_buf[128] = 0;
    rs485_5.TX5_send_bytelength = 0;
    rs485_5.TX5_send_cnt = 0;
    rs485_5.RX5_rev_timeout = 0;
    rs485_5.RX5_rev_cnt = 0;
    //DR_485 = 0;
}

/**
 * @brief	串口4中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart2_RXTX_Isr( void ) interrupt 4 
{   

    /* 1, 检测到S4TI置1，即发送完毕                       */
    if( TI0 == 1)          //
    {
        /* 2, 软件将S4TI清零，等待发送标志位重置，可继续发送    */
        TI0 = 0;    
        Busy2 = 0;   
        /* 3, 依次将TX2_buf中数据送出（写SBUF0操作即为发送）    */
        if( download_flag == 0 )
        {
            if( rs485_2.TX2_send_bytelength != 0 )
            {
                SBUF0 = rs485_2.TX2_buf[rs485_2.TX2_send_cnt++];
                rs485_2.TX2_send_bytelength--;
            }else
            {
                rs485_2.TX2_send_cnt = 0;
                //DR_485 = 0;
            }
        }
    }
    
    /* 1, 检测到S2RI置1，即接收完毕                       */
    if( RI0 == 1 )
    {
        /* 2, 软件将S2RI清零，等待接收标志位重置，可继续发送    */
        RI0 = 0;

        R_u2[R_CN2] = SBUF0;
		R_OD2 = 1;
		if(R_CN2<UART2_RX_LENTH-1)
		{
			R_CN2++;
		} 
		T_O2=5; 
        
        /* 3, 判断数据包是否接收完毕                           */
        if( !rs485_2.RX2_rev_end_Flag )
        {
            /* 4, 数据包大于RX_buf 则从头计数                  */
            if( rs485_2.RX2_rev_cnt > 128 )
            {
                rs485_2.RX2_rev_cnt = 0;
            }

            /* 5, 依次将RX2_buf中数据接收（读S2BUF操作即为接收）*/
            rs485_2.RX2_buf[rs485_2.RX2_rev_cnt] = SBUF0;
            rs485_2.RX2_rev_cnt++;
        }
        /* 6, 重置接收完毕判断时间                              */
        rs485_2.RX2_rev_timeout = 5;
    }

}


/**
 * @brief	串口4中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart4_TX_Isr() interrupt 10 
{   
    /* 1, 检测到S4TI置1，即发送完毕                       */
    if( SCON2T & S4TI )          //
    {
        /* 2, 软件将S4TI清零，等待发送标志位重置，可继续发送    */
        SCON2T &= ~S4TI;     
        /* 3, 依次将TX4_buf中数据送出（写SBUF0操作即为发送）    */
        if( rs485_4.TX4_send_bytelength != 0 )
        {
            SBUF2_TX = rs485_4.TX4_buf[rs485_4.TX4_send_cnt++];
            rs485_4.TX4_send_bytelength--;
        }else
        {
            rs485_4.TX4_send_cnt = 0;
            //DR_485 = 0;
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
void Uart4_RX_Isr() interrupt 11 
{   
        /* 1, 检测到S2RI置1，即接收完毕                       */
    if( SCON2R & S4RI )
    {
        /* 2, 软件将S2RI清零，等待接收标志位重置，可继续发送    */
        SCON2R &= ~S4RI; 
        /* 3, 判断数据包是否接收完毕                           */
        if( !rs485_4.RX4_rev_end_Flag )
        {
            /* 4, 数据包大于RX_buf 则从头计数                  */
            if( rs485_4.RX4_rev_cnt > 128 )
            {
                rs485_4.RX4_rev_cnt = 0;
            }

            /* 5, 依次将RX4_buf中数据接收（读S2BUF操作即为接收）*/
            rs485_4.RX4_buf[rs485_4.RX4_rev_cnt] = SBUF2_RX;
            rs485_4.RX4_rev_cnt++;
        }
        /* 6, 重置接收完毕判断时间                              */
        rs485_4.RX4_rev_timeout = 5;
    }
}


/**
 * @brief	串口4中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Uart5_TX_Isr() interrupt 12 
{   
    /* 1, 检测到S4TI置1，即发送完毕                       */
    if( SCON3T & S5TI )          //
    {
        /* 2, 软件将S4TI清零，等待发送标志位重置，可继续发送    */
        SCON3T &= ~S5TI;     
        /* 3, 依次将TX5_buf中数据送出（写SBUF0操作即为发送）    */
        if( rs485_5.TX5_send_bytelength != 0 )
        {
            SBUF3_TX = rs485_5.TX5_buf[rs485_5.TX5_send_cnt++];
            rs485_5.TX5_send_bytelength--;
        }else
        {
            rs485_5.TX5_send_cnt = 0;
            //DR_485 = 0;
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
void Uart5_RX_Isr() interrupt 13 
{   
        /* 1, 检测到S2RI置1，即接收完毕                       */
    if( SCON3R & S5RI )
    {
        /* 2, 软件将S2RI清零，等待接收标志位重置，可继续发送    */
        SCON3R &= ~S5RI; 

        /* 3, 判断数据包是否接收完毕                           */
        if( !rs485_5.RX5_rev_end_Flag )
        {
            /* 4, 数据包大于RX_buf 则从头计数                  */
            if( rs485_5.RX5_rev_cnt > 128 )
            {
                rs485_5.RX5_rev_cnt = 0;
            }

            /* 5, 依次将RX5_buf中数据接收（读S2BUF操作即为接收）*/
            rs485_5.RX5_buf[rs485_5.RX5_rev_cnt] = SBUF3_RX;
            rs485_5.RX5_rev_cnt++;
        }
        /* 6, 重置接收完毕判断时间                              */
        rs485_5.RX5_rev_timeout = 5;
    }
}

void Tim1_Isr( void ) interrupt 3
{
    TH1   = (uint8_t)(T0_PERIOD_1MS>>8);
    TL1   = (uint8_t)T0_PERIOD_1MS;  

    /* 1, 如果接收未超时                                             */
    if ( rs485_2.RX2_rev_timeout != 0 )  
    {
        rs485_2.RX2_rev_timeout--;
        /* 2, 如果接收超时                                          */
        if( rs485_2.RX2_rev_timeout == 0 )  
        {
            if( rs485_2.RX2_rev_cnt > 0 )  
            {   
                 /* 3, 接收完毕标志位亮起并初始化接收缓冲区         */
                rs485_2.RX2_rev_end_Flag = 1;    
            }
        }
    } 

        /* 1, 如果接收未超时                                             */
    if ( rs485_4.RX4_rev_timeout != 0 )  
    {
        rs485_4.RX4_rev_timeout--;
        /* 2, 如果接收超时                                          */
        if( rs485_4.RX4_rev_timeout == 0 )  
        {
            if( rs485_4.RX4_rev_cnt > 0 )  
            {   
                 /* 3, 接收完毕标志位亮起并初始化接收缓冲区         */
                rs485_4.RX4_rev_end_Flag = 1;    
            }
        }
    } 

    /* 1, 如果接收未超时                                             */
    if ( rs485_5.RX5_rev_timeout != 0 )  
    {
        rs485_5.RX5_rev_timeout--;
        /* 2, 如果接收超时                                          */
        if( rs485_5.RX5_rev_timeout == 0 )  
        {
            if( rs485_5.RX5_rev_cnt > 0 )  
            {   
                 /* 3, 接收完毕标志位亮起并初始化接收缓冲区         */
                rs485_5.RX5_rev_end_Flag = 1;    
            }
        }
    } 
}


// void uart2_send_byte( uint8_t byte )
// {
//     ES0 = 0;
//     SBUF0 = byte;
//     while (!TI0);
//     TI0 = 0;
//     ES0 = 1;
// }

// void uart2_send_str( uint8_t *str,uint16_t len )
// {
//     uint16_t i;

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

void press_scan( void )
{
    uint16_t reg_val = 0; 

    if( press_flag == 1 )
    {
        sys_read_vp(0x2000,(uint8_t*)&reg_val,1);

        if( reg_val != 0 )
        {
            switch(reg_val)
            {
                case 1:
                    LED = 0;
                    break;

                case 2:
                    LED = 1;
                    break;
                    
                case 3:
                    LED = 0;
                    break;

                case 4:
                    LED = 1;
                    break;
                    
                default:
                    break;
            }

            reg_val = 0;
            sys_write_vp(0x2000,(uint8_t*)&reg_val,1);
        }

        press_flag = 0;
    }
}
