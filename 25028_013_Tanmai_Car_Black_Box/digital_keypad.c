#include <xc.h>
#include "main.h"

void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}
unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES;
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            return KEYPAD_PORT & INPUT_LINES;
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;
        }
    }
    return ALL_RELEASED;
}
unsigned char detect_press(unsigned char sw)
{
    unsigned int count = 0;

    while(read_digital_keypad(LEVEL_DETECTION) == sw)
    {
        count++;
        __delay_ms(1);
    }

    if(count > 500)
        return LONG_PRESS;
    else
        return NO_PRESS;
}
