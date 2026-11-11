/*
 * File:   main.c
 * Project: PIC16F877A - Digital Keypad Based Clock / Event System
 * Description: Main application - Handles Menu Navigation, Key Press
 * (Short Press / Long Press), Time Keeping and Event / Alarm Logic.
 */

#define _XTAL_FREQ 20000000
#include <xc.h>
#include "main.h"
#include "functions.h"
#include "clcd.h"
#include "digital_keypad.h"


#pragma config FOSC = HS      // High Speed Oscillator (20MHz Crystal)
#pragma config WDTE = OFF     // Watchdog Timer Disabled

// Global Variables
Time_t myTime;              // Stores Current Time (Hour, Min, Sec, AM/PM)
Date_t myDate;              // Stores Current Date (Day, Month, Year)
Event_t myEvent[2];         // Array for 2 Events (Event 1 and Event 2)

unsigned char key = ALL_RELEASED;       // Current Key Status from Keypad
unsigned char field = 0;                // Current Editable Field (Hour/Min/Sec)
unsigned char config_select = 0;        // Selection in CONFIG Menu (0 = SET/VIEW, 1 = SET TIME/DATE)
unsigned char set_view_select = 0;      // Selection in SET/VIEW Menu (0 = SET, 1 = VIEW)
unsigned char set_time_date_select = 0; // Selection in TIME/DATE Menu (0 = SET TIME, 1 = SET DATE)
unsigned char event_num = 0;            // Current Event Number (0 = Event 1, 1 = Event 2)
unsigned char blink_flag = 0;           // Flag for Blinking Field in Edit Mode
unsigned char event_display_flag = 0;   // Flag for Event ON Display Blinking
unsigned int idle_count = 0;            // Counter for Auto Return to Default Screen
unsigned int up_press_count = 0;        // Counter for How Long SW1 (Up) is Pressed
unsigned int down_press_count = 0;      // Counter for How Long SW2 (Down) is Pressed


void init_config(void)
{
    ADCON1 = 0x07;  // All PORTA Pins as Digital I/O (ADC Disabled)
    TRISD = 0x00;   // PORTD as Output (For CLCD Data)
    TRISE = 0x00;   // PORTE as Output (For CLCD Control)
    TRISC = 0x00;   // PORTC as Output
    TRISB = 0xFF;   // PORTB as Input (For Digital Keypad)
    PORTC = 0x00;   // Clear PORTC Initially
    
    init_clcd();            // Initialize Character LCD
    init_digital_keypad();  // Initialize Digital Keypad
    
    // Default Time: 01:10:00 AM 
    myTime.hour = 1;
    myTime.min = 10;
    myTime.sec = 0;
    myTime.am_pm = 0; // 1 = AM, 0 = PM

    // Default Date: 27-06-2016
    myDate.day = 27;
    myDate.month = 6;
    myDate.year = 2016;

    // Default Event 1: 01:13:00, Type O, Enabled ON
    myEvent[0].hour = 1;
    myEvent[0].min = 13;
    myEvent[0].sec = 0;
    myEvent[0].duration_hour = 0;
    myEvent[0].duration_min = 30;
    myEvent[0].duration_sec = 0;
    myEvent[0].type = 0; // 0 = O (One-time / ON Type)
    myEvent[0].enabled = 1; // 1 = ON

    // Default Event 2: 02:00:00, Type D, Disabled OFF
    myEvent[1].hour = 2;
    myEvent[1].min = 0;
    myEvent[1].sec = 0;
    myEvent[1].duration_hour = 0;
    myEvent[1].duration_min = 30;
    myEvent[1].duration_sec = 0;
    myEvent[1].type = 1; // 1 = D (Duration Type)
    myEvent[1].enabled = 0; // 0 = OFF
}

