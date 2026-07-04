/*
 * File:   auth.c
 * Author: Santhosh T K
 *
 * Created on June 30, 2026, 12:38 AM
 */


// Authenticate user with a password
// and give access to logs
#include "clcd.h"
#include "digital_keypad.h"
#include "auth.h"
#include "timers.h"
#include <pic.h>
#include <xc.h>


static unsigned char original_password[5] = {'0', '1', '0', '1', '\0'};
static unsigned char user_entered_password[5];
unsigned char access = 0;

unsigned int index = 0; // Global and Lifetime[Throughout the program]
unsigned int attempt_no = 0;
unsigned int delay = DELAY;


void auth_user(void){

    unsigned char key;
    index = 0;
    
    while(1){

        clcd_print(" ENTER PASSWORD ", LINE1(0));
        if(index == 0) clcd_print("                ", LINE2(0));

    
        key = read_digital_keypad(STATE);
        __delay_ms(100);
    
        if(key == SW1){
            user_entered_password[index++] = '0';
            clcd_putch('*', LINE2(index));
        }

        else if(key == SW2){
            user_entered_password[index++] = '1';
            clcd_putch('*', LINE2(index));
        }

        if(index > 3){
            user_entered_password[index] = '\0';
            __delay_ms(1000);
        }
    

        // Check if 4 digits entered
        if(index == 4){

            unsigned int i = 0;
            for( ;i < 4; i++){
                if(user_entered_password[i] != original_password[i]){
                    break;
                }
            }


            if(i == 4){
                access = GRANTED;
                clcd_print("Access Granted.", LINE2(0));
                __delay_ms(500);
                return; // returning to main menu
            }
            else{
                access = DENIED;
                clcd_print("Access Denied.",LINE2(0));
                __delay_ms(1500);

                attempt_no++;
                index = 0;

                clcd_print("Attempt Left:", LINE2(0));
                clcd_putch((3 - attempt_no) + '0', LINE2(13));
                __delay_ms(1500);

                if(attempt_no == 3){
                    run_delay();
                    attempt_no = 0;
                }
            }
        }
    }
}


// Run delay for 30 Seconds with TMR0
void run_delay(void){

    init_timer0();

    while(delay){
        clcd_print(" ENTERING WAIT  ", LINE1(0));
        clcd_print("Seconds left: ", LINE2(0));

        // Display delay
        clcd_putch((delay / 10) + '0', LINE2(14));
        clcd_putch((delay % 10) + '0', LINE2(15));
    }

    delay = DELAY;

    return;
}


void change_original_password(unsigned char new_password[5]){

    for(unsigned int i = 0; i < 5; i++){
        original_password[i] = new_password[i];
    }

}