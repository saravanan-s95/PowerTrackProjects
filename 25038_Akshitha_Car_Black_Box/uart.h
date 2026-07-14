/* 
 * File:   uart.h
 * Author: Triveni B
 *
 */

#ifndef UART_H
#define UART_H

void init_uart(void);
void uart_putch(unsigned char data);
void uart_puts(const char *str);

#endif