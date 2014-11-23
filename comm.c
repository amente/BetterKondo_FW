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
    static uint8_t got_start;
    static int len;
    
    // read until the RX FIFO is empty
    while( EOF != (byte = uart_getbyte(COMM_UART)) )
    {
        // check if we received a start byte
        if (!got_start)
        {
            // if it's the start byte
            if ((uint8_t)byte == COMM_START)
            {
                // wait for the length
                while(EOF == (byte = uart_getbyte(COMM_UART)))
                    ;
                len = (uint8_t)((byte > COMM_BUF_SZ) ? COMM_BUF_SZ : byte);
                got_start = 1;
            }
            // otherwise we ignore the byte
            else
            {
                // discard byte
                continue;
            }
        }
        // read len-1 bytes
        else if (len-- > 1)
        {
            // save the byte
            comm_buf[comm_buf_index++] = (uint8_t)byte;
        }
        // this should be the last byte
        else
        {
            // save the last byte
            comm_buf[comm_buf_index++] = (uint8_t)byte;
            // we are done reading
            if (comm_callback != NULL)
            {
                ans = comm_callback(comm_buf, comm_buf_index);
                if (comm_buf[0] & COMM_CMD_ANS)
                {
                    // send start byte
                    uart_sendbyte(COMM_UART, 0xFE);
                    // send len
                    uart_sendbyte(COMM_UART, ans.len);
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
