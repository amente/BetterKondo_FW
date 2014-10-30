#include "CU_TM4C123.h"
#include "servos.h"

uint8_t cur_pos[16];

volatile uint32_t* ch_duty[16];

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
    int i;
    uint32_t pwm_period;
    
    ch_duty[0] = &(PWM1->_0_CMPA);
    ch_duty[1] = &(PWM1->_0_CMPB);
    ch_duty[2] = &(PWM0->_1_CMPA);
    ch_duty[3] = &(PWM0->_1_CMPB);
    ch_duty[4] = &(PWM0->_2_CMPA);
    ch_duty[5] = &(PWM0->_2_CMPB);
    ch_duty[6] = &(PWM0->_3_CMPA);
    ch_duty[7] = &(PWM0->_3_CMPB);
    ch_duty[8] = &(PWM0->_0_CMPA);
    ch_duty[9] = &(PWM0->_0_CMPB);
    ch_duty[10] = &(PWM1->_1_CMPA);
    ch_duty[11] = &(PWM1->_1_CMPB);
    ch_duty[12] = &(PWM1->_2_CMPA);
    ch_duty[13] = &(PWM1->_2_CMPB);
    ch_duty[14] = &(PWM1->_3_CMPA);
    ch_duty[15] = &(PWM1->_3_CMPB);
    
    
    //Enable clock to both PWM Modules
    SYSCTL->RCGCPWM |= 1<<0 | 1<<1; 
    SYSCTL->RCC |= (1<<20) | (0x7<<17); // PWM clock is SycClk / 64
    SYSCTL->RCGCTIMER |= 1<<2;
    
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
        //Extra pin
    GPIOB->AFSEL|= 1<<0;
    GPIOB->PCTL |= 7<<(4*0);
    GPIOB->DEN |= 1<<0;
    
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
    GPIOF->LOCK = 0x4C4F434B;  // UNLOCK PF0! 
    //lol v
    *(uint32_t*)&GPIOF->CR |= 1<<0;  // COMMIT!
    GPIOF->AFSEL |= 1<<0|1<<1|1<<2|1<<3;
    GPIOF->PCTL |= 5<<(4*0)|5<<(4*1)|5<<(4*2)|5<<(4*3);
    GPIOF->DEN |= 1<<0|1<<1|1<<2|1<<3;
     GPIOF->LOCK = 0x4C4F434B;  // UNLOCK!  
    *(uint32_t*)&GPIOF->CR &= ~(1<<0);  // UNCOMMIT!
    
    //Configure timer for PWM
    TIMER2->CFG |= 4<<0;
    //nAMS bit to 0x1, the TnCMR bit to
    //0x0, and the TnMR field to 0x2.
    TIMER2->TAMR |= 1<<3 | 2<<0;
    TIMER2->TAPR = 4;
    TIMER2->TAILR = 0xE200;

    TIMER2->TAMATCHR = 12800;
    
    TIMER2->CTL |= 1<<0;
    
    //Configure PWM Generators
    pwm_period = ((SystemCoreClock>>
    6) / PWM_FREQ) - 1;
    //PWM0
    PWM0->_0_CTL = 0;
    PWM0->_0_GENA = PWM_GEN_MODE_A;
    PWM0->_0_GENB = PWM_GEN_MODE_B;
    PWM0->_0_CTL = 1<<0;
    PWM0->_1_CTL = 0;
    PWM0->_1_GENA = PWM_GEN_MODE_A;
    PWM0->_1_GENB = PWM_GEN_MODE_B;
    PWM0->_1_CTL = 1<<0;
    PWM0->_2_CTL = 0;
    PWM0->_2_GENA = PWM_GEN_MODE_A; 
    PWM0->_2_GENB = PWM_GEN_MODE_B;
    PWM0->_2_CTL = 1<<0;
    PWM0->_3_CTL = 0;
    PWM0->_3_GENA = PWM_GEN_MODE_A;
    PWM0->_3_GENB = PWM_GEN_MODE_B;
    PWM0->_3_CTL = 1<<0;
    //PWM1
    PWM1->_0_CTL = 0;
    PWM1->_0_GENA = PWM_GEN_MODE_A; 
    PWM1->_0_GENB = PWM_GEN_MODE_B; 
    PWM1->_0_CTL = 1<<0;
    PWM1->_1_CTL = 0;
    PWM1->_1_GENA = PWM_GEN_MODE_A; 
    PWM1->_1_GENB = PWM_GEN_MODE_B;
    PWM1->_1_CTL = 1<<0;
    PWM1->_2_CTL = 0;
    PWM1->_2_GENA = PWM_GEN_MODE_A; 
    PWM1->_2_GENB = PWM_GEN_MODE_B;
    PWM1->_2_CTL = 1<<0;
    PWM1->_3_CTL = 0;
    PWM1->_3_GENA = PWM_GEN_MODE_A; 
    PWM1->_3_GENB = PWM_GEN_MODE_B;
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
    
     for(i=1;i<=16;i++){       
       servos_enable(i);
    }   
    
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





