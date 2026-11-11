#include <xc.h>
#include "digital_keypad.h"


void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;  //TRISB = TRISB | 0x0F
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES;            //PORTB & 0x0F [PORTB = sw1-sw4]
    }
    
    else
    { 
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)   //PORTB & 0x0F != 0x0F && 1
        {
            once = 0;
            
            return KEYPAD_PORT & INPUT_LINES;                        
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)        //PORTB & 0x0F == 0x0F 
        {
            once = 1;
        }
    }
    
    return ALL_RELEASED;                                            //0x0F
}


