#include "main.h"

#define MAX_LOGS 10


/* RTC registers to store HH, MM and SS */
unsigned char clock_reg[3], speed[3] = {0};
/* Vehicle event strings */
char  *event[ ] = { "ON",  "GN", "G1", "G2", "G3", "G4", "GR", "C "};
int pos = 0;
unsigned int adc_val;


/*
 * Dashboard Screen
 * Displays:
 * Time
 * Event
 * Speed
 *
 * Handles gear changes and collision events.
 */
void dashboard(unsigned char key) {
    clcd_print(" TIME     EV  SP", LINE1(0));
    // display time
    get_time(clock_reg);
    display_time(clock_reg);
    clcd_print(" TIME     EV  SP", LINE1(0));
    /* Event Handling */
    if(key == SW1)
    {
        // collision -> GN
        if(pos == 7)
        {
            pos = 1;
        }
        // GN -> G1 -> G2 -> G3 -> G4 -> GR
        else if(pos < 6)
        {
            pos++;
        }
        // store event
        store_event();
    }
    else if(key == SW2)
    {
        // collision -> GN
        if(pos == 7)
        {
            pos = 1;
        }
        // GR -> G4 -> G3 -> G2 -> G1 -> GN 
        else if(pos > 0)
        {
            pos--;
        }
        store_event();
    }
    // collision
    else if(key == SW3)
    {
        pos = 7;
        store_event();
    }
  
    clcd_print(event[pos], LINE2(10));
    
    // speed
    adc_val = read_adc(CHANNEL0) / 10.23;
    speed[0] = ( adc_val / 10 ) + '0';
    speed[1] = ( adc_val % 10 ) + '0';
    speed[2] = '\0';

    clcd_print(speed, LINE2(14));
}


void store_event()
{
    /* EEPROM write address */
    static unsigned char address = 0;
    /* Store HHMMSS (without ':') */
    for(int i = 0; i < 8; i++)
    {
        if(i==2 || i == 5)
        {
            continue;
        }
        write_external_eeprom(address++,time[i]);
    }
    /* Store event */
    write_external_eeprom(address++, event[pos][0]);
    write_external_eeprom(address++, event[pos][1]);
    /* Store speed */
    write_external_eeprom(address++, speed[0]);
    write_external_eeprom(address++, speed[1]);
    /* Circular buffer for 10 logs */
    if(address > 99)
    {
        address = 0;
    }
    /* Maintain event count */
    if(event_count < MAX_LOGS)
    {
        event_count++;
    }
}



