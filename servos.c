#include "CU_TM4C123.h"            // CU_SYSC4906::Device:Startup

#include "gpio.h"
#include "servos.h"

uint8_t cur_pos[16];

static volatile uint32_t *ch_duty[16] =
{
    &(PWM1->_0_CMPA),
    &(PWM1->_0_CMPB),
    &(PWM0->_1_CMPA),
    &(PWM0->_1_CMPB),
    &(PWM0->_2_CMPA),
    &(PWM0->_2_CMPB),
    &(PWM0->_3_CMPA),
    &(PWM0->_3_CMPB),
    &(PWM0->_0_CMPA),
    &(PWM0->_0_CMPB),
    &(PWM1->_1_CMPA),
    &(PWM1->_1_CMPB),
    &(PWM1->_2_CMPA),
    &(PWM1->_2_CMPB),
    &(PWM1->_3_CMPA),
    &(PWM1->_3_CMPB),
};

void servos_enable(uint8_t channel){
    if(channel == 1 || channel == 2){
        PWM1->ENABLE |= 1<<(channel-1);
    }else if(channel == 9 || channel == 10){
        PWM0->ENABLE |= 1<<((channel-1)%8);
    }else if(channel<=8){
        PWM0->ENABLE |= 1<<((channel-1)%8);
    }else{
        PWM1->ENABLE |= 1<<((channel-1)%8);
    }        
}

void servos_disable(uint8_t channel){
    if(channel == 1 || channel == 2){
        PWM1->ENABLE &= ~(1<<(channel-1));
    }else if(channel == 9 || channel == 10){
        PWM0->ENABLE &= ~(1<<((channel-1)%8));
    }else if(channel<=8){
        PWM0->ENABLE &= ~(1<<((channel-1)%8));
    }else{
        PWM1->ENABLE &= ~(1<<((channel-1)%8));
    }        
}

