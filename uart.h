#include <stdint.h>
#include "CU_TM4C123.h"

#ifndef __UART_H
#define __UART_H

#define FIFO_TRIG_LVL    UART_IFLS_RX4_8     // default RX 1/2 full

void uart_init(uint16_t);
uint8_t uart_sendbyte(uint8_t);
void uart_sendbytes(void *, uint16_t);
void uart_sendstr(void *);
int uart_getbyte(void);
uint16_t uart_getbytes(void *, uint16_t);

#endif
