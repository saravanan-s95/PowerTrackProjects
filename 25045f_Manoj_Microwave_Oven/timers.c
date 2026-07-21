#include <xc.h>

void init_timer1(void){
    
    /* Setting the internal clock source */
    TMR1CS = 0;
    
    /*Timer1 On bit*/
    TMR1ON = 1;
    
    TMR1 = 3036;
    
    
    T1OSCEN = 0;
    TMR1IF = 0;
    /* Setting the scale to 1:16 */
    T1CKPS1 = 0;
    T1CKPS0 = 0;
    
    /* The timer interrupt is enabled */
    TMR1IE = 1;
}
