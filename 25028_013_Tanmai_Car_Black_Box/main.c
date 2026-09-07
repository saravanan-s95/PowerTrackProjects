#include "main.h"
#pragma config WDTE =  OFF  // turn off WDT
unsigned char time[7];
void init_config() 
{
    init_clcd();
    init_i2c(100000);  
    init_ds1307();
    init_digital_keypad();
    init_adc();
    init_timer2();
    init_uart(9600);
}

void main(void) 
{
    init_config();
    GIE = 1;
    PEIE = 1;
    unsigned char current_mode = OPERATION_MODE, reset_flag = RESET_NOTHING ;
    eeprom_write_str(0x00, "1010");
    char *gear[] ={"GN", "GR", "G1", "G2", "G3", "G4"};
    unsigned char gear_count=0;
    unsigned char event[3] = "ON";
    unsigned char speed = 0;
    unsigned char key, menu;
    unsigned char press_type;
    //log_event(event, speed);
    
    while(1)
    {
        key = read_digital_keypad(STATE);
        speed = read_adc() / 10;
        if (speed > 100)
        {
            speed = 99;
        }
        
        for(int delay=200; delay--; );  // bouncing effect
        
        press_type = NO_PRESS;
        
        if((key == SW4 || key == SW5))
        {
            press_type = detect_press(key);
        }
        if ( key == SW1)
        {
            strcpy(event, "C ");
            log_event(event, speed);
        }        
        else if (key == SW2 && gear_count < 6)
        {
            strcpy(event, gear[gear_count]);
            log_event(event, speed);
            gear_count++;
        }
        else if (key == SW3 && gear_count > 0)
        {
            gear_count--;
            strcpy(event, gear[gear_count]);
            log_event(event, speed);
        }
        if(current_mode == SET_TIME_MODE && press_type == LONG_PRESS && key == SW4)
        {
            save_time_ds1307(time);
            current_mode = MENU_MODE;
            reset_flag = RESET_MENU_SCREEN;
            clear_screen();
        }
        else if ((current_mode == OPERATION_MODE) && (key == SW4 || key == SW5))
        {
            current_mode = LOGIN_MODE;
            reset_flag = RESET_LOGIN_SCREEN;
            clear_screen();
            clcd_print("ENTER PASSWORD",LINE1(0));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON_BLINK, INST_MODE);
            TIMER2_ENABLE = 1;
        } 
        if ( press_type == LONG_PRESS)
        {
            /* when inside MENU */
            if(current_mode == MENU_MODE)
            {
                if(key == SW4)     // enter selected menu
                {
                    switch(menu)
                    {
                        case VIEW_LOG:
                            current_mode = VIEW_LOG_MODE;
                            reset_flag = RESET_VIEW_LOG;
                            break;

                        case CLEAR_LOG:
                            current_mode = CLEAR_LOG_MODE;
                            break;

                        case DOWNLOAD_LOG:
                            current_mode = DOWNLOAD_LOG_MODE;
                            break;

                        case SET_TIME:
                            current_mode = SET_TIME_MODE;
                            reset_flag = RESET_TIME;
                            break;

                        case CHANGE_PWD:
                            current_mode = CHANGE_PWD_MODE;
                            reset_flag = RESET_CHANGE_PWD;
                            break;
                    }
                    clear_screen();
                }
                else if(key == SW5)   // logout
                {
                    current_mode = OPERATION_MODE;
                    clear_screen();
                }
            }
                /* when inside sub-menu */
            else if(current_mode >= VIEW_LOG_MODE &&
                    current_mode <= CHANGE_PWD_MODE)
            {
                if(key == SW4)     // back to menu
                {
                    current_mode = MENU_MODE;
                    reset_flag = RESET_MENU_SCREEN;
                    clear_screen();
                }

                else if(key == SW5)  // logout
                {
                    current_mode = OPERATION_MODE;
                    clear_screen();
                }
            }
        }
        switch ( current_mode )
        {
            case OPERATION_MODE:
                display_dashboard(event, speed);
                break;
                
            case LOGIN_MODE:
                switch (log_in_password(key,reset_flag))
                {
                    case RETURN_BACK:
                        clear_screen();
                        current_mode = OPERATION_MODE;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        TIMER2_ENABLE = 0;
                        break;
                    case LOGIN_SUCCESS:
                        clear_screen();
                        current_mode = MENU_MODE;
                        reset_flag = RESET_MENU_SCREEN;
                        continue;
                }
                break;
                
            case MENU_MODE:
                TIMER2_ENABLE = 1;
                menu = menu_screen(key, reset_flag);
                if(menu == RETURN_BACK)
                {
                        clear_screen();
                        current_mode = OPERATION_MODE;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        TIMER2_ENABLE = 0;
                }
                break;
            case VIEW_LOG_MODE:
                event_scroll(key, reset_flag);
                break;
            case CLEAR_LOG_MODE:
                clear_logs();
                clcd_print("LOGS CLEARED",LINE1(0));
                __delay_ms(1000);
                current_mode = MENU_MODE;
                reset_flag = RESET_MENU_SCREEN;
                continue; 
            case DOWNLOAD_LOG_MODE:
                clear_screen();
                clcd_print("DOWNLOADING...", LINE1(0));
                download_logs();
                __delay_ms(1000);
                current_mode = MENU_MODE;
                reset_flag = RESET_MENU_SCREEN;
                continue;
            case CHANGE_PWD_MODE:
                switch(change_password(key, reset_flag))
                {
                    case PASSWORD_CHANGED:
                    case PASSWORD_FAILED:
                        current_mode = MENU_MODE;
                        reset_flag = RESET_MENU_SCREEN;
                        clear_screen();
                    break;
                }
                break;
            case SET_TIME_MODE:
                set_time(key, reset_flag);
                break;
        }    
        reset_flag = RESET_NOTHING;
    }
}

 