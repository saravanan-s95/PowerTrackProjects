/*
 * File:   main.c
 * Author: Dell
 *
 * Created on 11 August, 2026, 7:33 PM
 */

#include "main.h"
#include "clcd.h"
#include "rtc.h"
#include "matrix_keypad.h"
#include "timer1.h"
#include "digital_timer.h"


// ================= CONFIG MENU =================

unsigned char config_menu(void)
{
    unsigned char key;
    unsigned char option = 0;

    clcd_clear();

    while(1)
    {
        if(option == 0)
        {
            clcd_print("SET TIME & DATE ", LINE1(0));
            clcd_print("SET / VIEW ALARM", LINE2(0));
        }
        else if(option == 1)
        {
            clcd_print("SET / VIEW ALARM", LINE1(0));
            clcd_print("SET TIMER       ", LINE2(0));
        }
        else
        {
            clcd_print("SET TIMER       ", LINE1(0));
            clcd_print("SET TIME & DATE ", LINE2(0));
        }

        key = read_switches();

        // Move down
        if(key == DOWN)
        {
            option++;

            if(option > 2)
            {
                option = 0;
            }

            while(read_switches() != 0);
        }

        // Move up
        else if(key == UP)
        {
            if(option == 0)
            {
                option = 2;
            }
            else
            {
                option--;
            }

            while(read_switches() != 0);
        }

        // Select option
        else if(key == RIGHT)
        {
            while(read_switches() != 0);

            return option;
        }
    }
}


// ================= MAIN =================

