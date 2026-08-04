/*
 * File:   isr.c
 * Author: Swaraj
 *
 * Created on 23 June, 2026, 5:55 PM
 */


#include <xc.h>
#include "timer.h"

void __interrupt() isr(void)
{
    static unsigned int count = 0;

    if(TMR2IF)
    {
        TMR2IF = 0;

        count++;

        if(count >= 1250)
        {
            count = 0;
            one_sec_flag = 1;
        }
    }
}