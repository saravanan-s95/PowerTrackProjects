/*
 * File:   main.c
 * Author: GANES
 *
 * Created on 30 August, 2026, 9:26 AM
 */


#include <xc.h>
#include "Timer.h"
#include "clcd.h"
#include "digital_timer.h"
#include "ds1307.h"
#include "i2c.h"
#include "newfile.h"


#pragma config WDTE = OFF

void init_config() {
    init_clcd();
//    clcd_print("HELLO", LINE1(0));
//    while (1);
    init_digital_keypad();
    init_i2c(1000000);
    //init_adc();
    init_ds1307();
    //init_uart(9600);
    init_timer1();
    //init_timer2();
    PEIE = 1;
    GIE = 1;
    clcd_print("DATE",LINE1(0));
    clcd_print("TIME",LINE2(0));
}
unsigned char choose =0;
unsigned char menu_inside =0;
void main(void) {
    init_config();

    while (1){
        unsigned char key = read_digital_keypad(STATE);
        
        if((key == SW1 || key == SW2) && choose == 0){
             clcd_print("                ",LINE1(0));
             clcd_print("                ",LINE2(0));
             choose = 1;
             menu_inside = 0;
        }
        if(choose == 1){
           // menu_inside = 1;
            menu(key);
        }
        else{
            dashboard(key);
        }
    }
    return;
}
