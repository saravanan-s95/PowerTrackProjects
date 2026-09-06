#include<xc.h>
#include "Timer.h"

void init_timer0()
{
    TMR0 = 0x06;
    
    //to select internal clock
    T0CS = 0;
    
    //no prescale 
    
    //ENABle TIMER INTERRUPT
    TMR0IE = 1;
    // clear the interrupt flag 
    TMR0IF = 0;

}
void init_timer2(void){
    TMR2ON = 1;
    PR2 = 250;
    TMR2IE =1 ;
    TMR2IF = 0;
}
void init_timer1(void)
{
    TMR1CS = 0;
    
    TMR1ON = 1;
    TMR1IE = 1;
    TMR1IF = 0;
    TMR1 = 3036;
}
