#include "main.h"

void main()
{
	sys_init();
	GPIO_Init();

	Uart2_Init(115200);
	Uart2_Send_Statu_Init();

	// Uart4_Init(115200);
	// Uart4_Send_Statu_Init();

	Tim0_Init();
	Tim1_Init();
	delay_ms(1000);
	LED = 1;
	//printf(" ===== code start ====== \r\n");
	while(1)
	{
		uart_frame_deal();
		Modbus_Event_Uart2();
	}
}