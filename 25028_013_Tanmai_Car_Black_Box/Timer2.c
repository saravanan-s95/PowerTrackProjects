#include "main.h"
#include <xc.h>

volatile unsigned char sec = 0;
volatile unsigned char inactive_time = 5;
void init_timer2()
{
    TIMER2_PRESCALE0 = 1; // 1:16 pre-scaler
    TIMER2_PRESCALE1 = 1;
    
    TIMER2_PERIOD = 249;
    TIMER2_INTERRUPT_FLAG = 0;
    TIMER2_INTERRUPT_ENABLE = 1;
}

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    if(TIMER2_INTERRUPT_FLAG)
    {
        TIMER2_INTERRUPT_FLAG = 0;
        count++;
        if(count == 1250)
        {
            if (sec > 0)
            {
                sec--;
            }
            else if(sec == 0 && inactive_time > 0)
            {
                inactive_time--;
            }
            count = 0;      
        }
    }
}
