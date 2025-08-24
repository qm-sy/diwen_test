#include "uart.h"

uint8_t          xdata         R_u2[UART2_RX_LENTH];	  //串口2接受数组
uint8_t          xdata         R_OD2 = 0;	      //串口2收到数据
uint16_t         xdata         R_CN2 = 0;		  //串口2长度计数器
uint8_t          xdata         T_O2  = 0;		  //串口2超时计数器
bit			                   Busy2 = 0;	      //串口2发送标志

bit                 Response_flog    = 0;  //应答用
bit                 Auto_data_upload = 0;//数据自动上传用
bit                 Crc_check_flog   = 0;//Crc校验标记
bit                 download_flag    = 0;

void Uart2_Init( uint32_t baud )
{
    MUX_SEL |= 0X40;        //[6] = 1 时，IO口 P0.4、P0.5作为uart2接口

    P0MDOUT &= 0XCF;        //P0口输出配置
    P0MDOUT |= 0X10;        //P0.4输出开启，P0.5输出关闭

    SCON0 = 0x50;           //uart2控制接口，SM1 = 1, REN0 = 1;
    ADCON = 0x80;           //波特率发生器选择  0x00=T1 定时器，0x80=使用 SREL0H:L。
    PCON &= 0x7f;           //[7] = 1 时，波特率倍频 为0则不倍频

    baud = 1024 - FOSC / 64 / baud;

    SREL0H = (uint8_t)(baud>>8);
    SREL0L = (uint8_t)baud;

    ES0 = 1;                //uart2 中断开启
    EA  = 1;                //总中断开关
}

void Uart4_Init( uint32_t baud )
{
	SCON2T= 0x80;       //发送使能和模式设置
	SCON2R= 0x80;       //接受使能和模式设置 
	ES2R = 1;           //中断接受使能
    ES2T = 1;           //中断发送使能

    baud = 1024 - FOSC / 64 / baud;

    BODE2_DIV_H = (uint8_t)(baud>>8);
    BODE2_DIV_L = (uint8_t)baud;

    P0MDOUT|=(1<<0); //p0^0 强推
    // TR4=0;
}

void uart_frame_deal( void )
{
	//串口2数据收发
	if(( R_OD2 == 1 ) && ( T_O2 == 0 ))
	{
		R_OD2=0;
		deal_uart_data(R_u2,(uint16_t*)(&R_CN2),2,RESPONSE_UART2,CRC_CHECK_UART2);
		R_CN2=0;
	} 
}

void deal_uart_data(uint8_t* Arr_rece,uint16_t* Data_len,uint8_t Uart_num,bit Response,bit Crc_ck)
{
	uint16_t N = 0;	
	bit Flog = 1;

	while(Flog)
	{		
		if(( Arr_rece[N] == DTHD1 ) && ( Arr_rece[N+1] == DTHD2 ))   //5A A5 07 82 1000 0001 0002
		{	     
			if( Arr_rece[N+3] == 0x82 )     
			{	   
				Response_flog = Response;				//应答标记
				Crc_check_flog = Crc_ck;				//Crc标记
				deal_82_cmd(Uart_num,Arr_rece + N);	  //处理82指令
				N = N + Arr_rece[N + 2] + 2;
			}								   		    // 0  1	 2  3  4	5  6  7	8  9 10
			else if( Arr_rece[N + 3] == 0x83 ) 		    //5A A5 08 83 1000 02 0001 0002
			{	   	
				uint8_t Val[FRAME_LEN] = {0}; 
				Crc_check_flog = Crc_ck;				//Crc标记
				deal_83_cmd(Uart_num, Val, Arr_rece + N);//处理83指令
				N = N + Arr_rece[N + 2] + 2;
			}else
			{
				N++;
			}		
		}
		if( N < (*Data_len - 4)) 
		{
			N++;
		}
		else 
		{
			Flog=0; 
		}           
	}
}

void  deal_82_cmd(uint8_t Uart, uint8_t* arr)
{    
	uint8_t i=0;
	if( Crc_check_flog == 0 ) //不含CRC检验
	{		 
		if(arr[4] == 0x00 && arr[5] == 0x06)
		{
			Write_Dgus(0x0006,0x5aa5);
			Write_Dgus(0x0007,0X8000);
		}else
		{
			Write_Dgusii_Vp_byChar((arr[4] << 8) + arr[5],arr + 6,arr[2] - 3);	
		}
		
		if( Response_flog == 1 )   
		{
			uint8_t Temp_arr[]={DTHD1,DTHD2,0X03,0X82,0X4F,0X4B}; 	
									
			for(i = 0; i < 6; i++)
			{
				uart_send_byte(Uart,Temp_arr[i]);	//5A A5 03 82 4F 4B
			}                  	 
		}
	}
}

/****************************************************************************/
void  deal_83_cmd(uint8_t Uart,uint8_t* arr,uint8_t* arr1)
{
    uint8_t i=0;
    if(Crc_check_flog==0) //不含CRC检验
    {		
        for( i = 0; i < 7; i++ )
        {
            arr[i]=arr1[i];
        }

        sys_read_vp((arr[4] << 8) + arr[5],(uint8_t*) & arr[7],arr[6]);
        arr[2]=(2 * arr[6]) + 4;

        for(i = 0; i < (arr[2] + 3); i++)
        {
            uart_send_byte(Uart,arr[i]);
        }                              
    }
}

void uart_send_byte(uint8_t Uart_number,uint8_t Dat)
{
    download_flag = 1;
    if( Uart_number == 2 )
    {
        while (Busy2);               
        Busy2 = 1;
        SBUF0 = Dat; 		 		 
    }
}
