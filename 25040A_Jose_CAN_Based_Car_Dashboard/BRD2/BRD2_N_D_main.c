
#include <xc.h>
#include "1_adc_adc.h"
#include "digital_keypad.h"
#include "can_N.h"
#include "main.h"

unsigned long int speed = 0;

unsigned char switch_read = 0;
unsigned char gear = 0;

unsigned char to_transmit_speed[3];
unsigned char to_transmit_gear[1];

void main(void) 
{
    init_adc();
    
    init_digital_keypad();
    
    init_can();
    
    while(1)
    {        
        
        switch_read = read_digital_keypad(STATE_CHANGE);
        if(switch_read == SWITCH1)
        {
            if(gear < 6)
            {
                gear++;
            }
            else
            {
                gear = 6;
            }
        }
        else if(switch_read == SWITCH2)
        {
            if(gear > 0)
            {
                gear--;
            }
            else
            {
                gear = 0;
            }
        }
        
        switch(gear)
        {
            case 0: to_transmit_gear[0] = 'N';
                    break;
            case 1: to_transmit_gear[0] = 1+'0';
                    break;
            case 2: to_transmit_gear[0] = 2+'0';
                    break;
            case 3: to_transmit_gear[0] = 3+'0';
                    break;
            case 4: to_transmit_gear[0] = 4+'0';
                    break;
            case 5: to_transmit_gear[0] = 5+'0';
                    break;
            case 6: to_transmit_gear[0] = 'R';
                    break;
        }
        
        
        speed = read_adc(CHANNEL4);
        speed = ((speed*100)/1023);

        
        if(gear == 0 && speed > 0)
        {
            speed = 0;
        }
        else if(gear == 1 && speed > 15)
        {
            speed = 15;
        }
        else if(gear == 2 && speed > 40)
        {
            speed = 40;
        }
        else if(gear == 3 && speed > 65)
        {
            speed = 65;
        }
        else if(gear == 4 && speed > 80)
        {
            speed = 80;
        }
        else if(gear == 5 && speed > 100)
        {
            speed = 100;
        }
        
        
        if(speed == 100)
        {
            to_transmit_speed[0] = (1 + '0');
            to_transmit_speed[1] = (0 + '0');
            to_transmit_speed[2] = (0 + '0');
        }
        else
        {
            to_transmit_speed[0] = (0 + '0');
            to_transmit_speed[1] = ((speed/10) + '0');
            to_transmit_speed[2] = ((speed%10) + '0');
        }
        
        
        can_transmit(0x03, to_transmit_speed, SPEED_ID);
        //__delay_ms(500);
        
        can_transmit(0x01, to_transmit_gear, GEAR_ID);
        //__delay_ms(500);
    }
     
    return;
}