void main(void)
{
    unsigned char key;

    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;

    unsigned char alarm_hour = 0;
    unsigned char alarm_minute = 0;
    unsigned char alarm_enabled = 0;
    unsigned char alarm_triggered = 0;

    unsigned char day;
    unsigned char month;
    unsigned char year;

    unsigned char selected_option;


    // ================= INITIALIZATION =================

    init_clcd();
    init_matrix_keypad();
    rtc_init();
    init_timer1();

    TRISC1 = 0;
    RC1 = 0;


    // ================= READ TIME =================

    hours = bcd_to_dec(rtc_read(0x02));
    minutes = bcd_to_dec(rtc_read(0x01));
    seconds = bcd_to_dec(rtc_read(0x00));


    // ================= READ DATE =================

    day = bcd_to_dec(rtc_read(0x04));
    month = bcd_to_dec(rtc_read(0x05));
    year = bcd_to_dec(rtc_read(0x06));


    // ================= HOME PAGE =================

    clcd_clear();

    clcd_print("DIGITAL TIMER", LINE1(0));


    // ================= MAIN LOOP =================

    while(1)
    {
        // Read current time continuously from RTC
        hours = bcd_to_dec(rtc_read(0x02));
        minutes = bcd_to_dec(rtc_read(0x01));
        seconds = bcd_to_dec(rtc_read(0x00));


        key = read_switches();

        // Display current RTC time on Home Page
        if(key != RIGHT)
        {
            clcd_putch((hours / 10) + '0', LINE2(0));
            clcd_putch((hours % 10) + '0', LINE2(1));   

            clcd_putch(':', LINE2(2));

            clcd_putch((minutes / 10) + '0', LINE2(3));
            clcd_putch((minutes % 10) + '0', LINE2(4));

            clcd_putch(':', LINE2(5));

            clcd_putch((seconds / 10) + '0', LINE2(6));
            clcd_putch((seconds % 10) + '0', LINE2(7));
            
        }

        // ================= ALARM CHECK =================

        if(alarm_enabled == 1)
        {
            if(hours == alarm_hour && minutes == alarm_minute)
            {
                if(alarm_triggered == 0)
                {
                    alarm_triggered = 1;

                    // Turn ON buzzer
                    RC1 = 1;

                    clcd_clear();
                    clcd_print("ALARM !!!", LINE1(0));
                    clcd_print("WAKE UP", LINE2(0));

                    __delay_ms(5000);

                    // Turn OFF buzzer
                    RC1 = 0;

                    // Return to Home Page
                    clcd_clear();

                    clcd_print("DIGITAL TIMER", LINE1(0));
                }
            }
            else
            {
                // Allow alarm to trigger again next minute
                alarm_triggered = 0;
            }
        }


        // ==================================================
        // ENTER CONFIGURATION MENU
        // ==================================================

        if(key == RIGHT)
        {
            while(read_switches() != 0);

            selected_option = config_menu();


            // ==================================================
            // SET TIME & DATE
            // ==================================================

            if(selected_option == 0)
            {
                // ---------------- HOUR ----------------

                clcd_clear();

                clcd_print("SET TIME", LINE1(0));
                clcd_print("HOUR: ", LINE2(0));

                while(1)
                {
                    clcd_putch((hours / 10) + '0', LINE2(6));
                    clcd_putch((hours % 10) + '0', LINE2(7));

                    key = read_switches();


                    if(key == UP)
                    {
                        hours++;

                        if(hours > 23)
                        {
                            hours = 0;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(hours == 0)
                        {
                            hours = 23;
                        }
                        else
                        {
                            hours--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- MINUTE ----------------

                clcd_clear();

                clcd_print("SET TIME", LINE1(0));
                clcd_print("MIN: ", LINE2(0));

                while(1)
                {
                    clcd_putch((minutes / 10) + '0', LINE2(5));
                    clcd_putch((minutes % 10) + '0', LINE2(6));

                    key = read_switches();


                    if(key == UP)
                    {
                        minutes++;

                        if(minutes > 59)
                        {
                            minutes = 0;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(minutes == 0)
                        {
                            minutes = 59;
                        }
                        else
                        {
                            minutes--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- SECOND ----------------

                clcd_clear();

                clcd_print("SET TIME", LINE1(0));
                clcd_print("SEC: ", LINE2(0));

                while(1)
                {
                    clcd_putch((seconds / 10) + '0', LINE2(5));
                    clcd_putch((seconds % 10) + '0', LINE2(6));

                    key = read_switches();


                    if(key == UP)
                    {
                        seconds++;

                        if(seconds > 59)
                        {
                            seconds = 0;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(seconds == 0)
                        {
                            seconds = 59;
                        }
                        else
                        {
                            seconds--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);

                        rtc_write(0x02, dec_to_bcd(hours));
                        rtc_write(0x01, dec_to_bcd(minutes));
                        rtc_write(0x00, dec_to_bcd(seconds));

                        clcd_clear();

                        clcd_print("TIME SAVED", LINE1(0));

                        __delay_ms(1000);

                        break;
                    }
                }


                // ---------------- DAY ----------------

                clcd_clear();

                clcd_print("SET DATE", LINE1(0));
                clcd_print("DAY: ", LINE2(0));

                while(1)
                {
                    clcd_putch((day / 10) + '0', LINE2(5));
                    clcd_putch((day % 10) + '0', LINE2(6));

                    key = read_switches();


                    if(key == UP)
                    {
                        day++;

                        if(day > 31)
                        {
                            day = 1;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(day <= 1)
                        {
                            day = 31;
                        }
                        else
                        {
                            day--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- MONTH ----------------

                clcd_clear();

                clcd_print("SET DATE", LINE1(0));
                clcd_print("MONTH: ", LINE2(0));

                while(1)
                {
                    clcd_putch((month / 10) + '0', LINE2(7));
                    clcd_putch((month % 10) + '0', LINE2(8));

                    key = read_switches();


                    if(key == UP)
                    {
                        month++;

                        if(month > 12)
                        {
                            month = 1;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(month <= 1)
                        {
                            month = 12;
                        }
                        else
                        {
                            month--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- YEAR ----------------

                clcd_clear();

                clcd_print("SET DATE", LINE1(0));
                clcd_print("YEAR: ", LINE2(0));

                while(1)
                {
                    clcd_putch((year / 10) + '0', LINE2(6));
                    clcd_putch((year % 10) + '0', LINE2(7));

                    key = read_switches();


                    if(key == UP)
                    {
                        year++;

                        if(year > 99)
                        {
                            year = 0;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(year == 0)
                        {
                            year = 99;
                        }
                        else
                        {
                            year--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);

                        rtc_write(0x04, dec_to_bcd(day));
                        rtc_write(0x05, dec_to_bcd(month));
                        rtc_write(0x06, dec_to_bcd(year));

                        clcd_clear();

                        clcd_print("DATE SAVED", LINE1(0));

                        __delay_ms(1000);

                        break;
                    }
                }


                // Return to HOME

                clcd_clear();

                clcd_print("DIGITAL TIMER", LINE1(0));
                clcd_print("RIGHT: CONFIG", LINE2(0));
            }


            // ==================================================
            // SET / VIEW ALARM
            // ==================================================

            else if(selected_option == 1)
            {
                // ---------------- ALARM ENTRY ----------------

                clcd_clear();

                clcd_print("SET ALARM", LINE1(0));
                clcd_print("RIGHT: ENTER", LINE2(0));

                while(1)
                {
                    key = read_switches();

                    if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- ALARM HOUR ----------------

                clcd_clear();

                clcd_print("SET ALARM", LINE1(0));
                clcd_print("HOUR: ", LINE2(0));

                while(1)
                {
                    clcd_putch((alarm_hour / 10) + '0', LINE2(6));
                    clcd_putch((alarm_hour % 10) + '0', LINE2(7));

                    key = read_switches();


                    if(key == UP)
                    {
                        alarm_hour++;

                        if(alarm_hour > 23)
                        {
                            alarm_hour = 0;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(alarm_hour == 0)
                        {
                            alarm_hour = 23;
                        }
                        else
                        {
                            alarm_hour--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- ALARM MINUTE ----------------

                clcd_clear();

                clcd_print("SET ALARM", LINE1(0));
                clcd_print("MIN: ", LINE2(0));

                while(1)
                {
                    clcd_putch((alarm_minute / 10) + '0', LINE2(5));
                    clcd_putch((alarm_minute % 10) + '0', LINE2(6));

                    key = read_switches();


                    if(key == UP)
                    {
                        alarm_minute++;

                        if(alarm_minute > 59)
                        {
                            alarm_minute = 0;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == DOWN)
                    {
                        if(alarm_minute == 0)
                        {
                            alarm_minute = 59;
                        }
                        else
                        {
                            alarm_minute--;
                        }

                        while(read_switches() != 0);
                    }


                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- ALARM ON / OFF ----------------

                clcd_clear();

                clcd_print("ALARM", LINE1(0));

                if(alarm_enabled == 0)
                {
                    clcd_print("OFF", LINE2(0));
                }
                else
                {
                    clcd_print("ON", LINE2(0));
                }


                while(1)
                {
                    key = read_switches();


                    // UP or DOWN = toggle alarm

                    if(key == UP || key == DOWN)
                    {
                        if(alarm_enabled == 0)
                        {
                            alarm_enabled = 1;

                            clcd_clear();

                            clcd_print("ALARM", LINE1(0));
                            clcd_print("ON", LINE2(0));
                        }
                        else
                        {
                            alarm_enabled = 0;

                            clcd_clear();

                            clcd_print("ALARM", LINE1(0));
                            clcd_print("OFF", LINE2(0));
                        }

                        while(read_switches() != 0);
                    }


                    // RIGHT = SAVE ALARM

                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);

                        break;
                    }
                }


                // ==================================================
                // ALARM SAVED
                // ==================================================

                clcd_clear();

                clcd_print("ALARM SET", LINE1(0));
                clcd_print("SAVED", LINE2(0));

                __delay_ms(1000);


                // ==================================================
                // RETURN TO HOME
                // ==================================================

                clcd_clear();

                clcd_print("DIGITAL TIMER", LINE1(0));

                while(read_switches() != 0);
            }


            // ==================================================
            // SET TIMER
            // ==================================================

            else if(selected_option == 2)
            {
                // Reset timer values

                minute = 0;
                second = 0;
                state = 0;


                // ---------------- SET MINUTE ----------------

                clcd_clear();

                clcd_print("SET TIMER", LINE1(0));
                clcd_print("MIN: 00", LINE2(0));

                while(1)
                {
                    clcd_putch((minute / 10) + '0', LINE2(5));
                    clcd_putch((minute % 10) + '0', LINE2(6));

                    key = read_switches();


                    // Increase minute

                    if(key == UP)
                    {
                        minute++;

                        if(minute > 99)
                        {
                            minute = 0;
                        }

                        while(read_switches() != 0);
                    }


                    // Decrease minute

                    else if(key == DOWN)
                    {
                        if(minute == 0)
                        {
                            minute = 99;
                        }
                        else
                        {
                            minute--;
                        }

                        while(read_switches() != 0);
                    }


                    // Move to seconds

                    else if(key == RIGHT)
                    {
                        while(read_switches() != 0);
                        break;
                    }
                }


                // ---------------- SET SECOND ----------------

                clcd_clear();

                clcd_print("SET TIMER", LINE1(0));
                clcd_print("SEC: 00", LINE2(0));

                while(1)
                {
                    clcd_putch((second / 10) + '0', LINE2(5));
                    clcd_putch((second % 10) + '0', LINE2(6));

                    key = read_switches();


                    // Increase second

                    if(key == UP)
                    {
                        second++;

                        if(second > 59)
                        {
                            second = 0;
                        }

                        while(read_switches() != 0);
                    }


                    // Decrease second

                    else if(key == DOWN)
                    {
                        if(second == 0)
                        {
                            second = 59;
                        }
                        else
                        {
                            second--;
                        }

                        while(read_switches() != 0);
                    }


                    // Start timer

                    else if(key == RIGHT)
                    {
                    while(read_switches() != 0);

                    state = RUNNING;

                    clcd_clear();
                    clcd_print("TIMER RUNNING", LINE1(0));

                    while(state == RUNNING)
                    {
                        clcd_putch((minute / 10) + '0', LINE2(0));
                        clcd_putch((minute % 10) + '0', LINE2(1));

                        clcd_putch(':', LINE2(2));

                        clcd_putch((second / 10) + '0', LINE2(3));
                        clcd_putch((second % 10) + '0', LINE2(4));

                        if(minute == 0 && second == 0)
                        {
                            state = 0;

                            RC1 = 1;

                            clcd_clear();
                            clcd_print("TIMER DONE", LINE1(0));
                            clcd_print("TIME UP", LINE2(0));

                            __delay_ms(5000);

                            RC1 = 0;
                        }
                    }

                    clcd_clear();

                    clcd_print("DIGITAL TIMER", LINE1(0));
                    clcd_print("RIGHT: CONFIG", LINE2(0));

                    break;  
                }
                }


                // ---------------- RETURN HOME ----------------

                clcd_clear();

                clcd_print("DIGITAL TIMER", LINE1(0));
            }
        }
    }
}