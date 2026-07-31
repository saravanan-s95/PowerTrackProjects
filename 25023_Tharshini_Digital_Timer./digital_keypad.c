/*
  File: digital_keypad.c
  Description: Handles input from digital keypad buttons
*/

#include "main.h"

void init_digital_keypad(void)
{
    //Set Keypad Port as input 
    KEYPAD_PORT_DDR |= INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    static unsigned char long_press_count=0;
    static unsigned char long_flag=0;
    static unsigned char prev_key=ALL_RELEASED;
    unsigned char key;
    
    key=KEYPAD_PORT & INPUT_LINES; //read switches
    
    if (mode == LEVEL_DETECTION)
    {
        return key;
    }
    else
    {
       //if key is pressed for short time SW_UP, SW_DOWN, SW_LEFT, SW_RIGHT
        //if key is pressed for long time key | 0x80(any of the 4 keys support long press)
       
        if(key!=ALL_RELEASED &&once)
        {
            once=0;
            prev_key=key;
            long_press_count=0;
            long_flag=0; //reset
        }
        else if(key!=ALL_RELEASED && key==prev_key && once==0)
        {
            long_press_count++; 
            //long press
            if(long_press_count>=4 && long_flag==0)
            {
                //long_press++;
                long_flag=1; //only one time
                return (prev_key | 0x80); //long press
            }
        }
        //key released
        else if(key==ALL_RELEASED && once==0)
        {
            once=1;
            //short press
            if(long_flag==0)
            {
                return prev_key; //short press            
            }    
            else
            {
                long_flag=0; //reset after long press
            }
        }
    }
    
    return ALL_RELEASED;
}
