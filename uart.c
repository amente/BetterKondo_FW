#include <stdio.h>
#include <stdbool.h>

#include "CU_TM4C123.h"
#include "uart.h"

void __attribute__((weak)) UART0_Rx()
{
    uart_sendstr("ERROR: Bytes received but UART0_Rx() not implemented!\n\r");
    while(UART0->MIS & (UART_MIS_RXMIS | UART_MIS_RTMIS))
        UART0->DR;  // flush the FIFO
}

void UART0_Handler()
{
    if (UART0->MIS & (UART_MIS_RXMIS | UART_MIS_RTMIS))
    {
        UART0_Rx();
        /*  Interrupt cleared by emptying FIFO  */
    }
    else
    {
        /*  Other errors  */
    }
}

/*
    Configure UART0 to do 9600 baud, 8-N-1
    UART0 uses PA0 & PA1
*/
void uart_init(uint16_t baud)
{
    // enable clock to Port A
    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R0;
    // enable clock to UART0
    SYSCTL->RCGCUART |= SYSCTL_RCGCUART_R0;
	
	// dummy read
    SYSCTL->RCGCGPIO;
    
    // select the alternate function for PA0 & PA1
    GPIOA->AFSEL |= (1<<0) | (1<<1);
	// configure the pins to be U0RX & U0TX
    GPIOA->PCTL |= GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX;
    // enable the digital pad for PA0 & PA1
    GPIOA->DEN |= (1<<0) | (1<<1);
    
    // calculate the int part of the baud rate divider
    UART0->IBRD = SystemCoreClock / (16 * baud);
    // calculate the fractional part of the baud rate divider
    UART0->FBRD = (((SystemCoreClock / (16 * baud) << 7) + 1) >> 1) & UART_FBRD_DIVFRAC_M;
    
    // configure the word length to be 8 bits
    UART0->LCRH |= UART_LCRH_WLEN_8 | UART_LCRH_FEN;
    // set the RX FIFO Trigger level
    UART0->IFLS |= FIFO_TRIG_LVL;
    // enable RX interrupts
    UART0->IM |= UART_IM_RXIM | UART_IM_RTIM;
    // enable TX and RX
    UART0->CTL |= UART_CTL_UARTEN;
    
    // enable the interrupt from the NVIC
    NVIC_EnableIRQ(UART0_IRQn);
}

uint8_t uart_sendbyte(uint8_t b)
{
    UART0->DR = b;
    while(UART0->FR & UART_FR_TXFF);    // busy wait until the 16 byte TX FIFO is no longer full
    return b;
}

void uart_sendbytes(void *buf, uint16_t len)
{
    uint8_t *bufptr = (uint8_t*) buf;
    while(len--)
    {
        uart_sendbyte(*(bufptr++));
    }
}

void uart_sendstr(void *buf)
{
    while(*(char*)buf)
    {
        uart_sendbyte(*(char*)buf);
        buf = (char*)buf + 1;
    }
}

int uart_getbyte()
{
    if (UART0->FR & UART_FR_RXFE)
    {
        return EOF;
    }
    else
    {
        return UART0->DR;
    }
}

uint16_t uart_getbytes(void *buf, uint16_t len)
{
    int c;
    uint8_t *bufptr = (uint8_t*) buf;
    while(len-- && ((c = uart_getbyte()) != EOF))
    {
        *bufptr = c;
        bufptr++;
    }
    return (uint32_t)bufptr-(uint32_t)buf;
}
