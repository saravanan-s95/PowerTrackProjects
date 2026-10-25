#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "sensor.h"

extern unsigned char prev_gear_pos;
extern unsigned char dashboard_ind;

void display_dashboard()
{
    int menu_delay = 0;
    /* Address to be read */
    unsigned char address;

    /* Variables for collecting speed gear and time */
    unsigned char speed;
    unsigned char gear;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    
    CLEAR_DISP_SCREEN;
    
    clcd_print("SP",LINE1(1));
    clcd_putch('G',LINE1(4));
    clcd_print("HH:MM:ss",LINE1(8));
        
    while(1)
    {
        /* Get current gear position */
        unsigned char cur_gear_pos = get_gear_pos();
        
        /* Gear has been changed */
        if(cur_gear_pos != prev_gear_pos)
        {
            /* Update gear position and log the entry */
            prev_gear_pos = cur_gear_pos;
            
            unsigned char speed = get_speed();
            log_event(speed,cur_gear_pos);
        }
        
        /* Read the oldest available address */
        address = dashboard_ind * 5;

        /* Store the data */
        speed = read_external_eeprom(address + 0);
        gear  = read_external_eeprom(address + 1);
        hour  = read_external_eeprom(address + 2);
        min   = read_external_eeprom(address + 3);
        sec   = read_external_eeprom(address + 4);

        /* Display speed */
        for(int i=2;i>=0;i--)
        {
            clcd_putch((speed % 10) + '0', LINE2(i));
            speed /= 10;
        }

        /* Display gear */
        if(gear == 0)
        {
            clcd_putch(' ',LINE2(4));
            clcd_putch('N',LINE2(5));
        }
        else if(gear == 6)
        {
            clcd_putch(' ',LINE2(4));
            clcd_putch('R',LINE2(5));
        }
        else
        {
            clcd_putch('G',LINE2(4));
            clcd_putch(gear + '0', LINE2(5));
        }

        /* Display time */
        clcd_putch(((hour >> 4) & 0x0F) + '0', LINE2(8));
        clcd_putch((hour & 0x0F) + '0', LINE2(9));

        clcd_putch(':', LINE2(10));

        clcd_putch(((min >> 4) & 0x0F) + '0', LINE2(11));
        clcd_putch((min & 0x0F) + '0', LINE2(12));

        clcd_putch(':', LINE2(13));

        clcd_putch(((sec >> 4) & 0x0F) + '0', LINE2(14));
        clcd_putch((sec & 0x0F) + '0', LINE2(15));
        
        /* Read edge triggered matrix keypad */
        unsigned char key = read_matrix_keypad(LEVEL);

        /* Switch 11 */
        if(key == SWITCH11)
        {
            /* Long press */
            if(menu_delay++ >= LONG_PRESS)
            {
                /* Clear screen and return to main menu */
                CLEAR_DISP_SCREEN;
                return;
            }
        }
        else
        {
            menu_delay = 0;
        }
    }
}