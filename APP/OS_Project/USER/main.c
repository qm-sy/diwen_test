#include "sys.h"
#include "uart.h"

void main()
{
	sys_init();
	Uart2_Init(115200);
	Tim0_Init();
	p_flag = 0;

	while(1)
	{
		if( p_flag == 1 )
		{
			printf("hello world \r\n");
			p_flag = 0;
		}
		printf("ggg\r\n");
		sys_delay_about_ms(1000);
	}
}