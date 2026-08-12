/*
 * File:   main.c
 * Author: Swaraj
 *
 * Created on 23 June, 2026, 3:56 PM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "timer.h"
#include "matrix_keypad.h"

volatile unsigned char one_sec_flag = 0;   // 1 second flag
unsigned char state = HOME_SCREEN;     //Initial state 

unsigned char minutes = 0;
unsigned char seconds = 0;

unsigned char set_field = 0;      // 0=MINUTES, 1=SECONDS
unsigned char cooking_flag = 0;      //  to fetch the cooking condition

unsigned char temp = 0;    // Default temperature
unsigned char preheat = 60;   //preheating time 

void init_config(void)
{
    init_clcd();
    init_matrix_keypad() ;
    init_timer2 ();
    
    FAN_DDR = 0;           
    BUZZER_DDR = 0;

    FAN = 0;                //initially fan is off
    BUZZER = 0;             //initially buzzer is off
}

void splash_screen(void)
{
    unsigned char i;

    clcd_print("Powering on", LINE2(2));     // splashing a message initially 
    clcd_print("Microwave oven", LINE3(1));

    for(i = 0; i < 16; i++)
    {
        clcd_putch(0xFF, LINE1(i));   //spaces effecton line 1 
        __delay_ms(80);
    }  
    
    for(i = 0; i < 16; i++)
    {
        clcd_putch(0xFF, LINE4(i));   //spaces effecton line 4 
        __delay_ms(80);
    }

    __delay_ms(1000);
}

//HOME SCREEN (showing the options)
void display_home(void)
{
    clcd_clear();    

    clcd_print("1.MICRO", LINE1(0));
    clcd_print("2.GRILL", LINE2(0));
    clcd_print("3.CONVECTION", LINE3(0));
    clcd_print("4.START", LINE4(0));
}

void main(void)
{
    unsigned char key;

    init_config();

    splash_screen();
    display_home();

    while (1)
    {
        key = read_matrix_keypad(STATE);  //reading the key

        switch (state)
        {
            case HOME_SCREEN:   //for home screen

                if (key == 1) //key 1 for MICRO MODE
                {
                    state = MICRO_MODE;
                    clcd_clear();
                }
                else if (key == 2)  //key 2 for GRILL MODE
                {
                    state = GRILL_MODE;
                    clcd_clear();
                }
                else if (key == 3)   //key 3 for CONVECTION MODE
                {
                    state = CONV_MODE;
                    clcd_clear();
                }
                else if (key == 4)   //key 4 for START MODE
                {
                    state = QUICK_START;
                    clcd_clear();
                }
                break;

            case MICRO_MODE:  //MICRO MODE Case

                clcd_print("MICRO MODE", LINE1(2));
                __delay_ms(1000);

                clcd_clear();

                clcd_print("POWER : 900W", LINE2(1));
                __delay_ms(1000);

                clcd_clear();

                state = SET_TIME;   //for setting time 
                break;

            case SET_TIME:   //set time

                if (set_field == 0)   
                {
                    clcd_print("SET MINUTES", LINE1(2));
                    clcd_print("MIN :", LINE2(2));

                    clcd_putch((minutes / 10) + '0', LINE2(8));   
                    clcd_putch((minutes % 10) + '0', LINE2(9));

                    clcd_print("4:+ 7:-", LINE3(0));   // 4 for increment and 7 for decrement
                    clcd_print("5:NEXT", LINE4(0));    // 5 for next

                    if (key == 4)
                    {
                        if (minutes < 60)
                            minutes++;
                    }
                    else if (key == 7)
                    {
                        if (minutes > 0)
                            minutes--;
                    }
                    else if (key == 5)
                    {
                        set_field = 1;
                        clcd_clear();
                    }
                }
                else
                {
                    clcd_print("SET SECONDS", LINE1(2));
                    clcd_print("SEC :", LINE2(2));

                    clcd_putch((seconds / 10) + '0', LINE2(8));
                    clcd_putch((seconds % 10) + '0', LINE2(9));

                    clcd_print("4:+ 7:-", LINE3(0));     // 4 for increment and 7 for decrement
                    clcd_print("#=ENTER", LINE4(0));     // # for ENTER

                    if (key == 4)
                    {
                        if (seconds < 59)
                            seconds++;
                    }
                    else if (key == 7)
                    {
                        if (seconds > 0)
                            seconds--;
                    }
                    else if (key == '#')
                    {
                        state = READY;
                        clcd_clear();
                    }
                }
                break;

            case READY:  // If time set then ready

                clcd_print("READY", LINE1(5));
                clcd_print("TIME :", LINE2(0));

                clcd_putch((minutes / 10) + '0', LINE2(7));
                clcd_putch((minutes % 10) + '0', LINE2(8));
                clcd_putch(':', LINE2(9));
                clcd_putch((seconds / 10) + '0', LINE2(10));
                clcd_putch((seconds % 10) + '0', LINE2(11));

                clcd_print("4.START", LINE3(0));   //to start the time
                clcd_print("5.PAUSE 6.STOP", LINE4(0));    // to pause and stop the time]

                if (key == 4)
                {
                    cooking_flag = 1;     //when cooking status 1 then fan on means fan start
                    FAN = 1;    // to indicate microoven starts
                }
                else if (key == 5)  //for pause
                {
                    cooking_flag = 0;
                    FAN = 0;
                }
                else if (key == 6)  // for stop
                {
                    cooking_flag = 0;
                    FAN = 0;

                    minutes = 0;   // all stops
                    seconds = 0;
                    set_field = 0;

                    state = HOME_SCREEN;   //state chnge to home screen 

                    clcd_clear();
                    display_home();   // and back to home screen
                }

                //countdown after setting time
                if (one_sec_flag && cooking_flag)
                {
                    one_sec_flag = 0;

                    if (seconds > 0)
                    {
                        seconds--;
                    }
                    else
                    {
                        if (minutes > 0)
                        {
                            minutes--;
                            seconds = 59;
                        }
                        else
                        {
                            FAN = 0;
                            BUZZER = 0;

                            cooking_flag = 0;
                            state = DONE;   // after time end then state change to DONE

                            clcd_clear();
                        }
                    }
                }
                break;

            case DONE:  //after time up then done

                FAN = 0;   //fan stops
                BUZZER = 1;   //buzzer starts to indicate work done

                clcd_print("DONE", LINE1(5));
                clcd_print("ENJOY MEAL", LINE2(2));

                __delay_ms(3000);

                BUZZER = 0;  // after 3 sec buzzer off

                minutes = 0;
                seconds = 0;
                set_field = 0;
                cooking_flag = 0;

                state = HOME_SCREEN;   // state change

                clcd_clear();
                display_home();   // back to home screen

                break;

            case GRILL_MODE:   //GRILL mode

                clcd_print("GRILL MODE", LINE2(2));

                __delay_ms(1000);

                clcd_clear();

                minutes = 0;
                seconds = 0;
                set_field = 0;

                state = SET_TIME;   //same like micro mode set time

                break;

            case SET_TEMP:  // for setting temperature

                clcd_print("SET TEMP", LINE1(3));

                clcd_putch((temp / 100) + '0', LINE2(6));
                clcd_putch(((temp % 100) / 10) + '0', LINE2(7));
                clcd_putch((temp % 10) + '0', LINE2(8));
                clcd_putch('C', LINE2(9));

                clcd_print("4:+ 7:-", LINE3(0));   // 4 for increment and 7 for decrement
                clcd_print("#:NEXT", LINE4(0));    //# for NEXT

                if (key == 4)
                {
                    if (temp < 180)   //setting limit of temperature upto 180 degree
                        temp++;
                }
                else if (key == 7)
                {
                    if (temp > 0)
                        temp--;
                }
                else if (key == '#')   //after seeting temp then preheating starts
                {   
                    state = PREHEAT;   // state change to PREHEAT
                    clcd_clear();
                }

                break;

            case PREHEAT:  //PREHEATING case

                clcd_print("PREHEATING", LINE1(2));

                clcd_putch((preheat / 10) + '0', LINE2(5));
                clcd_putch((preheat % 10) + '0', LINE2(6));
                clcd_print(" SEC", LINE2(7));

                
                //countdown for preheating from 60 sec -> 0 sec
                if (one_sec_flag)
                {
                    one_sec_flag = 0;

                    if (preheat > 0)
                    {
                        preheat--;    //preheat value already set at globally 60sec
                    }
                    else
                    {
                        minutes = 0;
                        seconds = 0;
                        set_field = 0;

                        state = SET_TIME;
                        clcd_clear();
                    }
                }

                break;

            case CONV_MODE:   //CONVECTION MODE

                temp = 0;
                clcd_clear();
                state = SET_TEMP;   //Same like SET time 

                break;
                
            case QUICK_START:  //START MODE

                //only start the microwave we set default time of 30 sec to heat
                minutes = 0;
                seconds = 30;   

                cooking_flag = 1;
                FAN = 1;

                state = READY;

                clcd_clear();

                break;
        }
    }
}