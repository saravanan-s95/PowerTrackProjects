/*
 * File:   main.c
 */

#include <xc.h>
#include "main.h"
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)

static void init_config(void) {
    BUZZER_DDR = 0;
    FAN_DDR = 0;
    FAN = OFF;
    BUZZER = OFF;
    
    TRISD1 = 0;
    RD1 = 0;
    
    init_clcd();
    init_matrix_keypad();
    init_timer1();
    
    GIE = 1;
    PEIE = 1;
}

void main(void) {
    unsigned char key;
    
    init_config();
    while (1) {
        key = read_matrix_keypad(STATE);  // 0 1 2 3 4 5 6 7 8 9 '*' '#'  ALL_RELEASED
        
        switch(current_state)
        {
            case POWER_ON :
                power_on_screen();
                break;
            
            case MENU :
                menu_screen(key);
                break;
                
            case MICRO :
                micro_screen();
                break;
                
            case GRILL :
                grill_screen();
                break;
                
            case CONVECTION :
                convection_screen(key);
                break;
                
            case START :
                start_screen();
                break;
            
            case SET_TIME :
                set_time_screen(key);
                break;
                
            case RUN_MODE :
                run_mode_screen(key);
                break;
                
            case PREHEAT :
                preheat_screen();
                break;
                
            case TIME_UP :
                time_up_screen();
                break;
        }
    }
    return;
}
