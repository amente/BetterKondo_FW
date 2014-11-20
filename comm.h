#ifndef __COMM_H
#define __COMM_H

#include "CU_TM4C123.h"
#include <stdint.h>

typedef struct __COMM_ANS
{
    uint8_t *msg;
    uint8_t len;
} COMM_ANS;

#define COMM_UART           UART2
#define COMM_BAUD           9600

#define COMM_START          0xFE

#define COMM_BUF_SZ         32

#define COMM_CMD_ANS        0x80
#define COMM_CMD_INFO       0
#define COMM_CMD_SETPOS     1

void comm_init(COMM_ANS (*cb)(uint8_t msg[], uint8_t len));

#endif
