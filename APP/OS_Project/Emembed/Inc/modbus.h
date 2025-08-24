#ifndef __MODBUS_H__
#define __MODBUS_H__

#include "communication.h"

void Modbus_Event_Uart2( void );
void Modbus_Event_Uart4( void );
void Modbus_Fun6( void );
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

#endif