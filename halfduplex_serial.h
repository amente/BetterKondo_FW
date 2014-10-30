#ifndef __HALFDUPLEX_SERIAL_H
#define __HALFDUPLEX_SERIAL_H

#include <stdint.h>
#include "CU_TM4C123.h"
#include "gpio.h"

#define HALFDUP_SER_BAUD_1200 6664
#define HALFDUP_SER_BAUD_2400 1666
#define HALFDUP_SER_BAUD_9600 833

void halfdup_ser_init(uint16_t baud);
void halfdup_ser_begin(GPIO_PIN* pin);
void wait_half_bit(void);
uint8_t halfdup_ser_getByte(void);
void halfdup_ser_sendByte(uint8_t byte);

#endif
