
#include <xc.h>

#include "1_adc_adc.h"
#include "digital_keypad.h"
#include "main.h"
#include "can_N.h"

unsigned int rpm = 0;
unsigned int old_rpm = 0;

unsigned char switch_read = 0;
unsigned char current_switch = 0;

unsigned char to_transmit_rpm[4];
unsigned char to_transmit_indicator[1];

void main(void) 
{
    init_adc();
    
    init_digital_keypad();

    init_can();
    
    while(1)
    {
        
        old_rpm = rpm;
        rpm = ((read_adc(CHANNEL4)/10.23)*60);

        if(rpm != old_rpm)
        {
            to_transmit_rpm[0] = ((rpm/1000) + '0');
            to_transmit_rpm[1] = ((rpm/100)%10 + '0');
            to_transmit_rpm[2] = ((rpm/10)%10 + '0');
            to_transmit_rpm[3] = (rpm%10 + '0');
        }
        
        
        
        switch_read = read_digital_keypad(STATE_CHANGE);
        
        if(switch_read != ALL_RELEASED && switch_read != 0xFF)
        {
            current_switch = switch_read;
        }
        
        if(current_switch == SWITCH1)
        {
            to_transmit_indicator[0] = 'L';     //Left
        }
        else if(current_switch == SWITCH2)
        {
            to_transmit_indicator[0] = 'O';     //Off
        }
        else if(current_switch == SWITCH3)
        {
            to_transmit_indicator[0] = 'H';    //Hazard
        }
        else if(current_switch == SWITCH4)
        {
            to_transmit_indicator[0] = 'R';    //Right
        }
       
        
        can_transmit(0x04, to_transmit_rpm, RPM_ID);
        //__delay_ms(500);
        
        can_transmit(0x01, to_transmit_indicator, IND_ID);
        //__delay_ms(500);
    }
    
    return;
}
