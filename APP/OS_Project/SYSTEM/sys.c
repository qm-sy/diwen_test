#include "sys.h"

static uint16_t data SysTick = 0;	


//���ļĴ�����ʼ��
void sys_cpu_init()
{
	EA = 0;
	RS0 = 0;
	RS1 = 0;

	CKCON = 0x00;
	T2CON = 0x70;
	DPC = 0x00;
	PAGESEL = 0x01;
	D_PAGESEL = 0x02; //DATA RAM  0x8000-0xFFFF
	MUX_SEL = 0x00;   //UART2,UART3�رգ�WDT�ر�
	RAMMODE = 0x00;
	PORTDRV = 0x01;   //����ǿ��+/-8mA
	IEN0 = 0x00;      //�ر������ж�
	IEN1 = 0x00;
	IEN2 = 0x00;
	IP0 = 0x00;       //�ж����ȼ�Ĭ��
	IP1 = 0x00;

	WDT_OFF();      	//�رտ��Ź�
}

//��ʱ��2��ʼ��,��ʱ���Ϊ1ms
void sys_timer2_init()
{
	T2CON = 0x70;
	TH2 = 0x00;
	TL2 = 0x00;

	TRL2H = 0xBC;	//1ms�Ķ�ʱ��
	TRL2L = 0xCD;       

	IEN0 |= 0x20;	//������ʱ��2
	TR2 = 0x01;
}

//ϵͳ��ʼ��
void sys_init()
{
	sys_cpu_init();//���ļĴ�����ʼ��
	sys_timer2_init();//��ʱ��2��ʼ��
}

// //���������ʱ,��λms
// //����޸����Ż��ȼ�,��ô�˺����ڲ��Ĳ�����Ҫ���µ���
// void sys_delay_about_ms(uint16_t ms)
// {
// 	uint16_t i,j;
// 	for(i=0;i<ms;i++)
// 			for(j=0;j<3000;j++);    
// }

// //���������ʱ,��λus
// //����޸����Ż��ȼ�,��ô�˺����ڲ��Ĳ�����Ҫ���µ���
// void sys_delay_about_us(uint8_t us)
// {
// 	uint8_t i,j;
// 	for(i=0;i<us;i++)
// 			for(j=0;j<5;j++);    
// }

void delay_ms(uint16_t ms)
{
    int data mi,mj;
    for(mi=0;mi<ms;mi++)
        for(mj=0;mj<18500;mj++);
}

void delay_us(uint16_t us)
{
    int data ui,uj;
    for(ui=0;ui<us;ui++)
        for(uj=0;uj<5;uj++);
}

uint16_t Read_Dgus(uint16_t Dgus_Addr)
{	   	   	uint16_t R_Dgus=0;
//			EA = 0;
			ADR_H = 0x00;
			ADR_M = (uint8_t)((Dgus_Addr/2)>>8);
			ADR_L = (uint8_t)(Dgus_Addr/2);
			RAMMODE = 0xAF;		//������
			while(!APP_ACK);
			APP_EN = 1;
			while(APP_EN);
			if(Dgus_Addr%2)	R_Dgus = (DATA1<<8)+DATA0; 
			else   R_Dgus=(DATA3<<8)+DATA2;
			RAMMODE = 0;
//			EA = 1;
			return   R_Dgus;		
}
/****************************************************************************/
void Write_Dgus(uint16_t Dgus_Addr,uint16_t Val)
{          
//	EA = 0;
	ADR_H = 0x00;
	ADR_M = (uint8_t)((Dgus_Addr/2)>>8);
	ADR_L = (uint8_t)(Dgus_Addr/2);
	RAMMODE = 0xAF;		//������
	while(!APP_ACK);
	APP_EN = 1;
	while(APP_EN);
	RAMMODE = 0;
	ADR_M = (uint8_t)((Dgus_Addr/2)>>8);
	ADR_L = (uint8_t)(Dgus_Addr/2);
	RAMMODE = 0x8F;		//д����
	while(!APP_ACK);
	if(Dgus_Addr%2){DATA1=(uint8_t)(Val>>8); DATA0=(uint8_t)(Val); }
	else { DATA3 =(uint8_t)(Val>>8);	DATA2 =(uint8_t)(Val);}
	APP_EN = 1;
	while(APP_EN);
	RAMMODE = 0;
//	EA = 1;
}

