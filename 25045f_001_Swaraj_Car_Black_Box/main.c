/*
 * File:   main.c
 * Author: Swaraj
 *
 * Created on 10 june 2026
 */

#pragma config WDTE = OFF

#include <xc.h>
#include "main.h"
#include <string.h>
#include "uart.h"

extern unsigned char old_menu_pos;

void init_config()
{
    init_adc();
    init_clcd();
    init_digital_keypad();
    init_i2c(100000); 
    init_ds1307();
    init_uart();
}

unsigned char operation = 0;

void main(void)
{
    init_config();

    unsigned char key;

    while(1)
    {
        key = read_digital_keypad(STATE);

        if(operation == 0)
        {
            // Dashboard

            if(key == SW4)
            {
                while(read_digital_keypad(STATE) != ALL_RELEASED);  //wait until key released
                if(login() == 1)
                {
                    operation = 1;  //operation 1 ->  login
                }
                
                old_menu_pos = 255;   //force to refresh 
                
                clcd_print("                ", LINE1(0));
                clcd_print("                ", LINE2(0));

                while(read_digital_keypad(STATE) != ALL_RELEASED);
            }
            else
            {
                dashboard(key);
            }
        }
        else if(operation == 1)   //if login pass success then enter into menu
        {
            menu(key);
        }
    }
}
