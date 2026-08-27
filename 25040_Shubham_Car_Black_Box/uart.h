#ifndef UART_H
#define UART_H
void init_con_uart();
void char_transmit_uart(unsigned char ch);
void string_transmit_uart(unsigned char* str);


unsigned char uart_recieve();
#endif