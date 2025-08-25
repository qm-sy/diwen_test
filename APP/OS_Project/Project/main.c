#include "main.h"

void main()
{
	sys_init();
	GPIO_Init();
	burn_params_init();

	Uart2_Init(115200);
	Uart2_Send_Statu_Init();

	Uart4_Init(115200);
	Uart4_Send_Statu_Init();

	Uart5_Init(115200);
	Uart5_Send_Statu_Init();

	Tim0_Init();
	Tim1_Init();

	delay_ms(200);

	LED = 1;
	//printf(" ===== code start ====== \r\n");
	while(1)
	{
		uart_frame_deal();			//串口程序下载
		Modbus_Event_Uart2();
		Modbus_Event_Uart4();
		Modbus_Event_Uart5();
	}
}