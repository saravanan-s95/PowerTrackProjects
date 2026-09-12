#include <xc.h>

#include "CBB_uart.h"


//Function ti initialize UART protocol
void init_uart(void)
{
    //TXSTA
    TX9 = 0;        //Disabling parity
    TXEN = 1;       //Enable transmitting
    SYNC = 0;       //Select asynchronous mode
    BRGH = 1;       //High speed
    
    //RCSTA
    SPEN = 1;       //Serial Port enable
    RX9 = 0;        //Disabling parity 
    CREN = 1;       //Enable receiver
    
    //BAUDCON
    BRG16 = 0;      //For n=129, we only need 8-bit register
    WUE = 0;      //Disabling wakeup enable
    ABDEN = 0;      //Disabling auto baud detect
    
    //PORT CONFIGURATION
    TRISC6 = 0;     //Transmit 
    TRISC7 = 1;     //Receive
    
    //ASSIGN BAUD-RATE 
    SPBRG = 129;    //To achieve 9600 baud rate
}


//Function to transmit a character (a byte) using UART Protocol
void char_transmit_uart(unsigned char ch)
{
    while(!TXIF);   //Wait till TXIF become 1 (ie., TXREG empty)
    
    TXREG = ch;
}


//Function to transmit a string using UART Protocol
void string_transmit_uart(unsigned char* str)
{
    while(*str != '\0')
    {
        char_transmit_uart(*str);
        str++;
    }
}
