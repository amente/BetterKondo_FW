#include "CU_TM4C123.h"                 // CU_SYSC4906::Device:Startup
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "comm.h"

/* ISR IS CALLING THIS FUNC SO DON'T TAKE TOO LONG! */
static COMM_ANS (*comm_callback)(uint8_t msg[], uint8_t len) = NULL;
static uint8_t comm_buf[COMM_BUF_SZ], comm_buf_index = 0;

void UART2_Handler()
{ 
    int byte;
    COMM_ANS ans;
    static uint8_t got_start, len;
    
    // read until the RX FIFO is empty
    while( EOF != (byte = uart_getbyte(COMM_UART)) )
    {
        // check if we received a start byte
        if (!got_start)
        {
            // if it's not a start byte
            if ((uint8_t)byte != COMM_START)
            {
                // discard byte
                continue;
            }
            else
            {
                // wait for the length
                while(EOF == (byte = uart_getbyte(COMM_UART)))
                    ;
                len = (uint8_t)byte;
                got_start = 1;
            }     
        }
        // check if we've read all the bytes 
        else if( (len-- > 0) && (comm_buf_index < COMM_BUF_SZ) )
        {
            // save the byte
            comm_buf[comm_buf_index++] = (uint8_t)byte;
        }
        else
        {
            // we are done reading
            if (comm_callback != NULL)
            {
                ans = comm_callback(comm_buf, comm_buf_index + 1);
                if (comm_buf[0] & COMM_CMD_ANS)
                {
                    // send start byte
                    uart_sendbyte(COMM_UART, 0xFE);
                    // send len
                    uart_sendbyte(COMM_UART, 0xFE);
                    // send the rest of the answer
                    uart_sendbytes(COMM_UART, ans.msg, ans.len);
                }
            }
            // reset index
            comm_buf_index = 0;
            // reset got_start
            got_start = 0;
        }
    }
}

void comm_init(COMM_ANS (*cb)(uint8_t msg[], uint8_t len))
{
    // uart init
    uart_init(COMM_UART, COMM_BAUD);
    // enable uart interrupt
    uart_int_on(COMM_UART);
    // set comm callback
    comm_callback = cb;
}
