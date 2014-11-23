#include "CU_TM4C123.h"            // CU_SYSC4906::Device:Startup

#include <stdint.h>
#include <stdio.h>
#include "i2c.h"
#include "servos.h"
#include "uart.h"
#include "util.h"
#include "halfduplex_serial.h"
#include "comm.h"

#define VERSION 1.0

extern GPIO_PIN GPIO_PINS[6][8];

GPIO_PIN* PD0 = &(GPIO_PINS[3][0]);

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





/*
void parseCmd(uint8_t start,uint8_t end){
    uint8_t len = end-start; 
    if(len == 3){
       servos_setDegree(RX_BUFF[start+1],1.0*RX_BUFF[start+2]);    
    }else if(len==2){        
       uart_sendbytes(cur_pos,16);
    }
    frame_start = 0xFF;
}
*/

void UART0_Handler()
{
    // empty the RX FIFO
    while(!(UART0->FR & UART_FR_RXFE))
        uart_sendbyte(UART2, UART0->DR);
    // clear interrupt
    UART0->ICR |= UART_ICR_RXIC | UART_ICR_RTIC;
}

COMM_ANS comm_callback(uint8_t msg[], uint8_t len)
{
    /* ISR IS CALLING THIS FUNC SO DON'T TAKE TOO LONG! */
    
    static COMM_ANS ans;
    ans.msg = msg;
    ans.len = len;
uart_sendbytes(COMM_UART, msg, len);
uart_sendbytes(UART2, msg, len);
    // parse the msg
    switch (msg[0] & ~(COMM_CMD_ANS))
    {
        case COMM_CMD_SETPOS:
            servos_setDegree(msg[1], msg[2]);
            break;
        default:
            ;
    }
    
    // answer
    return ans;
}

int main(void){
    int i, j;
    uart_init(UART0, 9600);
    uart_int_on(UART0);
    
    comm_init(comm_callback);
    
    servos_init();
    
    for(i=1; i<=17; i++)
    {
        servos_enable(i);
        //servos_setDegree(i, mov_pos[0][i-1]);
    }
    
    while(1)
    {
        for (i=0; i<=180; i++)
        {
            //servos_setDegree(17, (float)i);
            //for(j=0; j<0xFFFF; j++);
        }
    }
}
