/* 
 * File:   uart.h
 * Author: Sundar
 *
 * Created on 12 October, 2026, 8:29 AM
 */

#ifndef UART_H
#define	UART_H

void init_uart();
void transmit_char_uart(unsigned char ch);
void transmit_str_uart(unsigned char* str);

unsigned char receive_uart();

#endif	/* UART_H */