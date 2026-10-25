/*
 * File:   main.c
 * Author: Sundar
 *
 * Created on 29 September, 2026, 9:07 AM
 */


#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include "i2c.h"
#include "sensor.h"
#include "ds1307.h"
#include "timer2.h"
#include "uart.h"

unsigned char prev_gear_pos = 0;

void main(void) {
    /* Initialize the peripherals */
    init_adc();
    init_matrix_keypad();
    init_clcd();
    init_i2c();
    init_ds1307();
    init_timer2();
    init_uart();
   
    int delay = 0;
    int cursor = 1;
    
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
        
        /* Main menu display */
        clcd_print("DASHBOARD",LINE1(3));
        clcd_print("MENU",LINE2(3));
        
        /* Based on cursor value display the cursor */
        if(cursor == 1)
        {
            clcd_print("->",LINE1(0));
            clcd_print("  ",LINE2(0));
        }
        else
        {
            clcd_print("  ",LINE1(0));
            clcd_print("->",LINE2(0));
        }
        
        /* Read level triggered matrix keypad */
        unsigned char key = read_matrix_keypad(LEVEL);
        
        /* Switch 12 */
        if(key == SWITCH12)
        {
            /* Long press */
            if(delay++ >= LONG_PRESS)
            {
                /* Change display */
                if(cursor == 1)
                {
                    /* Dashboard */
                    display_dashboard();
                }
                else
                {
                    /* Menu */
                    display_menu();
                }
            }
        }
        
        /* Short press */
        else if(delay > 0 && delay < LONG_PRESS)
        {
            /* Change line flag */
            if(cursor == 1)
            {
                cursor = 2;
            }
            else
            {
                cursor = 1;
            }
            delay = 0;
        }
        
        else
        {
            delay = 0;
        }
    }
}