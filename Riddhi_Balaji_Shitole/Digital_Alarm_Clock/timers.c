#include "main.h"

unsigned int seconds = 0;
unsigned char screen_flag = 0;
void init_timer()
{
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;
    
    TMR0IF = 0;
    T0CS = 0;
    PSA = 1;
    TMR0 = 6;
}

void __interrupt() isr(void)
{
    static unsigned int count = 0;

    if(TMR0IF)
    {
        TMR0 = TMR0 + 8;

        count++;

        if(count >= 20000)
        {
            count = 0;

            seconds++;

            if(seconds >= 20)
            {
                screen_flag = 1;
            }
        }

        TMR0IF = 0;
    }
}

