#include <xc.h>
#include "ds1307.h"
#include "external_eeprom.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "set_time.h"

unsigned char log_count = 0;
unsigned char log_index = 0;
unsigned char start = 0;
unsigned char dashboard_ind = 0;
unsigned char clock_reg[3];
unsigned char time[9];

void log_event(unsigned char speed,unsigned char gear)
{
    /* Get current time from RTC */
    get_time();
    
    /* Get the required address */
    unsigned char address = log_index * 5;   
    
    /* Write 5 bytes in sequence of speed gear and time(hour,minute and second) */
    write_external_eeprom(address+0,speed);
    __delay_ms(5);
    write_external_eeprom(address+1,gear);
    __delay_ms(5);
    write_external_eeprom(address+2,clock_reg[0]);
    __delay_ms(5);
    write_external_eeprom(address+3,clock_reg[1]);
    __delay_ms(5);
    write_external_eeprom(address+4,clock_reg[2]);
    __delay_ms(5);
    
    /* Latest index for displaying in dashboard */
    dashboard_ind = log_index;
    
    if(log_count < 12)
    {
        log_count++;
    }
    else
    {
        /* Overwriting the oldest log */
        if(start++ == 11)
        {
            start = 0;
        }
    }
    
    /* Reset log index */
    if(log_index++ == 11)
    {
        log_index = 0;
    }
}