#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "ds1307.h"
#include "sensor.h"

extern unsigned char prev_gear_pos;
extern unsigned char log_count;
extern unsigned char start;

void view_log()
{
    int menu_delay = 0;
    int menu_delay2 = 0;
    unsigned char view_index = 0;
    unsigned char log_position;
    unsigned char address;

    /* Variables for storing speed gear and time(hour, minute and second) */
    unsigned char speed;
    unsigned char gear;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    
    CLEAR_DISP_SCREEN;
    
    /* Log cleared/empty */
    if(log_count == 0)
    {
        clcd_print("LOG EMPTY", LINE1(3));

        while(1)
        {
            /* Read level triggered matrix keypad */
            unsigned char key = read_matrix_keypad(LEVEL);

            /* Switch 11 */
            if(key == SWITCH11)
            {
                /* Long press */
                if(menu_delay++ >= LONG_PRESS)
                {
                    /* Return to menu */
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
            
            unsigned char speed_log = get_speed();
            log_event(speed_log,cur_gear_pos);
        }
        
        /* Start from oldest available log */
        log_position = start + view_index;

        if(log_position >= 12)
        {
            log_position = log_position - 12;
        }

        /* Get the required address to be read */
        address = log_position * 5;

        /* Read and store in variables */
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

        /* Read level triggered matrix keypad */
        unsigned char key = read_matrix_keypad(LEVEL);

        /* Switch 11 */
        if(key == SWITCH11)
        {
            /* Long press */
            if(menu_delay++ >= LONG_PRESS)
            {
                /* Clear screen and return to menu */
                CLEAR_DISP_SCREEN;
                return;
            }
        }
        
        /* Short press */
        else if(menu_delay > 0 && menu_delay < LONG_PRESS)
        {
            /* Previous log */
            if(view_index > 0)
            {
                view_index--;
            }
            menu_delay = 0;
        }
        
        else
        {
            menu_delay = 0;
        }
        
        /* Switch 12 */
        if(key == SWITCH12)
        {
            /* Long press */
            if(menu_delay2++ >= LONG_PRESS)
            {
                
            }
        }
        
        /* Short press */
        else if(menu_delay2 > 0 && menu_delay2 < LONG_PRESS)
        {
            /* Next log */
            if(view_index < (log_count - 1))
            {
                view_index++;
            }
            menu_delay2 = 0;
        }
        
        else
        {
            menu_delay2 = 0;
        }
    }
}