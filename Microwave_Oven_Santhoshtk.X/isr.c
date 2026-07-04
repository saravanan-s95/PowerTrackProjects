#include <xc.h>
#include "main.h"

extern u_i min;
extern u_i sec;
extern u_i pause;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR0IF == 1)
    {
        /* TMR0 Register valur + 6 (offset count to get 250 ticks) + 2 Inst Cycle */
        TMR0 = TMR0 + 6 + 2;
        
        if (++count == 20000)
        {
            count = 0;
            // Completion of a second
            if(!pause){
                if(sec == 0){
                    min--;
                    sec = 59;
                }
                else{
                    sec--;
                }
            }

        }
        
        TMR0IF = 0;
    }
}