/*
 * File:   main.c
 * Author: 25014
 *
 * Created on 31 December, 2025
 */


#include "clcd.h"
#pragma config WDTE = OFF

#include <xc.h>
#include "main.h"
#include "uart.h"
#include "auth.h"


void init_config()
{
    init_adc();
    init_clcd();
    init_digital_keypad();
    init_i2c(100000); 
    init_ds1307();
    init_uart(9600);

    GIE = 1; // For TMR0 interrupt
}

void main(void) {


    init_config();
    
    unsigned char key, operation = 0;
    
    while(1)
    {
        key = read_digital_keypad(STATE);
        
        if(key == SW4)
        {
            operation = 1;
            
        }
       
        if(operation == 0)
        {
            dashboard(key);
        }
        else if(operation == 1)
        {
            main_menu();
            operation = 0;
        }
           
    }
}
