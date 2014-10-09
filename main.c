
#include "i2c.h"
#include <stdint.h>
#include "servos.h"

int 
main(void){
    servos_init();
    //uint16_t data;
    //I2C_init();
    
    //I2C_ReadMultiRegisters(0x4F,0x00,2,(uint8_t *)&data); 
    
    /*SYSCTL->RCGCGPIO |= (1<<2) | (1<<4) | (1<<5);
    SYSCTL->RCGCTIMER |= 1<<0;
    SYSCTL->RCGCADC |= 1<<0;
    SYSCTL->RCGCPWM |= 1<<0;
    SYSCTL->RCC |= (1<<20) | (0x7<<17); // PWM clock is SycClk / 64
    
    GPIOC->AFSEL |= 1<<4;
	GPIOC->PCTL |= 4<<(4*4);
    GPIOC->DR8R |= 1<<4;
	GPIOC->DEN |= 1<<4; 
    
    GPIOE->AMSEL |= (1<<3) | (1<<2);
    GPIOC->DEN &= ~((1<<3) | (1<<2));
    
    GPIOF->PUR |= 1<<4;
    GPIOF->IM |= 1<<4;
    GPIOF->DIR |= 1<<3 | 1<<2 | 1<<1;
    GPIOF->DEN |= 1<<4 | 1<<3 | 1<<2 | 1<<1;
    GPIOF->DATA |= 1<<1;
    
    PWM0->_3_CTL = 0;
	PWM0->_3_GENA = 0x8C; 
	PWM0->_3_CTL |= 1<<0;    
	PWM0->ENABLE |= 1<<6;
    
    TIMER0->CFG = 0;            // 32bit mode
    TIMER0->TAMR |= 0x02<<0;    // periodic mode 
    TIMER0->TAILR = SystemCoreClock/16000;
    TIMER0->IMR |= 1<<0;
    TIMER0->TAV = 0;
    TIMER0->CTL |= (1<<0) | (1<<5);
    
    ADC0->EMUX |= 0x5<<0;
    ADC0->SSMUX0 |= 1<<4;
    ADC0->SSCTL0 |= (1<<5) | (1<<6);
    ADC0->ACTSS |= 1<<0;
    ADC0->IM |= 1<<0;
    
    SysTick->LOAD = SystemCoreClock / 1000 * 50;  // 50ms
    SysTick->CTRL |= 1<<0;
    
    PWM0->_3_LOAD = ((SystemCoreClock >> 6)/100) - 1;;
    PWM0->_3_CMPA = (((SystemCoreClock >> 6)/100) - 1) >> 1;*/
    
    
    while(1){
       
                
    }

    
}
