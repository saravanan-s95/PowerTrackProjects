/* 
 * File:   uart.h
 * Author: HP
 *
 * Created on 22 June, 2026, 6:16 PM
 */

#ifndef UART_H
#define	UART_H


void init_uart(void);
void uart_putch(unsigned char data);
void uart_puts(const char *str);

#endif	/* UART_H */

