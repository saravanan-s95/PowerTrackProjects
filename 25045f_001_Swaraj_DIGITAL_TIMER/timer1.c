/*
 * File:   timer.c
 * Author: Dell
 *
 * Created on 10 August, 2026, 9:50 PM
 */


#include "main.h"
#include "timer1.h"

void init_timer1(void)
{
    // Stop Timer1 during initialization
    T1CON = 0x00;

    // Load initial Timer1 value
    TMR1H = 0x0B;
    TMR1L = 0xDC;

    // Clear Timer1 interrupt flag
    PIR1bits.TMR1IF = 0;

    // Enable Timer1 interrupt
    PIE1bits.TMR1IE = 1;

    // Enable peripheral interrupts
    INTCONbits.PEIE = 1;

    // Enable global interrupts
    INTCONbits.GIE = 1;

    // Start Timer1
    T1CONbits.TMR1ON = 1;
}