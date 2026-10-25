#include <xc.h>

char blink = 1;

void __interrupt() isr()
{
    static unsigned int count = 0;
    if(TMR2IF == 1)
    {
        if(count++ == 10000)
        {
            /* Blink clcd cursor every half second */
            blink = !blink;
            count = 0;
        }
        TMR2IF = 0;
    }
}