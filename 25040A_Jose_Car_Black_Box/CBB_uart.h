#ifndef UART_H
#define	UART_H

#include <xc.h>

void init_uart(void);

void char_transmit_uart(unsigned char ch);      //To transmit a character
void string_transmit_uart(unsigned char* str);  //To transmit a string

unsigned char uart_receive(void);


#endif	

