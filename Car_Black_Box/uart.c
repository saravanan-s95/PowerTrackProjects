#include <xc.h>
#include "uart.h"

void init_uart()
{
    TRISC6 = 0;
    TRISC7 = 1;
    
    TX9 = 0; // Disable parity
    TXEN = 1; // Transmit enabled
    SYNC = 0; // Asyncronous mode
    
    BRGH = 1; // High Baud Rate 
    
    SPEN = 1; // Serial port enabled
    
    RX9 = 0; // Disable parity 
    
    CREN = 1; // Enable receiver
    
    BRG16 = 0; // 8-bit baud rate generation
    
    WUE = 0; // Disable wake up
    
    ABDEN = 0; // Disable auto baud detection 
    
    SPBRG = 129; // To achieve 9600 baud rate 
}

void transmit_char_uart(unsigned char ch)
{
    while(TXIF != 1);
    
    TXREG = ch;
}

void transmit_str_uart(unsigned char* str)
{
    while(*str)
    {
        transmit_char_uart(*str);
        str++;
    }
}

unsigned char receive_uart()
{
    while(RCIF != 1);
    
    return RCREG;
}