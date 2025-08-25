#ifndef __TIM_H__
#define __TIM_H__

#include "sys.h"
#include "uart.h"

extern bit press_flag;

#define T0_PERIOD_1MS 0xBCCD

void Tim0_Init( void );   
void Tim1_Init( void );      

#endif