void servos_init(){
    uint32_t pwm_period;
    
    // init GPIOs
    gpio_init();
    
    // Enable clock to both PWM Modules
    SYSCTL->RCGCPWM |= SYSCTL_RCGCPWM_R0 | SYSCTL_RCGCPWM_R0;
    // PWM clock is SycClk / 64
    SYSCTL->RCC |= SYSCTL_RCC_USEPWMDIV | SYSCTL_RCC_PWMDIV_64;
    // Enable clock to Timer 2
    SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R2;
    
    // Configure alternate functions for PWM pins
    // PORT A pins: PA6 and PA7
    GPIOA->AFSEL |= 1<<6 | 1<<7;
    GPIOA->PCTL  |= GPIO_PCTL_PA6_M1PWM2 | GPIO_PCTL_PA6_M1PWM2;
    GPIOA->DEN   |= 1<<6 | 1<<7;
    // PORT B pins: PB0, PB4, PB5, PB6, PB7
    GPIOB->AFSEL |= 1<<0 | 1<<4 | 1<<5 | 1<<6 | 1<<7;
    GPIOB->PCTL  |= GPIO_PCTL_PB0_T2CCP0 |
                    GPIO_PCTL_PB4_M0PWM2 |
                    GPIO_PCTL_PB5_M0PWM3 |
                    GPIO_PCTL_PB6_M0PWM0 |
                    GPIO_PCTL_PB7_M0PWM1 ;
    GPIOB->DEN   |= 1<<0 | 1<<4 | 1<<5 | 1<<6 | 1<<7;
    // PORT C pins: PC4, PC5,
    GPIOC->AFSEL |= 1<<4 | 1<<5;
    GPIOC->PCTL  |= GPIO_PCTL_PC4_M0PWM6 | GPIO_PCTL_PC5_M0PWM7;
    GPIOC->DEN   |= 1<<4 | 1<<5;
    // PORT D pins: PD0, PD1
    GPIOD->AFSEL |= 1<<0 | 1<<1;
    GPIOD->PCTL  |= GPIO_PCTL_PD0_M1PWM0 | GPIO_PCTL_PD1_M1PWM1;
    GPIOD->DEN   |= 1<<0 | 1<<1;
    // PORT E pins: PE4, PE5
    GPIOE->AFSEL |= 1<<4 | 1<<5;
    GPIOE->PCTL  |=  GPIO_PCTL_PE4_M0PWM4 | GPIO_PCTL_PE5_M0PWM5;
    GPIOE->DEN   |= 1<<4 | 1<<5;
    // PORT F pins: PF0, PF1, PF2, PF3
    // unlock GPIOF so we can config PF0
    GPIOF->LOCK   = GPIO_LOCK_KEY;    // UNLOCK
    *(volatile uint32_t *)&GPIOF->CR |= 1<<0;
    GPIOF->AFSEL |= 1<<0 | 1<<1 | 1<<2 | 1<<3;
    GPIOF->PCTL  |= GPIO_PCTL_PF0_M1PWM4 |
                    GPIO_PCTL_PF1_M1PWM5 |
                    GPIO_PCTL_PF2_M1PWM6 |
                    GPIO_PCTL_PF3_M1PWM7 ;
    GPIOF->DEN   |= 1<<0 | 1<<1 | 1<<2 | 1<<3;
    *(volatile uint32_t *)&GPIOF->CR |= 1<<0;
    GPIOF->LOCK   = 0;                // LOCL
    
    // Configure Timer 2 for PWM
    // stop the timer
    TIMER2->CTL &= ~TIMER_CTL_TAEN;
    // select 16 bit mode (PWM)
    TIMER2->CFG |= TIMER_CFG_16_BIT;
    /*
        Set the TnAMS bit to 0x1,
                TnCMR bit to 0x0,
                TnMR field to 0x2. (p716)
    */
    TIMER2->TAMR = (TIMER2->TAMR & ~TIMER_TAMR_TACMR) |
                                    TIMER_TAMR_TAAMS  |
                                    TIMER_TAMR_TAMR_PERIOD;
    //
    TIMER2->TAPR = 4;
    //
    TIMER2->TAILR = 0xE200;
    //
    TIMER2->TAMATCHR = 12800;
    //
    TIMER2->CTL |= 1<<0;
    
    //Configure PWM Generators
    pwm_period = (SystemCoreClock / 64 / PWM_FREQ) - 1;
    //PWM0
    PWM0->_0_CTL &= ~PWM_0_CTL_ENABLE;
    PWM0->_0_GENA = PWM_GEN_MODE_A;
    PWM0->_0_GENB = PWM_GEN_MODE_B;
    PWM0->_0_CTL |= PWM_0_CTL_ENABLE;
    PWM0->_1_CTL &= ~PWM_1_CTL_ENABLE;
    PWM0->_1_GENA = PWM_GEN_MODE_A;
    PWM0->_1_GENB = PWM_GEN_MODE_B;
    PWM0->_1_CTL |= PWM_1_CTL_ENABLE;
    PWM0->_2_CTL &= ~PWM_2_CTL_ENABLE;
    PWM0->_2_GENA = PWM_GEN_MODE_A; 
    PWM0->_2_GENB = PWM_GEN_MODE_B;
    PWM0->_2_CTL |= PWM_2_CTL_ENABLE;
    PWM0->_3_CTL &= ~PWM_3_CTL_ENABLE;
    PWM0->_3_GENA = PWM_GEN_MODE_A;
    PWM0->_3_GENB = PWM_GEN_MODE_B;
    PWM0->_3_CTL |= PWM_3_CTL_ENABLE;
    //PWM1
    PWM1->_0_CTL &= ~PWM_0_CTL_ENABLE;
    PWM1->_0_GENA = PWM_GEN_MODE_A;
    PWM1->_0_GENB = PWM_GEN_MODE_B;
    PWM1->_0_CTL |= PWM_0_CTL_ENABLE;
    PWM1->_1_CTL &= ~PWM_1_CTL_ENABLE;
    PWM1->_1_GENA = PWM_GEN_MODE_A;
    PWM1->_1_GENB = PWM_GEN_MODE_B;
    PWM1->_1_CTL |= PWM_1_CTL_ENABLE;
    PWM1->_2_CTL &= ~PWM_2_CTL_ENABLE;
    PWM1->_2_GENA = PWM_GEN_MODE_A; 
    PWM1->_2_GENB = PWM_GEN_MODE_B;
    PWM1->_2_CTL |= PWM_2_CTL_ENABLE;
    PWM1->_3_CTL &= ~PWM_3_CTL_ENABLE;
    PWM1->_3_GENA = PWM_GEN_MODE_A;
    PWM1->_3_GENB = PWM_GEN_MODE_B;
    PWM1->_3_CTL |= PWM_3_CTL_ENABLE;
    
    //Set PWM Period
    PWM0->_0_LOAD = pwm_period;
    PWM0->_1_LOAD = pwm_period;
    PWM0->_2_LOAD = pwm_period;
    PWM0->_3_LOAD = pwm_period;
    PWM1->_0_LOAD = pwm_period;
    PWM1->_1_LOAD = pwm_period;
    PWM1->_2_LOAD = pwm_period;
    PWM1->_3_LOAD = pwm_period;

/* maybe not at this time
    // enable all channels
    PWM0->ENABLE =  PWM_ENABLE_PWM0EN |
                    PWM_ENABLE_PWM1EN |
                    PWM_ENABLE_PWM2EN |
                    PWM_ENABLE_PWM3EN |
                    PWM_ENABLE_PWM4EN |
                    PWM_ENABLE_PWM5EN |
                    PWM_ENABLE_PWM6EN |
                    PWM_ENABLE_PWM7EN ;
    PWM1->ENABLE =  PWM_ENABLE_PWM0EN |
                    PWM_ENABLE_PWM1EN |
                    PWM_ENABLE_PWM2EN |
                    PWM_ENABLE_PWM3EN |
                    PWM_ENABLE_PWM4EN |
                    PWM_ENABLE_PWM5EN |
                    PWM_ENABLE_PWM6EN |
                    PWM_ENABLE_PWM7EN ;
*/
}

void servos_doPosDegree(uint8_t posDegree[16]){
    int i;
    for(i=1;i<=16;i++){       
       servos_setDegree(i,(float)posDegree[i-1]);
    }     
}

void servos_setPos(uint8_t channel,uint16_t value){
    *ch_duty[channel-1] = value;   
}

void servos_setDegree(uint8_t channel,float value){
    servos_setPos(channel,SERVO_MIN_POS+ (value/180)*(SERVO_MAX_POS-SERVO_MIN_POS));
    cur_pos[channel-1] = value;
}
