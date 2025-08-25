#include "modbus.h"

void Modbus_Event_Uart2( void )
{
    uint16_t crc,rccrc = 0;

    /*1.接收完毕                                           */
    if( rs485_2.RX2_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485_2.RX2_rev_end_Flag = 0;

        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485_2.RX2_buf, rs485_2.RX2_rev_cnt-2);
        rccrc = (rs485_2.RX2_buf[rs485_2.RX2_rev_cnt-1]) | (rs485_2.RX2_buf[rs485_2.RX2_rev_cnt-2]<<8);

        /*4.清空接收计数                                    */
        rs485_2.RX2_rev_cnt = 0; 

        /*5.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
                    
            /*6.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485_2.RX2_buf[0] == 0x01 )
            {
                switch ( rs485_2.RX2_buf[1] )
                {
                    case 0x06:
                        Modbus_Fun6();
                        break;  

                    default:
                        break;
                }
            }
        }
    }
}

void Modbus_Event_Uart4( void )
{
    uint16_t crc,rccrc = 0;

    /*1.接收完毕                                           */
    if( rs485_4.RX4_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485_4.RX4_rev_end_Flag = 0;

        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485_4.RX4_buf, rs485_4.RX4_rev_cnt-2);
        rccrc = (rs485_4.RX4_buf[rs485_4.RX4_rev_cnt-1]) | (rs485_4.RX4_buf[rs485_4.RX4_rev_cnt-2]<<8);

        /*4.清空接收计数                                    */
        rs485_4.RX4_rev_cnt = 0; 

        /*5.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
            /*6.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485_4.RX4_buf[0] == 0x01 )
            {
                switch ( rs485_4.RX4_buf[1] )
                {
                    case 0x06:
                        Modbus_Fun6();
                        break;  

                    default:
                        break;
                }
            }
        }
    }
}

void Modbus_Event_Uart5( void )
{
    uint16_t crc,rccrc = 0;

    /*1.接收完毕                                           */
    if( rs485_5.RX5_rev_end_Flag == 1 )
    {
        /*2.清空接收完毕标志位                              */    
        rs485_5.RX5_rev_end_Flag = 0;

        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485_5.RX5_buf, rs485_5.RX5_rev_cnt-2);
        rccrc = (rs485_5.RX5_buf[rs485_5.RX5_rev_cnt-1]) | (rs485_5.RX5_buf[rs485_5.RX5_rev_cnt-2]<<8);

        /*4.清空接收计数                                    */
        rs485_5.RX5_rev_cnt = 0; 

        /*5.CRC校验通过，进行地址域校验                      */
        if( crc == rccrc )
        {
            /*6.地址域校验通过，进入相应功能函数进行处理      */
            if( rs485_5.RX5_buf[0] == 0x01 )
            {
                switch ( rs485_5.RX5_buf[1] )
                {
                    case 0x06:
                        Modbus_Fun6();
                        break;  

                    default:
                        break;
                }
            }
        }
    }
}

void Modbus_Fun6( void )
{
    // memcpy(rs485_2.TX2_buf,rs485_2.RX2_buf,8);

    // download_flag = 0;
    // rs485_2.TX2_send_bytelength = 8;

    // TI0 = 1;                              //开始发送

    // memcpy(rs485_4.TX4_buf,rs485_4.RX4_buf,8);

    // download_flag = 0;
    // rs485_4.TX4_send_bytelength = 8;

    // SCON2T |= S4TI;  

    //memcpy(rs485_5.TX5_buf,rs485_5.RX5_buf,8);

    rs485_5.TX5_buf[0] = 0x00;
    rs485_5.TX5_buf[1] = 0x01;
    rs485_5.TX5_buf[2] = 0x02;
    rs485_5.TX5_buf[3] = 0x03;
    rs485_5.TX5_buf[4] = 0x04;
    rs485_5.TX5_buf[5] = 0x05;

    download_flag = 0;
    rs485_5.TX5_send_bytelength = 6;

    SCON3T |= S5TI;  
}

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