#include <xc.h>
#include "main.h"

extern volatile unsigned char one_second;

void __interrupt() isr(void)
{
   static unsigned int count = 0; 
   if(TMR1IF == 1)
   {
       /* TMR1 Register valur + 6 (offset count to get 250 ticks) + 2 Inst Cycle */
        TMR1 = TMR1 + 3036 + 2;
        
        if (++count == 80)
        {
            count = 0;
           // RD1 = !RD1;  
            one_second = 1;
        } 
   }
   TMR1IF  = 0;
}
