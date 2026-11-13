#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"

static const char keymap[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

void keypad_init(void)
{
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
}

char keypad_scan(void)
{
    uint8_t row;
    for (row = 0; row < 4; row++)
    {
        PORTB |= 0xF0;
        if (row == 0) PORTBbits.RB4 = 0;
        if (row == 1) PORTBbits.RB5 = 0;
        if (row == 2) PORTBbits.RB6 = 0;
        if (row == 3) PORTBbits.RB7 = 0;
        __delay_us(30);

        if (!PORTAbits.RA0) return keymap[row][0];
        if (!PORTAbits.RA1) return keymap[row][1];
        if (!PORTAbits.RA2) return keymap[row][2];
        if (!PORTAbits.RA3) return keymap[row][3];
    }
    return 0;
}

char keypad_get_event(void)
{
    char key;
    uint16_t held = 0;

    do { key = keypad_scan(); } while (!key);
    __delay_ms(25);
    if (keypad_scan() != key) return 0;

    while (keypad_scan() == key)
    {
        __delay_ms(10);
        if (held < 1000) held++;
    }
    __delay_ms(20);

    if (held >= 70)
    {
        if (key == '2') return 'U';
        if (key == '8') return 'V';
        if (key == '5') return 'S';
        if (key == 'D') return 'd';
    }
    return key;
}

void keypad_wait_release(void)
{
    while (keypad_scan())
        __delay_ms(10);
}
