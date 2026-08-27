void timer2()
{
    
    
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
#include <xc.h>


extern int sec;

unsigned int count1,count2,count;
void __interrupt ()isr(void)
{
    
    if(TMR2IF==1)
    {
        count1++;
        if(count1==80)
        {
            count1=0;
            sec++;
        }
        TMR2IF=0;
    }
    
}