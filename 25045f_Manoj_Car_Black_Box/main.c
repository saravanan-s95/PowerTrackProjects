/*
 * File:   main.c
 * Author: 25014
 *
 * Created on 31 December, 2025
 */

#pragma config WDTE = OFF

#include <xc.h>
#include "main.h"
#include "uart.h"

void init_config()
{
    init_adc();
    init_clcd();
    init_digital_keypad();
    init_i2c(100000); 
    init_ds1307();
    init_uart(9600);
}

void main(void) {
    init_config();
    
    unsigned char key, operation = 0;
    
    while(1)
    {
        key = read_digital_keypad(STATE);

        if(operation == 0)
        {
            dashboard(key);

            if(key == SW4)
            {
                operation = 1;
            }
        }
        else if(operation == 1)
        {
            unsigned char login_status = login_screen();

            if(login_status == 1){
             //   clcd_print("yes yes", LINE1(0));
                operation = 2; 
            }else
                operation = 0;   
        }
        else if(operation == 2)
        {
            unsigned char ret = main_menu();

            if(ret == 1)
            {
                operation = 0;
            }
        }
    }
}
