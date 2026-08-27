/*
 * File:   uart.c
 * Author: 91902
 *
 * Created on 7 July, 2026, 2:15 PM
 */

#include<xc.h>
#include "uart.h"

void init_con_uart()
{
    //TRISC|=0xc0;
    
    TX9=0; //select 8bit transmit 
    TXEN =1; //TRANSMIT ENABLE BIT
    SYNC = 0; //MODE SELECT AS ASYNCHRONOUS
    BRGH=1;//HIGH BAUD RATE
    
    //RCSTA
    SPEN =1;//SERIAL PORT ENABLE
    RX9 = 0; //SELECT 8-BIT RECEPTION
    CREN=1; //ENABLE THE RECEIVER
    
    //BAUDCON
    //ABDEN =0; //SELECT BAUD RATE
    WUE = 0;// WAKE-UP disABLE BIT
    BRG16=0; //8-BIT BAUDRATE
    ABDEN=0; //disable auto baud detect
    
    TRISCbits.RC7=1;
    TRISCbits.RC6=0;
    
    SPBRG = 129; //to achieve 9600 baud rate
}
void char_transmit_uart(unsigned char ch)
{
    while(!TXIF);
    TXREG = ch;
}
void string_transmit_uart(unsigned char*str)
{
    while(*str != '\0')
    {
        char_transmit_uart(*str);
        str++;

    }
}
unsigned char uart_recieve()
{
    while(!RCIF);
    return RCREG;
}
