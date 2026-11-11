#define _XTAL_FREQ 20000000
#include <xc.h>
#include "clcd.h"
#include "timers.h"
#include "matrix_keypad.h"

#pragma config WDTE = OFF

#define HEATER PORTCbits.RC5                   // Heater connected to RC5
#define COOLER PORTCbits.RC2                   // cooler connected to RC2
#define BUZZER PORTCbits.RC0                   // buzzer connected to RC0

volatile unsigned char sec_tick = 0;            // 1 second flag set by timer interrupt
volatile unsigned char preheat = 60;            // 60 sec preheat time for convection mode
volatile unsigned int power_on_count = 0;       // Timer count for debouncing and delays
volatile unsigned char cooking_done_wait = 0;   // Wait time after cooking done
volatile int remaining_sec = 0;                 // Remaining cooking time in seconds
volatile unsigned char state = 1;               // Main state machine variable
volatile unsigned int buzzer_timer = 0;         // Buzzer on duration control


unsigned char mode = 0;         // 1-MICRO, 2-GRILL, 3-CONVECTION
int total_sec = 0;              // Total cooking time entered by user
unsigned char mm = 0, ss = 0;   // Minutes and Seconds entered

static void init_config(void) 
{
    
    TRISCbits.TRISC0 = 0;  // Set RC0 as output (Buzzer)
    TRISCbits.TRISC2 = 0;  // Set RC2 as output (cooler)
    TRISCbits.TRISC5 = 0;  // Set RC5 as output (heater)
    
    HEATER = 0;       //Initially heater off
    COOLER = 0;       //Initially cooler off
    BUZZER = 0;       //Initially buzzer off
    
    init_clcd(); 
    init_timer0();
    
    GIE = 1;           // Enable Global Interrupt
    PEIE = 1;          // Enable Peripheral Interrupt
    TMR0IF = 0;        // Clear Timer0 flag
    TMR0IE = 1;        // Enable Timer0 Interrupt
}

