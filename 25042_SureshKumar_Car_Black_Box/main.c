/*
 * File:   main.c
 * Author: SureshKumar
 *
 * Created on 19 June, 2026, 3:54 PM
 */


#pragma config WDTE = OFF

#include <xc.h>
#include "main.h"
#include "uart.h"

unsigned char operation = 0;

void init_config(void)
{
    init_adc();
    init_clcd();
    init_digital_keypad();
    init_i2c(100000);
    init_ds1307();
    init_uart();
}

void main(void)
{
    unsigned char key;

    init_config();

    while(1)
    {
        key = read_digital_keypad(STATE);

        if(operation == 0)
        {
            dashboard(key);
        }
        else
        {
            menu(key);
        }
    }
}
