
#include <xc.h>

#include "can_N3.h"
#include "main.h"
#include "A9_lcd.h"

void blink_led(unsigned char pattern);
void indicator(unsigned char received_data);

unsigned char display_title[] = "RPM  IND SPD GER";

unsigned char received_data[9];
unsigned char data_length = 0;
unsigned int msg_id = 0;
unsigned int delay = 0;
unsigned char blink = 'O';

void main(void) 
{   
    init_can();
    init_clcd();
    
    TRISB = TRISB & 0x0C;
    //PORTB = 0x00;
    
    clcd_print_line(display_title, 0x80);
    
    while(1)
    {        
        if(can_receive(&data_length, received_data, &msg_id) == TRUE)
        {
            switch(msg_id)
            {
                case RPM_ID:
                    {
                        clcd_print_line(received_data, 0xC0);
                    }
                    break;
                case IND_ID:
                    {
                        clcd_print_line(received_data, 0xC6);
                        
                        blink = received_data[0];
                    }
                    break;
                case SPEED_ID:
                    {
                        clcd_print_line(received_data, 0xC9);
                    }
                    break;
                case GEAR_ID:
                    {
                        clcd_print_line(received_data, 0xCE);
                    }
                    break;
            }
        }
        
        indicator(blink);
    }
     
    return;
}


