/*
  File: timers.c
  Description: configures and manages hardware timers
*/

#include "main.h"


void init_timer2(void)
{
    /* Loading the Pre Load register with 250 */
    PR2 = 250;
    
    /* The timer interrupt is enabled */
    TMR2IE = 1;
       
    /* Switching on the Timer2 */
    TMR2ON = 0;
    
    TMR2IF=0;
}