//��DGUS�е�VP��������
//addr:����ֱ�Ӵ���DGUS�еĵ�ַ
//buf:������
//len:��ȡ������,һ���ֵ���2���ֽ�
void sys_read_vp(uint16_t addr,uint8_t* buf,uint16_t len)
{   
	uint8_t i; 
	
	i = (uint8_t)(addr&0x01);
	addr >>= 1;
	ADR_H = 0x00;
	ADR_M = (uint8_t)(addr>>8);
	ADR_L = (uint8_t)addr;
	ADR_INC = 0x01;
	RAMMODE = 0xAF;
	while(APP_ACK==0);
	while(len>0)
	{   
		APP_EN=1;
		while(APP_EN==1);
		if((i==0)&&(len>0))   
		{   
			*buf++ = DATA3;
			*buf++ = DATA2;                      
			i = 1;
			len--;	
		}
		if((i==1)&&(len>0))   
		{   
			*buf++ = DATA1;
			*buf++ = DATA0;                      
			i = 0;
			len--;	
		}
	}
	RAMMODE = 0x00;
}

//дDGUS�е�VP��������
//addr:����ֱ�Ӵ���DGUS�еĵ�ַ
//buf:������
//len:���������ݵ�����,һ���ֵ���2���ֽ�
void sys_write_vp(uint16_t addr,uint8_t* buf,uint16_t len)
{   
	uint8_t i;  
	
	i = (uint8_t)(addr&0x01);
	addr >>= 1;
	ADR_H = 0x00;
	ADR_M = (uint8_t)(addr>>8);
	ADR_L = (uint8_t)addr;    
	ADR_INC = 0x01;
	RAMMODE = 0x8F;
	while(APP_ACK==0);
	if(i && len>0)
	{	
		RAMMODE = 0x83;	
		DATA1 = *buf++;		
		DATA0 = *buf++;	
		APP_EN = 1;		
		len--;
	}
	RAMMODE = 0x8F;
	while(len>=2)
	{	
		DATA3 = *buf++;		
		DATA2 = *buf++;
		DATA1 = *buf++;		
		DATA0 = *buf++;
		APP_EN = 1;		
		len -= 2;
	}
	if(len)
	{	
		RAMMODE = 0x8C;
		DATA3 = *buf++;		
		DATA2 = *buf++;
		APP_EN = 1;
	}
	RAMMODE = 0x00;
} 

/*****************************************************************************/
void Write_Dgusii_Vp_byChar(unsigned int addr,unsigned char *buf,unsigned int len)
{
	unsigned char i;
//	bit EA_Bak;
//	EA_Bak = EA;
//	EA = 0;
	i = (unsigned char)(addr & 0x01);
	addr = addr / 2;
	ADR_H = 0x00;
	ADR_M = (unsigned char)(addr >> 8);
	ADR_L = (unsigned char)(addr);
	ADR_INC = 0x01;
	RAMMODE = 0x8F;
	while(APP_ACK == 0);
	if(i && len > 0){
		if(len == 1){
			RAMMODE = 0x82;
			DATA1 = *buf++;
			APP_EN = 1;
			while(APP_EN == 1);
			len--;
		} else {
			RAMMODE = 0x83;
			DATA1 = *buf++;
			DATA0 = *buf++;
			APP_EN = 1;
			while(APP_EN == 1);
			len -= 2;
		}
	}
	RAMMODE = 0x8F;
	while(len >= 4){
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		DATA0 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
		len -= 4;
	}
	
	if(len == 3){
		RAMMODE = 0x8E;
		DATA3 = *buf++;
		DATA2 = *buf++;
		DATA1 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	} else if(len == 2){
		RAMMODE = 0x8C;
		DATA3 =* buf++;
		DATA2 =* buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	} else if(len == 1){
		RAMMODE = 0x88;
		DATA3 = *buf++;
		APP_EN = 1;
		while(APP_EN == 1);
	}
	RAMMODE = 0x00;
//	EA = EA_Bak;
}

//��ʱ��2�жϷ������
void sys_timer2_isr()	interrupt 5
{
    TF2=0;    
    SysTick--; 
    WDT_RST(); 
}



