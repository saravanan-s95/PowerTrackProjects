/*
 * File:   main.c
 * Author: GANES
 *
 * Created on 3 September, 2026, 10:32 PM
 */


#include <xc.h>
#include "clcd.h"
#include "newfile.h"
#include "i2c.h"
#include "adc.h"
#include "ds1307.h"
#include "Timer.h"
#include "car_black_box.h"
#include "uart.h"

#pragma config WDTE = OFF
#include <xc.h>

char EEPROM_BUFFER[10];
unsigned char address = 0x00;

void init_config() {
    init_clcd();
    init_digital_keypad();
    init_i2c(1000000);
    init_adc();
    init_ds1307();
    init_uart(9600);
    init_timer2();
    PEIE = 1;
    GIE = 1;
    clcd_print(" TIME     EV  SP ",LINE1(0));
}

unsigned char login = 0;
void main(void) {
    init_config();
    unsigned char mode = 0;
    while (1)
    {
         unsigned char key = read_digital_keypad(STATE);
         if(key == SW4){
             clcd_print("                ",LINE1(0));
             clcd_print("                ",LINE2(0));
              login = 1;
         }
         if(login == 1){
             
             login_mode(key);
             //login = 1;
         }else{
             dash_board(key);
         }
    }
    return;
}
