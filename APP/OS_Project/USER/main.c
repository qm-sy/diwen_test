#include "sys.h"
#include "uart.h"


void main()
{
	sys_init();
	Uart2_Init(115200);
	Uart2_Send_Statu_Init();
	Tim0_Init();
	Tim1_Init();
	p_flag = 0;
	P1MDOUT |= 0x80;
	PORTDRV = 0X01;
	LED = 0;
	while(1)
	{
		Modbus_Event();
	}
}