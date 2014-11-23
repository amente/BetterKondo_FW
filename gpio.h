#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include "CU_TM4C123.h"

#define GPIO_FUNCTION_HALFDUP_SER 0
#define GPIO_FUNCTION_PWM 1

typedef struct _GPIO_PIN{
    uint8_t pin_num;
    GPIOA_Type* port;
    IRQn_Type IRQn;
    uint8_t GPIO_Function;
}GPIO_PIN;

#endif

GPIO_PIN* get_interupt_pin(uint8_t port_id);
void gpio_init(void);
