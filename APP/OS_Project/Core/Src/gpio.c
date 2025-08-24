#include "gpio.h"

void GPIO_Init( void )
{
    P0MDOUT |= 0X10;
    P1MDOUT |= 0X80;
}