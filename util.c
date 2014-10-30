#include "util.h"


void delay_us(uint32_t us){
    SysTick->LOAD = us*4;
    SysTick->CTRL = 1<<0;
    while(!(SysTick->CTRL & 1<<16));
    SysTick->CTRL &= ~(1<<0);
}

void delay_ms(uint32_t ms){
    delay_us(ms*1000);
}
