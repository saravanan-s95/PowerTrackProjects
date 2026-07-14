#include <xc.h>

extern signed char min, sec;
extern unsigned char timer_flag;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {
        if (++count == 1250)
        {
            if((sec - 1) < 0)
            {
                if((min - 1) < 0)
                {
                    timer_flag = 2;
                    RC2 = 0;
                }
                else
                {
                    --min;
                    sec = 59;
                }
            }
            else
            {
                --sec;
            }
            count = 0;
        }
        TMR2IF = 0;
    }
}
