#include "i2c.h"
#include <stdint.h>
#include <stdio.h>
#include "servos.h"
#include "uart.h"
#include "util.h"
#include "halfduplex_serial.h"

extern GPIO_PIN GPIO_PINS[6][8];

GPIO_PIN* PD0 = &(GPIO_PINS[3][0]);

uint8_t readIndex = 0;
uint8_t frame_start = 0xFF;
#define RX_BUFF_SIZE  100
uint8_t RX_BUFF[RX_BUFF_SIZE];
extern uint8_t cur_pos[16];
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



/*uint8_t mov_pos[][16] = {{96,171,173,89,88,115,112,83,94,68,129,90,85,0,0,89},                         
                         {96,150,173,89,88,115,112,72,93,68,129,90,85,0,28,89},
                         {96,168,173,89,98,115,112,72,93,68,129,90,85,0,37,89},
                         {96,168,180,86,98,115,112,72,93,68,129,90,85,47,7,89},
                         {96,168,180,86,105,115,112,72,93,68,129,90,77,47,7,89}                         
                          };*/






void parseCmd(uint8_t start,uint8_t end){
    uint8_t len = end-start; 
    if(len == 3){
       servos_setDegree(RX_BUFF[start+1],1.0*RX_BUFF[start+2]);    
    }else if(len==2){        
       uart_sendbytes(cur_pos,16);
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
    
    gpio_init();
    
    halfdup_ser_init(HALFDUP_SER_BAUD_1200);
    halfdup_ser_begin(PD0);     
    while(1){
        halfdup_ser_sendByte(0x88);  
        for(i=0;i<0xFFFF;i++);
    }
       
    /*uart_init(9600);    
    servos_init();
    
    
    servos_doPosDegree(mov_pos[0]);  
    delay_ms(200);
    
    servos_doPosDegree(mov_pos[1]);  
    delay_ms(200);
    servos_doPosDegree(mov_pos[2]);  
    delay_ms(200);
    servos_doPosDegree(mov_pos[3]);  
    delay_ms(200);
    servos_doPosDegree(mov_pos[4]);  
    delay_ms(200);

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
        }     
    }*/
}
