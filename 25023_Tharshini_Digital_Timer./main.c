/*
  Name        : THARSHINI S
  File        : main.c
  Project     : Digital Alarm Clock
  Description : Main program controlling the Digital Alarm Clock.
                Initializes all peripherals, handles screen navigation,
                processes keypad input, manages alarm operations,
                and controls the overall system workflow.
*/

#include "main.h"

//Disable Watchdog Timer
#pragma config WDTE = OFF
extern unsigned int alarm_remaining_secs;

static void init_config(void)
{
    init_i2c(100000); //initialize i2c with 100 Khz
    init_ds1307(); //initialize rtc
    init_clcd(); //initialize clcd
    init_digital_keypad(); //initialize digital keypad
    init_timer2(); //initialize timer2
    TMR2ON=1;
    init_alarms(); //buzzer pin never set up without this
    
    //enable global and peripheral interrupts
    PEIE = 1;
    GIE = 1;

}

void main(void)
{
    unsigned char screen_flag = DEFAULT_SCREEN; //Stores the current screen
    unsigned char reset_flag = RESET_NOTHING; //to avoid initialization again and again 
    unsigned char key;
    unsigned char menu_position;
    unsigned char alarm_position;
    
    init_config();
    
    //Display welcome screen
    clear_screen();
    clcd_print(" WELCOME TO ",LINE1(2));
    clcd_print("SMART CLOCK..",LINE2(2));
    __delay_ms(2000);
    clear_screen();
    
    while(1)
    {
         key=read_digital_keypad(STATE); //Read keypad input
         
         //Stop the alarm when any key is pressed
         if(BUZZER==1 && key!=ALL_RELEASED)
         {
             BUZZER=0;
             alarm_remaining_secs=0;
             clear_screen();
             continue;
         }
         
         trigger_alarms(); //Check whether any alarm should be triggered
         
         //to config screen up or down
         if((key==SW_UP||key==SW_DOWN) && screen_flag==DEFAULT_SCREEN)
         {
             screen_flag=CONFIG_SCREEN;
             clear_screen();
             reset_flag=RESET_CONFIG;
             TMR2ON=1;
             continue;
         }
         
        //long press UP on Config Screen -> enter selected option
        else if(screen_flag==CONFIG_SCREEN && key==SW_UP_L)
        {
            if(menu_position==0)
            {
                screen_flag=EVENT_SUBMENU;
                reset_flag=RESET_EVENT_SUBMENU;
            }
            else
            {
                screen_flag=TIME_DATE_SUBMENU;
                reset_flag=RESET_TIME_DATE_SUBMENU;
            }
            continue;
        }
         
        //long press UP inside Event Submenu -> Set Event or View Event
        else if(screen_flag==EVENT_SUBMENU&&key==SW_UP_L)
        {
           if(menu_position==0)
           {
               screen_flag=EDIT_ALARM;//Set Event
               reset_flag=RESET_EDIT_ALARM;
           }
           else
           {
               screen_flag=SET_VIEW_ALARMS;//View Event
               reset_flag=RESET_SET_VIEW_ALARMS;
           }
           continue;
        }
        //long press down->come back to config screen
        else if(screen_flag==EVENT_SUBMENU&&key==SW_DOWN_L)
        {
            screen_flag=CONFIG_SCREEN;
            reset_flag=RESET_CONFIG;
            continue;
        }
        //long press UP inside time date Submenu -> Set Time or Set Date
        else if(screen_flag==TIME_DATE_SUBMENU && key==SW_UP_L)
        {
            if(menu_position==0)
            {
                screen_flag=SET_TIME_SCREEN;
                reset_flag=RESET_SET_TIME_SCREEN;
            }
            else
            {
                screen_flag=SET_DATE_SCREEN;
                reset_flag=RESET_SET_DATE_SCREEN;
            }
            continue;
        }
        //long press DOWN on Config Screen -> logout to Default screen
        else if(screen_flag==CONFIG_SCREEN && key==SW_DOWN_L)
        {
            screen_flag=DEFAULT_SCREEN;
            clear_screen();
            //TMR2ON=0;
            reset_flag=RESET_NOTHING;
            continue;
        }
         
        //Execute the current screen 
        switch(screen_flag)
        {
            case DEFAULT_SCREEN: 
                display_default_screen();
                break;
            case CONFIG_SCREEN:
                //Display Configuration menu
                menu_position=config_screen(reset_flag,key);
                if(menu_position==RETURN_BACK)
                {
                    screen_flag=DEFAULT_SCREEN;
                    clear_screen();
                    //TMR2ON=0;
                    reset_flag=RESET_NOTHING;
                    continue;
                }
                reset_flag=RESET_NOTHING;
                break;
            case EVENT_SUBMENU:
                //Display Event submenu
                menu_position=event_submenu_screen(reset_flag,key);
                if(menu_position==RETURN_BACK)
                {
                    screen_flag=CONFIG_SCREEN;
                    reset_flag=RESET_CONFIG;
                    continue;
                }
                reset_flag=RESET_NOTHING;
                break;
                
            case SET_VIEW_ALARMS:
               //Display saved alarms
               alarm_position=set_view_alarms(key,reset_flag);
                if(alarm_position==RETURN_BACK)
                {
                    screen_flag=EVENT_SUBMENU;
                    reset_flag=RESET_EVENT_SUBMENU;
                    continue;
                }
                reset_flag=RESET_NOTHING;
                break; 
                
            case EDIT_ALARM:
                //Create a new alarm
                if(edit_alarm(key,reset_flag)==RETURN_BACK)
                {
                    screen_flag=EVENT_SUBMENU;
                    reset_flag=RESET_EVENT_SUBMENU;
                    continue;                    
                }
                reset_flag=RESET_NOTHING;
                break;
                
            case TIME_DATE_SUBMENU:
                //Display Time/Date submenu
                menu_position=time_date_submenu_screen(reset_flag,key);
                if(menu_position==RETURN_BACK)
                {
                   screen_flag=CONFIG_SCREEN;
                    reset_flag=RESET_CONFIG;
                    continue; 
                }
                reset_flag=RESET_NOTHING;
                break;

            case SET_TIME_SCREEN:
                //Set current time
                if(set_time_screen(key,reset_flag)==RETURN_BACK)
                {
                    screen_flag=TIME_DATE_SUBMENU;
                    reset_flag=RESET_TIME_DATE_SUBMENU;
                    continue;
                }
                reset_flag=RESET_NOTHING;
                break;
            
            case SET_DATE_SCREEN:
                //Set current date
                if(set_date_screen(key,reset_flag)==RETURN_BACK)
                {
                    screen_flag=TIME_DATE_SUBMENU;
                    reset_flag=RESET_TIME_DATE_SUBMENU;
                    continue;
                }
                reset_flag=RESET_NOTHING;
                break;              
            }        

    }
}
