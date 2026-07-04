/*
 * File:   main.c
 * Author: Santhosh T K
 *
 * Created on July 4, 2026, 1:56 PM
 */


#include <xc.h>
#include "main.h"

#pragma config WDTE = OFF

static void init_config(void) {

    init_clcd();
    init_matrix_keypad();
    init_timer0();
    init_buzz();
    init_fan();
    // DEBUG_DDR = 0;

}

void main(void) {

    init_config();
    power_on_screen();

    while (1) {

        main_menu_screen();

    }

    return;
}
