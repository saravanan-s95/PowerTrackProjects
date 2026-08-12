/*
 * File:   timer.c
 * Author: Swaraj
 *
 * Created on 23 June, 2026, 5:53 PM
 */


#include <xc.h>
#include "timer.h"

volatile unsigned char one_sec_flag = 0;

void init_timer2(void)
{
    T2CKPS0 = 1;
    T2CKPS1 = 1;      // Prescaler 1:16

    PR2 = 249;
    TMR2 = 0;

    TMR2IF = 0;
    TMR2IE = 1;

    PEIE = 1;
    GIE = 1;

    TMR2ON = 1;
}