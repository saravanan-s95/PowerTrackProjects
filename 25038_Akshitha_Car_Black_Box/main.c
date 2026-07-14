/*
 * File:   main.c
 * Author: AKSHITHA REDDY G
 Description : Main source file for the Car Black Box project.
 *             Implements event logging, RTC, EEPROM, CLCD,
 *             UART communication, and menu operations using
 *             the PIC16F877A microcontroller.
 */

#pragma config WDTE = OFF

#include <xc.h>
#include "main.h"
#include <string.h>
#include "uart.h"
#include "clcd.h"

extern unsigned char old_menu_pos;

void init_config()
{
    /*INTIAL CONFIGURATION*/
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
            /*DASHBOARD*/
            if(key == SW4)
            {
                while(read_digital_keypad(STATE) != ALL_RELEASED);  
                if(login() == 1)
                {
                    operation = 1; 
                }
                
                old_menu_pos = 255;   
                
                clcd_clear();

                while(read_digital_keypad(STATE) != ALL_RELEASED);
            }
            else
            {
                dashboard(key);
            }
        }
        else if(operation == 1)   
        {
            menu(key);
        }
    }
}
