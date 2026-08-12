/*
 * File:   isr.c
 * Author: Dell
 *
 * Created on 10 August, 2026, 9:56 PM
 */


#include "main.h"
#include "isr.h"
#include "digital_timer.h"

void __interrupt() isr(void)
{
    if(PIR1bits.TMR1IF)
    {
        // Reload Timer1
        TMR1H = 0x0B;
        TMR1L = 0xDC;

        // Clear Timer1 interrupt flag
        PIR1bits.TMR1IF = 0;

        // Count Timer1 interrupts
        static unsigned int count = 0;

        count++;

        if(count >= 100)
        {
            count = 0;

            // One second completed
            if(state == RUNNING)
            {
                if(second > 0)
                {
                    second--;
                }
                else if(minute > 0)
                {
                    minute--;
                    second = 59;
                }
            }
        }
    }
}