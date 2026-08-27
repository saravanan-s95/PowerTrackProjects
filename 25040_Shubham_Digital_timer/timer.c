#include<xc.h>
#include "main.h"
void timer2()
{
    
    GIE=1;
    PEIE=1;
    TMR2=0;
    PR2=249;
    TMR2IE=1;
    TMR2IP=0;
    TMR2IF=0;
    
    TMR2ON =1;
    T2CKPS1=1;
    T2CKPS0=0;
    
    T2OUTPS3=1;
    T2OUTPS2=0;
    T2OUTPS1=0;
    T2OUTPS0=1;
    
}
extern unsigned int sec;

unsigned int count;
void __interrupt ()isr(void)
{
    
    if(TMR2IF==1)
    {
        clcd_putch('S',LINE2(14));
        count++;
        if(count==125)
        {
            count=0;
            sec++;
        }
        TMR2IF=0;
    }
    
}
