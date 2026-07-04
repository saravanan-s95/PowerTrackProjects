#include <xc.h>
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "operating_modes.h"
#include "dashboard.h"

#define FULL_DOT    0xFF

void breakpoint(void){
    DEBUG = 0;
    __delay_ms(2000);
    DEBUG = 1;
}

u_i min = 0;
u_i sec = 0;
u_i pause = 0;


void power_on_screen(void){

    // First Line
    for(u_i i = 0; i < 16; i++){
        clcd_putch(FULL_DOT, LINE1(i));
        __delay_ms(100);
    }

    // Second Line
    clcd_print("Powering ON", LINE2(2));

    // Third Line
    clcd_print("Microwave Oven", LINE3(1));

    // Fourth Line
    for(u_i i = 0; i < 16; i++){
        clcd_putch(FULL_DOT, LINE4(i));
        __delay_ms(100);
    }

    __delay_ms(500);
    return;
}


void main_menu_screen(void){

    clear_screen();
    unsigned char key;

    while(1){

        // Display menu options
        clcd_print("1. Micro", LINE1(0));
        clcd_print("2. Grill", LINE2(0));
        clcd_print("3. Convection", LINE3(0));
        clcd_print("4. Start", LINE4(0));

    
        
        key = read_matrix_keypad(STATE);

        switch(key){

            case 1:
                micro_mode();
                clear_screen();
                break;
            
            case 2:
                grill_mode();
                clear_screen();
                break;
            
            case 3:
                convection_mode();
                clear_screen();
                break;
            
            case 4:
                start_mode();
                clear_screen();
                break;
        }
    }
}



void set_time_screen(void){
    
    clear_screen();
    clcd_print("SET TIME <MM:SS>", LINE1(0));
    clcd_print("*:CLEAR  #:ENTER", LINE4(0));
    unsigned char key;
    unsigned char min_digits = 0;
    unsigned char sec_digits = 0;

    // If not reset, the state will be preserved on
    // next call to this function.
    min = 0;
    sec = 0;
    pause = 0;

    /*
    * Maximum safe time limit for Continious Operation of Microwave oven is
    * 30 Minutes to prevent component failure. 
    */

    unsigned char field_choosen = 0;

    while(1){

        key = read_matrix_keypad(STATE);

        if(key != ALL_RELEASED){
            // breakpoint();

            if(key == '*'){
                if(field_choosen) {
                    min = 0;
                    min_digits = 0;
                }
                else{
                    sec = 0;
                    sec_digits = 0;
                } 
            }
            else if(key == '#'){
                // Enter into running mode
                
                return;
            }
            else{
                if(field_choosen == 0){
                    // seconds
                    if(sec_digits < 2){
                        sec = (sec * 10) + key;
                        sec_digits++;

                        if(sec >= 60) sec = 60;
                    }

                    // If two digits are entered, switch to minutes
                    if(sec_digits == 2) field_choosen = 1;
                }
                else if(field_choosen == 1){
                    // Minutes
                    if(min_digits < 2){
                        min = (min * 10) + key;
                        min_digits++;

                        if(min >= 30){
                            min = 30;
                            sec = 0;
                        }
                    }
                    // If two digits are entered, switch to seconds
                    if(min_digits == 2) field_choosen = 0;
                }
            }
        }


        clcd_print("TIME:", LINE2(0));
        clcd_putch((min / 10) + '0', LINE2(5));
        clcd_putch((min % 10) + '0', LINE2(6));

        clcd_putch(':', LINE2(7));

        clcd_putch((sec / 10) + '0', LINE2(8));
        clcd_putch((sec % 10) + '0', LINE2(9));


        // Blink the choosen field
        if(field_choosen == 0) clcd_print("  ", LINE2(8));
        else if(field_choosen == 1) clcd_print("  ", LINE2(5));

    }

}


void coocking_screen(void){

    GIE = 1;
    FAN = 1;

    clear_screen();
    unsigned char key;

    clcd_print(" TIME = ", LINE1(0));
    clcd_print("  4.START/RESUME", LINE2(0));
    clcd_print("  5.PAUSE", LINE3(0));
    clcd_print("  6.STOP", LINE4(0));


    while(1){

        key = read_matrix_keypad(STATE);

        if(key == 4){
            if(pause == 1){
                pause = 0;
                FAN = 1;
            }
            else{
                // Add 30 seconds
                if(sec > 30){
                    min += 1;
                    sec = sec - 30;
                }
                else{
                    sec += 30;
                }
            }
        }
        else if(key == 5){
            pause = 1;
            FAN = 0;
        }
        else if(key == 6){
            GIE = 0;
            FAN = 0;
            return;
        }


        clcd_putch((min / 10) + '0', LINE1(9));
        clcd_putch((min % 10) + '0', LINE1(10));

        clcd_putch(':', LINE1(11));

        clcd_putch((sec / 10) + '0', LINE1(12));
        clcd_putch((sec % 10) + '0', LINE1(13));

        if(min == 0 && sec == 0){
            GIE = 0;
            FAN = 0;
            BUZZ = 1;
            __delay_ms(2000);
            BUZZ = 0;
            enjoy_meal_screen();
            return;
        }
    }

}


void set_temp_screen(void){

    clear_screen();
    u_i temp = 0; // MAX: 180
    unsigned char key;
    unsigned char digits = 0;

    clcd_print("SET Temp <*C>", LINE1(0));
    clcd_print("TEMP = ", LINE2(0));
    clcd_print("*:CLEAR  #:ENTER", LINE4(0));

    // Poll key
    while(1){
        key = read_matrix_keypad(STATE);

        if(key != ALL_RELEASED){
            if(digits < 3){
                temp = (temp * 10) + key;
                if(temp > 180) temp = 180;
                digits++;
            }
            
            else if(key == '*'){
                temp = 0;
                digits = 0;
            }
            
            else if(key == '#') return;
        }
        

        clcd_putch((temp / 100) + '0', LINE2(7));
        clcd_putch(((temp / 10) % 10) + '0', LINE2(8));
        clcd_putch((temp % 10) + '0', LINE2(9));
        
    }
}


void pre_heat_screen(void){

    clear_screen();

    clcd_print("   Pre-Heating", LINE1(0));
    clcd_print("Time Left: ", LINE2(0));

    // 13, 14s
    // Pre-heating time
    sec = 60;
    GIE = 1;
    while(sec){
        clcd_putch((sec / 10) + '0', LINE2(11));
        clcd_putch((sec % 10) + '0', LINE2(12));
        clcd_putch('s', LINE2(13));
    }

    GIE = 0;
    return;
}


void enjoy_meal_screen(void){

    clear_screen();
    clcd_print("COOCKED!!!", LINE2(0));
    clcd_print("Enjoy your", LINE3(0));
    clcd_print("Meal...", LINE4(0));

    __delay_ms(2000);
    clear_screen();
}