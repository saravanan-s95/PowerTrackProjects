#include <xc.h>

void init_timer0(void)
{
    /* Setting the internal clock source */
    T0CS = 0;
    
    /* Assinging the prescaler to Watchdog Timer */
    PSA = 0; 
    PS2 = 1; PS1 = 0; PS0 = 1;

    TMR0 = 6;
    
    /* The timer interrupt is enabled */
    TMR0IE = 1;
    
    TMR0IF = 0;
}

