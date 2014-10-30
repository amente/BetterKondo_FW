#include "gpio.h"

extern void halfdup_ser_start_bit(GPIO_PIN* pin);

GPIO_PIN GPIO_PINS[6][8];

static const uint8_t hashPos[17] = {0, 0, 1,0, 2, 0, 0, 0,3,7,0,0,0,6,0,5,4}; // Magic table

void GPIOA_Handler(){
    GPIO_PIN *pin;
     pin = get_interupt_pin(0);
    if(pin->GPIO_Function == GPIO_FUNCTION_HALFDUP_SER){
        halfdup_ser_start_bit(pin);       
    }
}


void GPIOB_Handler(){
    GPIO_PIN *pin;
     pin = get_interupt_pin(1);
    if(pin->GPIO_Function == GPIO_FUNCTION_HALFDUP_SER){
        halfdup_ser_start_bit(pin);       
    }
    
}

void GPIOC_Handler(){
    GPIO_PIN *pin;
     pin = get_interupt_pin(2);
    if(pin->GPIO_Function == GPIO_FUNCTION_HALFDUP_SER){
        halfdup_ser_start_bit(pin);       
    }
    
}

void GPIOD_Handler(){
    GPIO_PIN *pin;
     pin = get_interupt_pin(3);
    if(pin->GPIO_Function == GPIO_FUNCTION_HALFDUP_SER){
        halfdup_ser_start_bit(pin);       
    }
    
}

void GPIOE_Handler(){
    GPIO_PIN *pin;
     pin = get_interupt_pin(4);
    if(pin->GPIO_Function == GPIO_FUNCTION_HALFDUP_SER){
        halfdup_ser_start_bit(pin);       
    }
    
}

void GPIOF_Handler(){
    GPIO_PIN *pin;
     pin = get_interupt_pin(5);
    if(pin->GPIO_Function == GPIO_FUNCTION_HALFDUP_SER){
        halfdup_ser_start_bit(pin);       
    }
    
}

GPIO_PIN* get_interupt_pin(uint8_t port_id){
    // Assume interrupt on only one pin
    // Uses a hash table to find the position of nth bit set
    GPIO_PIN* pin;
    switch(port_id){
        case 0 :            
            pin = &(GPIO_PINS[0][hashPos[GPIOA->MIS%17]]); 
            break;            
        case 1 :            
            pin = &(GPIO_PINS[1][hashPos[GPIOB->MIS%17]]); 
            break;
        case 2 :            
            pin = &(GPIO_PINS[2][hashPos[GPIOC->MIS%17]]); 
            break;
        case 3 :            
            pin = &(GPIO_PINS[3][hashPos[GPIOD->MIS%17]]); 
            break;
        case 4 :            
            pin = &(GPIO_PINS[4][hashPos[GPIOE->MIS%17]]); 
            break;
        case 5 :            
            pin = &(GPIO_PINS[5][hashPos[GPIOF->MIS%17]]); 
            break;        
    }
    return pin;
}

void gpio_init(void){
    SYSCTL->RCGCGPIO |= 0xFF;
    GPIO_PINS[3][0].pin_num = 0;
    GPIO_PINS[3][0].port = GPIOD;  
    GPIO_PINS[3][0].IRQn = GPIOD_IRQn;
}
