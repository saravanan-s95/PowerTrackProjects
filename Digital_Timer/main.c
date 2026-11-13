/*
 * Project Name : Digital Timer
 * Microcontroller : PIC16F877A
 * Language : Embedded C
 * IDE : MPLAB X IDE 6.35
 * Compiler : XC8 v4.00
 *
 * Description:
 * Digital clock and alarm system using LCD,
 * matrix keypad and DS1307 RTC.
 *
 * Author : Sheeba Insily R
 */

#include "main.h"
#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "matrix_keypad.h"
#include "buzzer.h"
#include "alarm.h"
#include "menu.h"

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

static void display_default(const RTC_Time *now)
{
    clcd_goto(0,0);
    clcd_print("TIME ");
    clcd_print2(now->hour); clcd_putc(':');
    clcd_print2(now->min); clcd_putc(':'); clcd_print2(now->sec);
    clcd_goto(1,0);
    clcd_print("DATE ");
    clcd_print2(now->date); clcd_putc('/');
    clcd_print2(now->month); clcd_putc('/');
    clcd_print2(now->year);
}

void main(void)
{
    RTC_Time now;
    uint8_t previous_second = 255;

    ADCON1 = 0x06;        /* Configure analog-capable pins as digital */
    TRISD = 0x00;         /* LCD data */
    TRISBbits.TRISB0 = 0; /* LCD RS */
    TRISBbits.TRISB1 = 0; /* LCD EN */
    TRISBbits.TRISB4 = 0; TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0; TRISBbits.TRISB7 = 0;
    TRISA = 0x0F;         /* keypad columns */
    TRISCbits.TRISC2 = 0;

    PORTA = 0x0F; PORTB = 0xF0; PORTC = 0; PORTD = 0;

    clcd_init();
    keypad_init();
    buzzer_init();
    ds1307_init();
    alarm_init();

    if (!ds1307_read(&now))
    {
        now.sec=0; now.min=0; now.hour=12;
        now.weekday=1; now.date=1; now.month=1; now.year=26;
        ds1307_write(&now);
    }

    while (1)
    {
        char key;
        ds1307_read(&now);
        if (now.sec != previous_second)
        {
            previous_second = now.sec;
            clcd_clear();
            display_default(&now);
            alarm_check(&now);
        }

        key = keypad_scan();
        if (key == '2' || key == '8')
        {
            keypad_wait_release();
            menu_config();
            previous_second = 255;
        }
        __delay_ms(50);
    }
}
