#include "halfduplex_serial.h"
#include "util.h"


uint8_t HALFDUP_SER_FIFO[16];
uint8_t HALFDUP_SER_FIFO_HEAD = 0;
uint8_t HALFDUP_SER_FIFO_TAIL = 0;

uint8_t recv_data = 0;
uint8_t recv_bit_pos = 0;
uint8_t half_bit_cnt=0;
GPIO_PIN *cur_pin;

void TIMER1B_Handler(){    
    TIMER1->ICR |= TIMER_ICR_TBTOCINT; // Clear timeout interrupt 
    half_bit_cnt++;
    // Start sampling after 1.5 bit from falling edge, skip first 1/2 bit then sample on every odd count
    if(half_bit_cnt>1 && (half_bit_cnt&(1<<7))){
        if(cur_pin->port->DATA & 1<<cur_pin->pin_num){
            recv_data |= 1<<recv_bit_pos;
        }else{
            recv_data &= ~(1<<recv_bit_pos);
        }
        recv_bit_pos++;
        if(recv_bit_pos==8){
            half_bit_cnt = 0; // Reset count
            HALFDUP_SER_FIFO[HALFDUP_SER_FIFO_HEAD] = recv_data;  
            HALFDUP_SER_FIFO_HEAD++; 
            if(HALFDUP_SER_FIFO_HEAD == 16){
                 HALFDUP_SER_FIFO_HEAD = 0;
            }                
        }else{
            //Trigger next sample
            TIMER1->CTL |= TIMER_CTL_TBEN;
        }
    }
}

void halfdup_ser_init(uint16_t baud){
    // Uses TIMER1
    // enable clock to TIMER1
    SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R1;   
     // configure T1CCP1 aka Timer B for one-shot
    TIMER1->CFG = TIMER_CFG_16_BIT;   
    TIMER1->TBMR |= TIMER_TBMR_TBILD | TIMER_TBMR_TBMR_1_SHOT;
    TIMER1->IMR  |= 1<<8; // Enable timeout interrupt
    TIMER1->TBILR = baud; // Load sampling baud
}

void wait_half_bit(){
    TIMER1->IMR  &= ~(1<<8); // Disable timeout interrupt
    TIMER1->CTL |= TIMER_CTL_TBEN;
    while(!(TIMER1->RIS & TIMER_RIS_TBTORIS)){}
    TIMER1->ICR |= TIMER_ICR_TBTOCINT;
    TIMER1->IMR  |= 1<<8; // Enable timout interrupt
}

void halfdup_ser_begin(GPIO_PIN* pin){
    pin->port->AFSEL &= ~(1<<pin->pin_num); // Disable alternate function
    pin->port->DEN |= 1<<pin->pin_num;
    pin->port->PUR |= 1<<pin->pin_num; 
    pin->port->DIR &= ~(1<<pin->pin_num); // Set direction input mode
    pin->GPIO_Function = GPIO_FUNCTION_HALFDUP_SER;
    pin->port->IS &= ~(1<<pin->pin_num) ; // Clear Level sense, set Edge Sense
    pin->port->IEV &= ~(1<<pin->pin_num) ; // Set interrupt falling edge
    NVIC_EnableIRQ(pin->IRQn);  // Enable interrupt at NVIC    
    pin->port->IM |= 1<<pin->pin_num; //Enable interrupt at the GPIO  
    cur_pin = pin;
}

void halfdup_ser_start_bit(GPIO_PIN* pin){
     if(pin==cur_pin){
        // Disable further interrupt at the port
        pin->port->IM &= ~(1<<pin->pin_num);
        // Clear edge sensitive interupt;
        pin->port->ICR |= 1<<pin->pin_num;        
        // Trigger timer to start sampling
        TIMER1->CTL |= TIMER_CTL_TBEN;
     }
}

uint8_t halfdup_ser_getByte(){
    HALFDUP_SER_FIFO_TAIL++;
    if(HALFDUP_SER_FIFO_TAIL == 17){
       HALFDUP_SER_FIFO_TAIL = 1;
    }  
    return HALFDUP_SER_FIFO[HALFDUP_SER_FIFO_TAIL-1];
}

void halfdup_ser_sendByte(uint8_t byte){
    uint8_t bit_count = 1;
    cur_pin->port->IM &= ~(1<<cur_pin->pin_num); //Disable interrupt at the GPIO
    NVIC_DisableIRQ(cur_pin->IRQn);
    cur_pin->port->DIR |= 1<<cur_pin->pin_num; // Set direction to output mode
    cur_pin->port->DATA &= ~(1<<cur_pin->pin_num);   // Start Bit
    wait_half_bit();    
    while(bit_count<=8){
        wait_half_bit();  
        wait_half_bit();        
        if(byte & 1<<(bit_count-1)){
            cur_pin->port->DATA |= 1<<cur_pin->pin_num;
        }else{
            cur_pin->port->DATA &= ~(1<<cur_pin->pin_num);
        }
        bit_count++;
    } 
    wait_half_bit();
    wait_half_bit();            
    cur_pin->port->DATA |= 1<<cur_pin->pin_num;  // Stop bit
    wait_half_bit();
    wait_half_bit();
    cur_pin->port->DIR &= ~(1<<cur_pin->pin_num); // Set direction input mode
    cur_pin->port->IM |= 1<<cur_pin->pin_num; //Enable interrupt at the GPIO    
    NVIC_EnableIRQ(cur_pin->IRQn);
}
