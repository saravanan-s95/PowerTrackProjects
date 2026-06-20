/*
 * File:   uart.c
 * Author: swaraj
 *
 * Created on 18 June, 2026, 11:30 AM
 */


#include <xc.h>
#include "uart.h"

void init_uart(void)
{
    // 9600 Baud 
    BRGH = 1;
    SPBRG = 39;

    SYNC = 0;
    SPEN = 1;

    TXEN = 1;
    CREN = 1;
}

void uart_putch(unsigned char data)
{
    while(!TXIF);
    TXREG = data;
}

void uart_puts(const char *str)
{
    while(*str)
    {
        uart_putch(*str++);
    }
}