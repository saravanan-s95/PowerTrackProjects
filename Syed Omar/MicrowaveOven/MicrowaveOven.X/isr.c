#include <xc.h>
#include "main.h"

extern unsigned char sec, min;

void __interrupt() isr(void)
{
    static unsigned int count = 0;

    if (TMR2IF == 1)
    {
        /* 20MHz osc -> Tcy = 0.2us; Timer2 1:16 prescale -> 3.2us/tick;
         * PR2 = 250 -> 800us per interrupt; 1250 interrupts = 1 second. */
        if (++count == 1250)
        {
            count = 0;
            if (sec > 0)
            {
                sec--;
            }
            else if (min > 0)
            {
                min--;
                sec = 59;
            }
        }
        TMR2IF = 0;
    }
}

