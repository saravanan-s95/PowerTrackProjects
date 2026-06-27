/*
 * Name : Anjana V
 * Description : The Car Black Box is an embedded system project developed using 
 *               the PIC16F877A microcontroller to monitor and record important 
 *               vehicle events. The system continuously displays the current time, 
 *               vehicle event status, and speed on a 16x2 CLCD.The project uses 
 *               a DS1307 Real Time Clock (RTC) to maintain accurate time and an 
 *               external EEPROM to store event logs even after power loss. Vehicle 
 *               events such as ON, GN, G1, G2, G3, G4, GR, and Collision (C) are 
 *               recorded with the corresponding time and speed values. The speed is obtained through the ADC module of the PIC microcontroller. I
 *
 *  PASSWORD : 1100
 */

#pragma config WDTE = OFF

#include "main.h"


unsigned char operation = 0;

/* Initialize all peripherals */
void init_config()
{
    init_adc();
    init_clcd();
    init_digital_keypad();
    init_i2c(100000); 
    init_ds1307();
    init_uart();
}

/* 0 -> Dashboard
 * 1 -> Menu
 */
void main(void) {
    init_config();
    unsigned char key;
    unsigned char ret;
    
    while(1)
    {
        /* Read key continuously */
        key = read_digital_keypad(STATE);
        /* Dashboard */
        if(operation == 0)
        {
            dashboard(key);
            /* SW4 -> Enter Login Screen */
            if(key == SW4)
            {
                clcd_print("                ", LINE1(0));
                clcd_print("                ", LINE2(0));

                while(1)
                {
                    key = read_digital_keypad(STATE);
                    ret = login(key);
                    /* Password correct */
                    if(ret == 1)      // password correct
                    {
                        operation = 1; // menu
                        break;
                    }
                    /* Login cancelled */
                    else if(ret == 2)
                    {
                        operation = 0;      // Back to Dashboard
                        break;
                    }
                }
            }
        }
        else if(operation == 1)
        {
            menu(key);
        }
    }
}
