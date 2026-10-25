#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "sensor.h"

extern unsigned char prev_gear_pos;

void display_menu()
{
    int menu_line_flag = 1;
    int menu_cursor = 1;
    int menu_delay = 0;
    int menu_delay2 = 0;
    
    CLEAR_DISP_SCREEN;
    
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
        
        /* Based on line flag display accordingly */
        if(menu_line_flag == 1)
        {
            clcd_print("VIEW LOG",LINE1(3));
            clcd_print("CLEAR LOG",LINE2(3));
        }
        else if(menu_line_flag == 2)
        {
            clcd_print("CLEAR LOG",LINE1(3));
            clcd_print("DOWNLOAD LOG",LINE2(3));
        }
        else
        {
            clcd_print("DOWNLOAD LOG",LINE1(3));
            clcd_print("SET TIME",LINE2(3));
        }
        
        /* Based on cursor value display cursor */
        if(menu_cursor == 1)
        {
            clcd_print("->",LINE1(0));
            clcd_print("  ",LINE2(0));
        }
        else
        {
            clcd_print("  ",LINE1(0));
            clcd_print("->",LINE2(0));
        }

        unsigned char key = read_matrix_keypad(LEVEL);

        /* SWITCH11 = UP */
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
        
        /* Short press */
        else if(menu_delay > 0 && menu_delay < LONG_PRESS)
        {
            /* Move UP */
            if(menu_cursor == 2)
            {
                menu_cursor = 1;
            }
            
            else if(menu_line_flag != 1)
            {
                CLEAR_DISP_SCREEN;
                menu_line_flag--;
            }
            menu_delay = 0;
        }
        
        else
        {
            menu_delay = 0;
        }
                
        /* SWITCH12 = DOWN / SELECT */
        if(key == SWITCH12)
        {
            /* Long press */
            if(menu_delay2++ >= LONG_PRESS)
            {
                /* Select operation */
                if(menu_line_flag == 1 && menu_cursor == 1)
                {
                    /* View log */
                    view_log();
                }
                else if(menu_line_flag == 1 && menu_cursor == 2 || menu_line_flag == 2 && menu_cursor == 1)
                {
                    /* Clear log */
                    clear_log();
                }
                else if(menu_line_flag == 2 && menu_cursor == 2 || menu_line_flag == 3 && menu_cursor == 1)
                {
                    /* Download log */
                    download_log();
                }
                else
                {
                    /* Set time */
                    set_time();
                }
            }
        }
        
        /* Short press */
        else if(menu_delay2 > 0 && menu_delay2 < LONG_PRESS)
        {
            /* Move DOWN */
            if(menu_cursor == 1)
            {
                menu_cursor = 2;
            }
            else if(menu_line_flag != 3)
            {
                CLEAR_DISP_SCREEN;
                menu_line_flag++;
            }

            menu_delay2 = 0;
        }
        
        else
        {
            menu_delay2 = 0;
        }
    }
}