// Function to start buzzer for given milliseconds
void buzzer_start(unsigned int ms)
{
        buzzer_timer = ms;
}
void main(void) 
{
    unsigned char key;
    unsigned char last_state = 99;      // For checking state change to avoid LCD flicker
    unsigned int last_key_time = 0;     // For key debouncing
    
    init_config();                      //init call
    
     // Power on display message
    clcd_print("Powering ON     ", LINE1(0));
    clcd_print("Microwave Oven  ", LINE2(0));
    unsigned int start = power_on_count;
    while(power_on_count - start < 1250);    // 1.25 sec delay for power on screen
    
    while (1) 
    {
        key = read_matrix_keypad(STATE);      // Read keypad
        
         // Buzzer handling logic
        if(buzzer_timer > 0)
        {
            BUZZER = 1;
            buzzer_timer--;
        }
        else
        {
            if(state != 5)               // In cooking done state buzzer toggles
                BUZZER = 0;
        }
        
        // STATE 1: MENU DISPLAY
        if(state == 1) 
        {
            // Update LCD only when state changes
            if(last_state != state)
            {
                last_state = state;
                clcd_print("1.MICRO 2.GRILL   ", LINE1(0));
                clcd_print("3.CONV  4.START   ", LINE2(0));
            }
            HEATER = 0; 
            COOLER = 0;
            
            // If mode key pressed with debounce
            if((key == 1 || key == 2 || key == 3) && (power_on_count - last_key_time > 200))
            {
                last_key_time = power_on_count;
                mode = key; 
                state = 2;                            // Go to Time Set state
                mm = 0; 
                ss = 0; 
                total_sec = 0; 
                last_state = 99;
                
                while(read_matrix_keypad(STATE) != ALL_RELEASED);     // Wait for key release
            }
        }
        
         // STATE 2: SET COOKING TIME
        else if(state == 2)               
        {
            if(last_state != state)
            {
                clcd_print("SET TIME MM:SS  ", LINE1(0));
                clcd_print("TM-00:00 *C #E  ", LINE2(0));      // * = Clear, # = Enter
                last_state = state;
            }
            if((key != ALL_RELEASED)&& (power_on_count - last_key_time > 150))
            {
                last_key_time = power_on_count;
                if(key <= 9)
                {
                     // Logic to shift digits like MMSS entry
                    unsigned int total_input = mm*100 + ss;
                    
                    total_input = total_input*10 + key;
                    
                    if(total_input > 9959) 
                    {
                        total_input = total_input % 10000;
                        mm = total_input / 100; 
                        ss = total_input % 100;
                    }
                    if(ss > 59)                // Seconds should not exceed 59
                        ss = 59;
                }
                else if(key == 10)              // * key for clear
                {
                    mm = 0; 
                    ss = 0;
                }
                else                            //# key for enter
                {
                    total_sec = mm*60 + ss;
                    if(total_sec > 0)           //valid time for enter
                    { 
                        remaining_sec = total_sec;
                        buzzer_start(100);      // Beep
                        if(mode == 3)          // Convection needs preheat
                        { 
                            state = 3; 
                            preheat = 60; 
                        }
                        else                   // Micro and Grill direct cooking
                        { 
                            state = 4; 
                        }
                        last_state = 99;
                        
                        while(read_matrix_keypad(STATE) != ALL_RELEASED);
                    }
                }
            }
            // Display entered time continuously
            clcd_putch((mm/10)+'0', LINE2(3));
            clcd_putch((mm%10)+'0', LINE2(4));
            clcd_putch((ss/10)+'0', LINE2(6));
            clcd_putch((ss%10)+'0', LINE2(7));
        }
        
        // STATE 3: PRE-HEATING (Only for CONV)
        else if(state == 3) 
        {
            if(last_state != state)
            { 
                last_state = state; 
                clcd_print("PRE-HEATING     ", LINE1(0));
                clcd_print("TIME REM: 00 sec", LINE2(0));
            }
            HEATER = 1;                        //Heater on during preheat
            COOLER = 1;
            if(sec_tick)                       // Every 1 second 
            { 
                sec_tick = 0; 
                if(preheat > 0)
                {
                    preheat--; 
                    clcd_putch((preheat/10)+'0', LINE2(10));
                    clcd_putch((preheat%10)+'0', LINE2(11));
                }
                if(preheat == 0)                // Preheat done
                {
                    HEATER = 0; 
                    COOLER = 0; 
                    state = 4;
                    remaining_sec = total_sec; 
                    last_state = 99;
                    buzzer_start(200);         // Beep after preheat
                }
            }
        }
        
         // STATE 4: COOKING
        else if(state == 4) 
        {
            if(last_state != state)
            { 
                last_state = state; 
                
                clcd_print("TIME = 00:00    ", LINE1(0));
                clcd_print("4.ST 5.PA 6.STOP", LINE2(0));
                
                 // Display initial remaining time
                int rm_mm = remaining_sec / 60;
                int rm_ss = remaining_sec % 60;
                clcd_putch((rm_mm/10)+'0', LINE1(7));
                clcd_putch((rm_mm%10)+'0', LINE1(8));
                clcd_putch((rm_ss/10)+'0', LINE1(10));
                clcd_putch((rm_ss%10)+'0', LINE1(11));
            }
            HEATER = 1;               // Both on during cooking
            COOLER = 1;
            if(sec_tick)
            { 
                sec_tick = 0; 
                if(remaining_sec > 0)
                {
                    remaining_sec--; 
                    
                     // Update time countdown on LCD
                    int rm_mm = remaining_sec / 60;
                    int rm_ss = remaining_sec % 60;
                    clcd_putch((rm_mm/10)+'0', LINE1(7));
                    clcd_putch((rm_mm%10)+'0', LINE1(8));
                    clcd_putch((rm_ss/10)+'0', LINE1(10));
                    clcd_putch((rm_ss%10)+'0', LINE1(11));
                    
                    if(remaining_sec == 0)   // Cooking finished
                    { 
                        state = 5;           // Go to done state
                        HEATER = 0; 
                        COOLER = 0; 
                        cooking_done_wait = 0; 
                        last_state = 99;
                        buzzer_start(200);
                    }
                }
            }
            
            // Pause key
            if(key == 5)
            { 
                HEATER = 0; 
                COOLER = 0; 
                clcd_print("PAUSED          ", LINE1(0)); 
                clcd_print("Press 4 to RES  ", LINE2(0)); 
                while(read_matrix_keypad(STATE) != 4);       // Wait till resume key
                last_state = 99;
            }
            
            // Start/Stop to go menu
            if((key == 4 || key == 6) && (power_on_count - last_key_time > 200))
            { 
                last_key_time = power_on_count;
                state = 1; 
                HEATER = 0; 
                COOLER = 0; 
                remaining_sec = 0; 
                total_sec = 0; 
                mm=0; 
                ss=0; 
                last_state = 99; 
                buzzer_start(200);          //buzzer_start(200); 
            }  
        }
        
        // STATE 5: COOKING DONE
        else if(state == 5)
        {
             if(last_state != state)
            { 
                last_state = state; 
                HEATER = 0; 
                COOLER = 0;
                clcd_print("Cooking Done!   ",LINE1(0));
                clcd_print("Enjoy ur meal!  ", LINE2(0));
             }
            if(sec_tick)
            { 
                sec_tick = 0; 
                cooking_done_wait++; 
                BUZZER = !BUZZER;             // Buzzer beeping for 6 sec
                if(cooking_done_wait >= 6)
                { 
                    BUZZER = 0; 
                    state = 1; 
                    mm=0; 
                    ss=0; 
                    total_sec=0; 
                    cooking_done_wait=0; 
                    last_state = 99;           // Return to menu
                }
            }
        }
    }
}