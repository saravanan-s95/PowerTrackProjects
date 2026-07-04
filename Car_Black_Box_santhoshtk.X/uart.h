/* 
 * File:   uart.h
 */

#ifndef UART_H
#define	UART_H

#define FOSC                20000000

void init_uart(unsigned long baud);
unsigned char get_char(void);
void put_char(unsigned char data);
void put_s(const char *s);

#endif	/* UART_H */