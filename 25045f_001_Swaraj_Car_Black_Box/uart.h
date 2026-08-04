/* 
 * File:   uart.h
 * Author: Dell
 *
 * Created on 18 June, 2026, 11:29 AM
 */

#ifndef UART_H
#define UART_H

void init_uart(void);
void uart_putch(unsigned char data);
void uart_puts(const char *str);

#endif