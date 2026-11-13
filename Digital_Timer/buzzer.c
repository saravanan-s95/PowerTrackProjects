#include <xc.h>
#include "main.h"
#include "buzzer.h"

void buzzer_init(void)
{
    TRISCbits.TRISC2 = 0;
    PORTCbits.RC2 = 0;
}
void buzzer_on(void)  { PORTCbits.RC2 = 1; }
void buzzer_off(void) { PORTCbits.RC2 = 0; }

void buzzer_beep(uint16_t duration_ms)
{
    buzzer_on();
    while (duration_ms >= 10)
    {
        __delay_ms(10);
        duration_ms -= 10;
    }
    buzzer_off();
}
