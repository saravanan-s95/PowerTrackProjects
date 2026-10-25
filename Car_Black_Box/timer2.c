#include <xc.h>
#include "timer2.h"

void init_timer2()
{
    PR2 = 250;
    
    TMR2ON = 1;
    
    GIE = 1;
    
    PEIE = 1;
    
    TMR2IE = 1;
    
    TMR2IF = 0;
}