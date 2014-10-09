#include "CU_TM4C123.h"
#include "servos.h"

void servos_init(){
    uint32_t pwm_period;
    
    //Enable clock to both PWM Modules
    SYSCTL->RCGCPWM |= 1<<0 | 1<<1; 
    SYSCTL->RCC |= (1<<20) | (0x7<<17); // PWM clock is SycClk / 64
    
    //Enable clock to GPIO PORTs A,B,C,D,E,F
    SYSCTL->RCGCGPIO |= 1<<0|1<<1|1<<2|1<<3|1<<4|1<<5;
    
    //Configure alternate functions for PWM pins
    //PORT A pins, PA6 and PA7
    GPIOA->AFSEL |= 1<<6 | 1<<7;
    GPIOA->PCTL |= 5<<(4*6) | 5<<(4*7);
    GPIOA->DEN |= 1<<6 | 1<<7;
    //PORT B pins, PB6,PB7,PB4,PB5
    GPIOB->AFSEL |= 1<<6 |1<<7|1<<4|1<<5;
    GPIOB->PCTL |= 4<<(4*6)|4<<(4*7)|4<<(4*4)|4<<(4*5);
    GPIOB->DEN |= 1<<6 |1<<7|1<<4|1<<5;
    //PORT C pins, PC4,PC5,
    GPIOC->AFSEL |= 1<<4|1<<5;
    GPIOC->PCTL |= 4<<(4*4)|4<<(4*5);
    GPIOC->DEN |= 1<<4|1<<5;
    //PORT D pins, PD0,PD1
    GPIOD->AFSEL |= 1<<0|1<<1;
    GPIOD->PCTL |= 5<<(4*0)|5<<(4*1);
    GPIOD->DEN |= 1<<0|1<<1;
    //PORT E pins, PE4,PE5
    GPIOE->AFSEL |= 1<<4|1<<5;
    GPIOE->PCTL |=  4<<(4*4)|4<<(4*5);
    GPIOE->DEN |= 1<<4|1<<5;
    //PORT F pins, PF0,PF1,PF2,PF3
    GPIOF->AFSEL |= 1<<0|1<<1|1<<2|1<<3;
    GPIOF->PCTL |= 5<<(4*0)|5<<(4*1)|5<<(4*2)|5<<(4*3);
    GPIOF->DEN |= 1<<0|1<<1|1<<2|1<<3;
    
    //Configure PWM Generators
    pwm_period = ((SystemCoreClock>>
    6) / PWM_FREQ) - 1;
    //PWM0
    PWM0->_0_CTL = 0;
    PWM0->_0_GENA = PWM_GEN_MODE; 
    PWM0->_0_CTL = 1<<0;
    PWM0->_1_CTL = 0;
    PWM0->_1_GENA = PWM_GEN_MODE; 
    PWM0->_1_CTL = 1<<0;
    PWM0->_2_CTL = 0;
    PWM0->_2_GENA = PWM_GEN_MODE; 
    PWM0->_2_CTL = 1<<0;
    PWM0->_3_CTL = 0;
    PWM0->_3_GENA = PWM_GEN_MODE; 
    PWM0->_3_CTL = 1<<0;
    //PWM1
    PWM1->_0_CTL = 0;
    PWM1->_0_GENA = PWM_GEN_MODE; 
    PWM1->_0_CTL = 1<<0;
    PWM1->_1_CTL = 0;
    PWM1->_1_GENA = PWM_GEN_MODE; 
    PWM1->_1_CTL = 1<<0;
    PWM1->_2_CTL = 0;
    PWM1->_2_GENA = PWM_GEN_MODE; 
    PWM1->_2_CTL = 1<<0;
    PWM1->_3_CTL = 0;
    PWM1->_3_GENA = PWM_GEN_MODE; 
    PWM1->_3_CTL = 1<<0;
    
    //Set PWM Period
    PWM0->_0_LOAD = pwm_period;
    PWM0->_1_LOAD = pwm_period;
    PWM0->_2_LOAD = pwm_period;
    PWM0->_3_LOAD = pwm_period;
    PWM1->_0_LOAD = pwm_period;
    PWM1->_1_LOAD = pwm_period;
    PWM1->_2_LOAD = pwm_period;
    PWM1->_3_LOAD = pwm_period;
    
    //Set duty cycle
    PWM0->_0_CMPA = pwm_period>>1;
    PWM0->_1_CMPA = pwm_period>>1;
    PWM0->_2_CMPA = pwm_period>>1;
    PWM0->_3_CMPA = pwm_period>>1;
    PWM1->_0_CMPA = pwm_period>>1;
    PWM1->_1_CMPA = pwm_period>>1;
    PWM1->_2_CMPA = pwm_period>>1;
    PWM1->_3_CMPA = pwm_period>>1;
     
    //Enable all PWM channels
    PWM0->ENABLE |= 0xFF;
    PWM1->ENABLE |= 0xFF;    
    
}





