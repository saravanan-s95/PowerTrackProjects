#include <xc.h>
#include "timers.h"

extern volatile unsigned char sec_tick;          // Flag to indicate 1 second completed
extern volatile unsigned int power_on_count;     // Millisecond counter for debouncing and delays

// Interrupt Service Routine - executes when any interrupt occurs
void __interrupt() isr(void)
{
    if(TMR0IF == 1)   //check if TIMER0 overflow interrupt occurs
    {
        TMR0IF = 0;   // Clear Timer0 interrupt flag (must clear in software)
        TMR0 = 6;     // Reload Timer0 value with 6 for next 1ms overflow (for 20MHz with prescaler)
        
        static unsigned int t0_count = 0;  // Static counter to count 1ms ticks
        t0_count++;
        power_on_count++;                  // Increment every 1ms, used for key debounce timing
        
        if(t0_count >= 625) 
        {
            t0_count = 0;                  // Reset counter
            sec_tick = 1;                  // Set 1 second flag for main loop to update time
        }
    }
}