void main(void)
{
    unsigned char state = DEFAULT;      // Current Menu State (Starts at DEFAULT Clock Screen)
    unsigned char prev_key = ALL_RELEASED; // Previous Key Pressed

    unsigned int tick_1sec = 0;    // Software Counter for 1 Second Tick
    unsigned int tick_blink = 0;   // Software Counter for Blink Timing
    unsigned int tick_event = 0;   // Software Counter for Event Display Blink
    unsigned char prev_state = 0xFF; // Previous State, Used to Detect State Change
    unsigned char long_done = 0;    // Flag to Prevent Multiple Long Press Triggers
    
    // Call Initialization
    init_config();

    while (1)
    {
        // Read Keypad in LEVEL Mode (Continuous Press Detection)
        key = read_digital_keypad(LEVEL);
        
        // If State Changed, Clear LCD and Reset Counters
        if(state != prev_state)
        {
            prev_state = state;
            key = ALL_RELEASED;
            
            idle_count = 0;
            up_press_count = 0;
            down_press_count = 0;
            clcd_print("                ", LINE1(0)); // Clear Line 1
            clcd_print("                ", LINE2(0)); // Clear Line 2
        }

        // No Key is Pressed
        if (key == ALL_RELEASED)
        {
            idle_count++;
            // Auto Return to DEFAULT Screen After Idle Timeout
            if (idle_count > 5000 && state != DEFAULT)
            {
                state = DEFAULT;
                field = 0;
                idle_count = 0;
            }

            // Handle Short Press (Released Quickly)
            // Short Press = Press Count Between 10 and 80
            if(long_done == 0)
            {
                // SW1 Short Press - Toggle Selection Up
                if (prev_key == SW1 && up_press_count > 10 && up_press_count < 80)
                {
                    if (state == DEFAULT)
                        state = CONFIG; // Go to CONFIG Menu
                    else if (state == CONFIG)
                        config_select = !config_select; // Toggle SET/VIEW <-> SET TIME/DATE
                    else if (state == SET_VIEW_EVENT)
                        set_view_select = !set_view_select; // Toggle SET <-> VIEW
                    else if (state == SET_TIME_DATE)
                        set_time_date_select = !set_time_date_select; // Toggle TIME <-> DATE
                }
                
                // SW2 Short Press - Toggle Selection Down (Same as SW1)
                if (prev_key == SW2 && down_press_count > 10 && down_press_count < 80)
                {
                    if (state == DEFAULT)
                        state = CONFIG;
                    else if (state == CONFIG)
                        config_select = !config_select;
                    else if (state == SET_VIEW_EVENT)
                        set_view_select = !set_view_select;
                    else if (state == SET_TIME_DATE)
                        set_time_date_select = !set_time_date_select;
                }
            }

            // Reset Press Counters After Release
            up_press_count = 0;
            down_press_count = 0;
            prev_key = ALL_RELEASED;
            long_done = 0;
        }
        else
           
        {
            idle_count = 0; // Reset Idle Timer When Key is Pressed
            
            // SW1 (UP / ENTER Key) is Pressed
            if (key == SW1)
            {
                up_press_count++;
                prev_key = SW1;
                // Long Press Detected (Count >= 80)
                if (up_press_count >= 80 && long_done == 0) 
                {
                    // Mark Long Press as Done
                    long_done = 1; 
                  
                    // ENTER Logic Based on Current State
                    if (state == CONFIG)
                    {
                        if (config_select == 0) 
                            state = SET_VIEW_EVENT; // Go to SET/VIEW EVENT Menu
                        else 
                            state = SET_TIME_DATE; // Go to SET TIME/DATE Menu
                        field = 0;
                    }
                    else if (state == SET_VIEW_EVENT)
                    {
                        if (set_view_select == 0) 
                            state = SET_EVENT; // Go to SET EVENT Screen
                        else 
                            state = VIEW_EVENT; // Go to VIEW EVENT Screen
                        field = 0;
                    }
                    else if (state == SET_TIME_DATE)
                    {
                        if (set_time_date_select == 0) 
                            state = SET_TIME; // Go to SET TIME Screen
                        else 
                            state = SET_DATE; // Go to SET DATE Screen
                        field = 0;
                    }
                    else if (state == SET_EVENT || state == SET_TIME || state == SET_DATE)
                    {
                        state = CONFIG; // Save and Go Back to CONFIG
                    }
                    up_press_count = 81; // Lock Counter to Avoid Re-trigger
                }
            }
            // SW2 (DOWN / BACK Key) is Pressed
            else if (key == SW2) 
            {
                down_press_count++;
                prev_key = SW2;
                // Long Press Detected (Count >= 80)
                if (down_press_count >= 80 && long_done == 0)
                {
                    long_done = 1;
                    
                    // BACK Logic Based on Current State
                    if (state == CONFIG) 
                        state = DEFAULT; // Back to Clock Screen
                    else if (state == SET_VIEW_EVENT) 
                        state = CONFIG; // Back to CONFIG
                    else if (state == SET_TIME_DATE) 
                        state = CONFIG; // Back to CONFIG
                    else if (state == SET_EVENT || state == VIEW_EVENT) 
                        state = SET_VIEW_EVENT; // Back to SET/VIEW Menu
                    else if (state == SET_TIME || state == SET_DATE) 
                        state = SET_TIME_DATE; // Back to TIME/DATE Menu
                    
                    field = 0;
                    down_press_count = 81; // Lock Counter
                }
            }
        }

        // Timer for 1 Second - Used for Clock Increment
        tick_1sec++;
        if (tick_1sec > 20000)
            {
                tick_1sec = 0;
                Increment_Time(); // Increment Current Time by 1 Sec
          
                // Event Display Blink Logic (Blink for 2 Sec in 7 Sec Cycle)
                tick_event++;
                if (tick_event < 5) 
                    event_display_flag = 0;
                else if (tick_event < 7) 
                    event_display_flag = 1;
                else 
                    tick_event = 0;
            }

        // Display / Function Call Based on Current State
        if (state == DEFAULT)
        {
            show_Default(); // Show Clock and Date
        }
        else if (state == CONFIG)
        {
            show_config(); // Show CONFIG Menu
        }
        else if (state == SET_VIEW_EVENT)
        {
            show_set_view_event(); // Show SET / VIEW Menu
        }
        else if (state == SET_EVENT)
        {
            // Blink Logic for Edit Mode (Blink Every ~0.5 Sec)
            tick_blink++;
            if (tick_blink > 10000)
            {
                tick_blink = 0;
                blink_flag = !blink_flag; // Toggle Blink
            }
            set_event(); // Edit Event Details
        }
        else if (state == VIEW_EVENT)
        {
            view_event(); // Display Event Details (Read Only)
        }
        else if (state == SET_TIME_DATE)
        {
            show_set_time_date(); // Show SET TIME / SET DATE Menu
        }
        else if (state == SET_TIME)
        {
            // Blink Logic for SET TIME
            tick_blink++;
            if (tick_blink > 10000)
            {
                tick_blink = 0;
                blink_flag = !blink_flag;
            }
            set_time(); // Edit Time
        }
        else if (state == SET_DATE)
        {
            // Blink Logic for SET DATE
            tick_blink++;
            if (tick_blink > 10000)
            {
                tick_blink = 0;
                blink_flag = !blink_flag;
            }
            stop_date(); // Edit Date
        }

        Check_Alarm(); // Check if Any Event Time Matches Current Time
        stop_alarm();  // Check if Alarm Should be Stopped
        
    }
}