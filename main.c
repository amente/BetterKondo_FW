#include "i2c.h"
#include <stdint.h>
#include <stdio.h>
#include "servos.h"
#include "uart.h"

uint8_t readIndex = 0;
uint8_t frame_start = 0xFF;
#define RX_BUFF_SIZE  100
uint8_t RX_BUFF[RX_BUFF_SIZE];
int uartRead;


uint8_t mov_pos[][16] = {{89,173,173,101,88,115,112,75,103,68,129,90,82,0,0,82},
                          {89,86,171,101,88,115,112,75,103,68,129,90,82,0,91,89},
                          {49,86,171,101,88,115,112,75,103,68,129,90,82,0,91,89},
                          {163,54,171,101,88,115,112,75,103,68,129,90,82,0,91,89},
                          //{89,86,171,101,88,115,112,75,103,68,129,90,82,0,91,89},
                          {89,86,171,101,88,115,112,75,103,68,129,90,82,0,117,11},
                          {89,86,171,101,88,115,112,75,103,68,129,90,82,0,91,124},
                          {89,86,171,101,88,115,112,75,103,68,129,90,82,0,91,89},
                          };


void delay_us(uint32_t us){
    SysTick->LOAD = us*16;
    SysTick->CTRL = 1<<0;
    while(!(SysTick->CTRL & 1<<16));
    SysTick->CTRL &= ~(1<<0);
}

void delay_ms(uint32_t ms){
    delay_us(ms*1000);
}



void parseCmd(uint8_t start,uint8_t end){
    uint8_t len = end-start; 
    if(len == 3){
       servos_setDegree(RX_BUFF[start+1],1.0*RX_BUFF[start+2]);   
       printf("%d",uartRead);
    }
    frame_start = 0xFF;
}

void UART0_Rx(){    
     uartRead = uart_getbyte();     
     if(uartRead != EOF){
         RX_BUFF[readIndex] = uartRead;
         if(RX_BUFF[readIndex]==0xFF){
            frame_start = readIndex;            
         }else if(RX_BUFF[readIndex] == 0xFE){
            if(frame_start!=0xFF){
                parseCmd(frame_start,readIndex);
            }
         }
         readIndex++;
         if(readIndex==RX_BUFF_SIZE){
            readIndex = 0;
         }        
     }        
}
int main(void){    
    int i;
    int j;
    uart_init(9600);    
    servos_init();
    servos_doPosDegree(mov_pos[0]);  
    delay_ms(500);
    
    while(1)
    {
        for(i=1;i<=6;i++){       
            servos_doPosDegree(mov_pos[i]);
            delay_ms(50);
        }
        for(i=5;i>1;i--){       
            servos_doPosDegree(mov_pos[i]);
            delay_ms(50);
        }
    }
        
    
    while(1){/*
        for(i = SERVO_MIN_POS; i<SERVO_MAX_POS; i++){
            servos_setPos(3,i);
            delay_ms(20);
        }
        
        for(i = SERVO_MAX_POS; i<SERVO_MIN_POS; i--){
            servos_setPos(3,i);
            delay_ms(20);
        }   */    
    }
}
