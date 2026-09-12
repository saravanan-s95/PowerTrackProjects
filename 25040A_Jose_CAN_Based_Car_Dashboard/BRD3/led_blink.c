
#include <xc.h>

extern unsigned int delay;

void blink_led(unsigned char pattern)
{
    delay++;
    if(delay <= 30000)
    {
        PORTB = pattern;
    }
    else if(delay <= 60000)
    {
        PORTB = 0x00;
    }
    else
    {
        delay = 0;
    }
}


void indicator(unsigned char received_data)
{
    if(received_data == 'L')
    {
        blink_led(0x03);
    }
    else if(received_data == 'R')
    {
        blink_led(0xC0);
    }
    else if(received_data == 'H')
    {
        blink_led(0xC3);
    }
    else if(received_data == 'O')
    {
        PORTB = 0x00;
    }
}