#ifndef __UART_H__
#define __UART_H__

#include "sys.h"

#define	      UART2_ENABLE          1               //T5L  串口2开启关闭设置，1开启，0关闭
#define	      UART3_ENABLE          1               //T5L  串口3开启关闭设置，1开启，0关闭
#define	      UART4_ENABLE          1               //T5L  串口4开启关闭设置，1开启，0关闭
#define	      UART5_ENABLE          1               //T5L  串口5开启关闭设置，1开启，0关闭

#define	      UART2_RX_LENTH	    2048            //T5L  串口2 接收数组长度 
#define	      UART3_RX_LENTH	    2048            //T5L  串口3 接收数组长度 
#define	      UART4_RX_LENTH    	2048            //T5L  串口4 接收数组长度 
#define	      UART5_RX_LENTH	    2048            //T5L  串口5 接收数组长度 

#define       BAUD_UART2	        115200          //T5L  串口2波特率设置
#define       BAUD_UART3	        115200          //T5L  串口3波特率设置
#define       BAUD_UART4	        115200          //T5L  串口4波特率设置
#define       BAUD_UART5	        115200          //T5L  串口5波特率设置

#define       RESPONSE_UART2          1               //串口2应答4F4B开启关闭设置，RESPONSE_UART2=1开启，RESPONSE_UART2=0关闭
#define       RESPONSE_UART3          1               //串口3应答4F4B开启关闭设置，RESPONSE_UART3=1开启，RESPONSE_UART3=0关闭
#define       RESPONSE_UART4          1               //串口4应答4F4B开启关闭设置，RESPONSE_UART4=1开启，RESPONSE_UART4=0关闭
#define       RESPONSE_UART5          1               //串口5应答4F4B开启关闭设置，RESPONSE_UART5=1开启，RESPONSE_UART5=0关闭

#define       DATA_UPLOAD_UART2          1           //串口2数据自动上传设置，1上传，0不上传
#define       DATA_UPLOAD_UART3          1           //串口3数据自动上传设置，1上传，0不上传
#define       DATA_UPLOAD_UART4          1           //串口4数据自动上传设置，1上传，0不上传
#define       DATA_UPLOAD_UART5          1           //串口5数据自动上传设置，1上传，0不上传

#define       CRC_CHECK_UART2            0           //串口2数据CRC检验设置，1开启CRC，0关闭CRC
#define       CRC_CHECK_UART3            0           //串口3数据CRC检验设置，1开启CRC，0关闭CRC
#define       CRC_CHECK_UART4            0           //串口4数据CRC检验设置，1开启CRC，0关闭CRC
#define       CRC_CHECK_UART5            0           //串口5数据CRC检验设置，1开启CRC，0关闭CRC

#define       DTHD1         0X5A			  //帧头1
#define       DTHD2         0XA5			  //帧头2  
#define       FOSC          206438400UL
#define       FRAME_LEN     255		    	 //帧长

extern uint8_t          xdata         R_u2[UART2_RX_LENTH];	  //串口2接受数组
extern uint8_t          xdata         R_OD2;	      //串口2收到数据
extern uint16_t         xdata         R_CN2;		  //串口2长度计数器
extern uint8_t          xdata         T_O2;		  //串口2超时计数器
extern bit			                  Busy2;	      //串口2发送标志

extern bit Response_flog;       //应答用
extern bit Auto_data_upload;    //数据自动上传用
extern bit Crc_check_flog;      //Crc校验标记
extern bit download_flag;

void burn_params_init( void );
void Uart2_Init( uint32_t baud );
void Uart4_Init( uint32_t baud );
void Uart5_Init( uint32_t baud );
void uart_frame_deal( void );
void deal_uart_data(uint8_t* Arr_rece,uint16_t* Data_len,uint8_t Uart_num,bit Response,bit Crc_ck);
void deal_82_cmd(uint8_t Uart,uint8_t* arr);
void deal_83_cmd(uint8_t Uart,uint8_t* arr,uint8_t* arr1);
void uart_send_byte(uint8_t Uart_number,uint8_t Dat);
uint16_t crc16table(uint8_t *ptr, uint16_t len);

#endif