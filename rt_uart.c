#include <stdio.h>
#include "uart.h"

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  return (uart_sendbyte(ch));
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

void _sys_exit(int return_code) {
  while (1);    /* endless loop */
}
