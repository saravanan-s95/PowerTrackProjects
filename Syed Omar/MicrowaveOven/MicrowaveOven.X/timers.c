#include <xc.h>
#include "timers.h"

void init_timer2(void)
{
    /* Prescaler 1:16 */
    T2CKPS0 = 1;
    T2CKPS1 = 1;

    /* PR2 = 250 -> with 20MHz osc / 1:16 prescale -> 800us per interrupt.
     * 1250 interrupts = 1 second (see isr.c). */
    PR2 = 250;

    TMR2IE = 1;
    TMR2IF = 0;

    TMR2ON = 0; /* off until a cook/preheat cycle starts */
}

