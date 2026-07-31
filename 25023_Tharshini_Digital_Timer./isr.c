/*
  File: isr.c
  Description: Contains interrupt service routines 
*/

#include "main.h"

extern unsigned char return_time;
extern unsigned char secs;
extern unsigned char blink_t;
extern unsigned char cycle_secs;
extern unsigned char buzzer_secs;
extern unsigned int alarm_remaining_secs;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    static unsigned int blink_count=0;
    
    if (TMR2IF == 1)
    {
        //blink counter
        if(++blink_count>=10000)
        {
            blink_count=0;
            blink_t=!blink_t;
        }
        if (++count == 20000)
        {
            count = 0;
            
            if(return_time>0)
            {
                return_time--;
            }   
            if(secs>0)
            {
                secs--;
            }
            //cycle_secs 0,1 -> show event (2 secs);2,3,4 -> show date/time (3 secs)
            if(++cycle_secs>=5)
            {
                cycle_secs=0;
            }
            if(BUZZER)
            {
                if(alarm_remaining_secs)
                {
                    alarm_remaining_secs--;
                }
                if(alarm_remaining_secs==0)
                {
                    BUZZER=0;//auto-off once the duration expires
                }                
            }           
        }        
        TMR2IF = 0;
    }
}