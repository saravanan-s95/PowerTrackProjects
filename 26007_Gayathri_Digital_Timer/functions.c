#include <xc.h>
#include "main.h"
#include "digital_keypad.h"
#include "clcd.h"
#include "functions.h"

// Function: show_Default
// Purpose: Show Main Clock Screen - Date and Time
// When event_display_flag = 0 -> Show Date + Time
// When event_display_flag = 1 -> Show Time + Next Event (Blinking Style)
void show_Default()
{
    if (event_display_flag == 0)
    {
        // Line 1: Display Date - Format: DATE DD-MM-YY
        clcd_print("DATE", LINE1(0));
        clcd_putch(myDate.day / 10 + '0', LINE1(5));   // Tens Digit of Day
        clcd_putch(myDate.day % 10 + '0', LINE1(6));   // Units Digit of Day
        clcd_putch('-', LINE1(7));
        clcd_putch(myDate.month / 10 + '0', LINE1(8));  // Tens Digit of Month
        clcd_putch(myDate.month % 10 + '0', LINE1(9)); // Units Digit of Month
        clcd_putch('-', LINE1(10));
        clcd_putch((myDate.year / 10) % 10 + '0', LINE1(11)); // Tens Digit of Year (Last 2 Digits)
        clcd_putch(myDate.year % 10 + '0', LINE1(12));        // Units Digit of Year
        clcd_print("   ", LINE1(13)); // Clear Remaining Spaces

        // Line 2: Display Time - Format: TIME HH:MM AM/PM
        clcd_print("TIME", LINE2(0));
        clcd_putch(myTime.hour / 10 + '0', LINE2(5));
        clcd_putch(myTime.hour % 10 + '0', LINE2(6));
        clcd_putch(':', LINE2(7));
        clcd_putch(myTime.min / 10 + '0', LINE2(8));
        clcd_putch(myTime.min % 10 + '0', LINE2(9));
        clcd_print(" ", LINE2(10));
        if (myTime.am_pm == 1)
        {
            clcd_print("PM", LINE2(11)); // 1 = PM
        }
        else
        {
            clcd_print("AM", LINE2(11)); // 0 = AM
        }
    }
    else
    {
        // Alternate Screen: Show Time on Line 1 and Event on Line 2
        clcd_print("TIME", LINE1(0));
        clcd_print(" ", LINE2(4));
        clcd_putch(myTime.hour / 10 + '0', LINE1(5));
        clcd_putch(myTime.hour % 10 + '0', LINE1(6));
        clcd_putch(':', LINE1(7));
        clcd_putch(myTime.min / 10 + '0', LINE1(8));
        clcd_putch(myTime.min % 10 + '0', LINE1(9));
        clcd_print("PM", LINE1(10));

        // Line 2: EVENT HH:MM Type
        clcd_print("EVENT", LINE2(0));
        clcd_print(" ", LINE2(5));
        clcd_putch(myEvent[event_num].hour / 10 + '0', LINE2(6));
        clcd_putch(myEvent[event_num].hour % 10 + '0', LINE2(7));
        clcd_putch(':', LINE2(8));
        clcd_putch(myEvent[event_num].min / 10 + '0', LINE2(9));
        clcd_putch(myEvent[event_num].min % 10 + '0', LINE2(10));
        clcd_print(" ", LINE2(11));
        clcd_print("PM", LINE2(12));
        clcd_print(" ", LINE2(14));
        
        // Event Type: O = Once, D = Daily, W = Weekly
        if (myEvent[event_num].type == 0)
            clcd_putch('O', LINE2(15)); // ONCE
        else if (myEvent[event_num].type == 1)
            clcd_putch('D', LINE2(15)); // Daily
        else
            clcd_putch('W', LINE2(15)); // Weekly
    }
}

// Function: show_config
// Purpose: Show CONFIG Menu - Select Between SET VIEW EVENT / SET TIME DATE
void show_config()
{
    if (config_select == 0)
    {
        // Cursor (->) on First Option
        clcd_print("-> SET VIEW EVENT", LINE1(0));
        clcd_print("  SET TIME DATE ", LINE2(0));
    }
    else
    {
        // Cursor (->) on Second Option
        clcd_print("  SET VIEW EVENT", LINE1(0));
        clcd_print("-> SET TIME DATE ", LINE2(0));
    }
}

// Function: show_set_view_event
// Purpose: Show SET / VIEW Menu 
void show_set_view_event()
{
    if (set_view_select == 0)
    {
        clcd_print("-> SET EVENT     ", LINE1(0));
        clcd_print("  VIEW EVENT    ", LINE2(0));
    }
    else
    {
        clcd_print("  SET EVENT     ", LINE1(0));
        clcd_print("-> VIEW EVENT    ", LINE2(0));
    }
}

// Function: set_event
// Purpose: Edit Event Details - Time, Type, Duration
// SW2 = Move to Next Field, SW1 = Increment Current Field Value
void set_event(void)
{
    // Line 1: Show Event Time and Type - Format: TIME: HH:MM PM O/D/W
    clcd_print("TIME: ", LINE1(0));
    clcd_putch(myEvent[event_num].hour / 10 + '0', LINE1(6));
    clcd_putch(myEvent[event_num].hour % 10 + '0', LINE1(7));
    clcd_putch(':', LINE1(8));
    clcd_putch(myEvent[event_num].min / 10 + '0', LINE1(9));
    clcd_putch(myEvent[event_num].min % 10 + '0', LINE1(10));
    clcd_print(" ", LINE1(11));
    clcd_print("PM", LINE1(12));
    clcd_print(" ", LINE1(14));
    if (myEvent[event_num].type == 0)
        clcd_putch('O', LINE1(15)); // Once
    else if (myEvent[event_num].type == 1)
        clcd_putch('D', LINE1(15)); // Daily
    else
        clcd_putch('W', LINE1(15)); // Weekly

    // Line 2: Show Duration - Format: DUR: HH:MM:SS
    clcd_print("DUR: ", LINE2(0));
    clcd_putch(myEvent[event_num].duration_hour / 10 + '0', LINE2(5));
    clcd_putch(myEvent[event_num].duration_hour % 10 + '0', LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(myEvent[event_num].duration_min / 10 + '0', LINE2(8));
    clcd_putch(myEvent[event_num].duration_min % 10 + '0', LINE2(9));
    clcd_putch(':', LINE2(10));
    clcd_putch((char)(myEvent[event_num].duration_sec / 10 + '0'), LINE2(11));
    clcd_putch((char)(myEvent[event_num].duration_sec % 10 + '0'), LINE2(12));

    // Detect New Key Press Only (Edge Detection)
    static unsigned char prev_key = 0xFF;
    if(key != prev_key)
    {
        if (key == SW2)
        {
            // Move to Next Field (0=Hour, 1=Min, 2=Type, 3=Dur Hour, 4=Dur Min)
            field++;
            if (field >= 5)
                field = 0; // Wrap Around to First Field
        }
        else if (key == SW1)
        {
            // Increment Value of Current Field
            if (field == 0)
            {
                myEvent[event_num].hour++;
                if (myEvent[event_num].hour > 12)
                    myEvent[event_num].hour = 1; // Hour Range 1-12
            }
            else if (field == 1)
            {
                myEvent[event_num].min++;
                if (myEvent[event_num].min >= 60)
                    myEvent[event_num].min = 0; // Minute Range 0-59
            }
            else if (field == 2)
            {
                myEvent[event_num].type++;
                if (myEvent[event_num].type >= 3)
                    myEvent[event_num].type = 0; // Type Cycle O -> D -> W -> O
            }
            else if (field == 3)
            {
                myEvent[event_num].duration_hour++;
                if (myEvent[event_num].duration_hour >= 24)
                    myEvent[event_num].duration_hour = 0; // Duration Hour 0-23
            }
            else if (field == 4)
            {
                myEvent[event_num].duration_min++;
                if (myEvent[event_num].duration_min >= 60)
                    myEvent[event_num].duration_min = 0; // Duration Min 0-59
            }
        }
        prev_key = key; // Update Previous Key
    }
}

// Function: view_event
// Purpose: Display Event Details in Read-Only Mode
// Short Press SW1 / SW2 = Toggle Between Event 1 and Event 2
// Long Press SW2 (Handled in main.c) = Go Back to SET/VIEW Menu
void view_event(void)
{
    static unsigned char prev_event = 0xFF;
    // Update LCD Only When Event Number Changes (
    if(prev_event != event_num)
    {
        clcd_print("                ", LINE2(0));
        clcd_print("   ", LINE1(13));
        // Line 1: EVENT X:HH:MM
        clcd_print("EVENT ", LINE1(0));
        clcd_putch(event_num + '1', LINE1(6)); // Display 1 for Event 0, 2 for Event 1
        clcd_putch(':', LINE1(7));
        clcd_putch(myEvent[event_num].hour / 10 + '0', LINE1(8));
        clcd_putch(myEvent[event_num].hour % 10 + '0', LINE1(9));
        clcd_putch(':', LINE1(10));
        clcd_putch(myEvent[event_num].min / 10 + '0', LINE1(11));
        clcd_putch(myEvent[event_num].min % 10 + '0', LINE1(12));
        clcd_print("   ", LINE1(13));

        // Line 2: Type and ON/OFF Status
        if (myEvent[event_num].type == 0)
            clcd_print("O", LINE2(0)); // Once
        else if (myEvent[event_num].type == 1)
            clcd_print("D", LINE2(0)); // Daily
        else
            clcd_print("W", LINE2(0)); // Weekly
        clcd_print("   ", LINE2(1));
        if (myEvent[event_num].enabled)
            clcd_print("ON ", LINE2(5)); // Event Enabled
        else
            clcd_print("OFF", LINE2(5)); // Event Disabled
        clcd_print("        ", LINE2(8));
        prev_event = event_num;
    }
    
    // Toggle Event Number on Short Press
    static unsigned char prev_key = 0xFF;
    if(key != prev_key)
    {
        if (key == SW1 || key == SW2)
        {
            // Switch Between Event 1 and Event 2
            if (event_num == 0)
                event_num = 1;
            else
                event_num = 0;
        }
        prev_key = key;
    }
}

// Function: show_set_time_date
// Purpose: Show SET TIME / SET DATE Menu
void show_set_time_date(void)
{
    if (set_time_date_select == 0)
    {
        clcd_print("-> SET TIME      ", LINE1(0));
        clcd_print("  SET DATE      ", LINE2(0));
    }
    else
    {
        clcd_print("  SET TIME      ", LINE1(0));
        clcd_print("-> SET DATE      ", LINE2(0));
    }
}

// Function: set_time
// Purpose: Edit Current Time - HH:MM:SS with Blinking Field
// SW2 = Move to Next Field, SW1 = Increment Value
void set_time(void)
{
    clcd_print("HH:MM:SS", LINE1(0));

    // Hour Field - Blink if Selected (field == 0) and blink_flag = 1
    if (field == 0 && blink_flag == 1)
        clcd_print("  ", LINE2(0)); // Hide During Blink OFF Phase
    else
    {
        clcd_putch(myTime.hour / 10 + '0', LINE2(0));
        clcd_putch(myTime.hour % 10 + '0', LINE2(1));
    }
    clcd_print(":", LINE2(2));

    // Minute Field - Blink if Selected (field == 1)
    if (field == 1 && blink_flag == 1)
        clcd_print("  ", LINE2(3));
    else
    {
        clcd_putch(myTime.min / 10 + '0', LINE2(3));
        clcd_putch(myTime.min % 10 + '0', LINE2(4));
    }
    clcd_print(":", LINE2(5));

    // Second Field - Blink if Selected (field == 2)
    if (field == 2 && blink_flag == 1)
        clcd_print("  ", LINE2(6));
    else
    {
        clcd_putch(myTime.sec / 10 + '0', LINE2(6));
        clcd_putch(myTime.sec % 10 + '0', LINE2(7));
    }
    static unsigned char prev_key = 0xFF;
    if(key != prev_key)
    {
        if (key == SW2)
        {
            field++; // Move to Next Field
            if (field >= 3)
                field = 0; // Wrap Around
        }
        else if (key == SW1)
        {
            if (field == 0)
            {
                myTime.hour++;
                if (myTime.hour > 12)
                    myTime.hour = 1;
            }
            else if (field == 1)
            {
                myTime.min++;
                if (myTime.min >= 60)
                    myTime.min = 0;
            }
            else if (field == 2)
            {
                myTime.sec++;
                if (myTime.sec >= 60)
                    myTime.sec = 0;
            }
        }
        prev_key = key;
    }
}

// Function: stop_date (set_date)
// Purpose: Edit Current Date - DD-MM-YY with Blinking Field
void stop_date(void)
{
    clcd_print("DD-MM-YY", LINE1(0));

    // Day Field
    if (field == 0 && blink_flag == 1)
        clcd_print("  ", LINE2(0));
    else
    {
        clcd_putch(myDate.day / 10 + '0', LINE2(0));
        clcd_putch(myDate.day % 10 + '0', LINE2(1));
    }
    clcd_print("-", LINE2(2));

    // Month Field
    if (field == 1 && blink_flag == 1)
        clcd_print("  ", LINE2(3));
    else
    {
        clcd_putch(myDate.month / 10 + '0', LINE2(3));
        clcd_putch(myDate.month % 10 + '0', LINE2(4));
    }
    clcd_print("-", LINE2(5));

    // Year Field (Last 2 Digits)
    if (field == 2 && blink_flag == 1)
        clcd_print("  ", LINE2(6));
    else
    {
        clcd_putch((myDate.year / 10) % 10 + '0', LINE2(6));
        clcd_putch(myDate.year % 10 + '0', LINE2(7));
    }
    static unsigned char prev_key = 0xFF;
    if(key != prev_key)
    {
        if (key == SW2)
        {
            field++;
            if (field >= 3)
                field = 0;
        }
        else if (key == SW1)
        {
            if (field == 0)
            {
                myDate.day++;
                if (myDate.day > 31)
                    myDate.day = 1; // Day Range 1-31
            }
            else if (field == 1)
            {
                myDate.month++;
                if (myDate.month > 12)
                    myDate.month = 1; // Month Range 1-12
            }
            else if (field == 2)
            {
                myDate.year++;
                if (myDate.year > 2099)
                    myDate.year = 2000; // Year Wrap 2000-2099
            }
        }
        prev_key = key;
    }
}

// Function: Check_Alarm
// Purpose: Check if Current Time Matches Any Enabled Event Time
// If Match, Turn ON Buzzer for Duration Time
void Check_Alarm()
{
    static unsigned char alarm_on[2] = {0, 0}; // Alarm Active Flag for Each Event
    static unsigned char start_min[2] = {0, 0}; // Alarm Start Minute
    static unsigned char start_hour[2] = {0, 0}; // Alarm Start Hour

    for (int i = 0; i < 2; i++)
    {
        // Check Only Enabled Events
        if (myEvent[i].enabled == 1) 
        {
            // Trigger Condition: Alarm Not Yet ON and Time Matches and Sec == 0
            if (alarm_on[i] == 0 && myEvent[i].hour == myTime.hour && myEvent[i].min == myTime.min && myTime.sec == 0)
            {
                unsigned char trigger = 0;

                if (myEvent[i].type == 0) // ONCE - Trigger One Time Only
                {
                    trigger = 1;
                }
                else if (myEvent[i].type == 1) // DAILY - Trigger Every Day
                {
                    trigger = 1;
                }
                else if (myEvent[i].type == 2) // WEEKLY - Trigger Only on Specific Day
                {
                    if (myDate.day % 7 == 0)
                        trigger = 1;
                }

                if (trigger == 1)
                {
                    BUZZER = 1; // Turn ON Buzzer
                    alarm_on[i] = 1;
                    start_hour[i] = myTime.hour;
                    start_min[i] = myTime.min;

                    if (myEvent[i].type == 0)
                        myEvent[i].enabled = 0; // Disable ONCE Event After Trigger
                }
            }

            // Check if Alarm Duration is Completed, Then Turn OFF Buzzer
            if (alarm_on[i] == 1)
            {
                unsigned int start_total = start_hour[i] * 60 + start_min[i]; // Start Time in Minutes
                unsigned int curr_total = myTime.hour * 60 + myTime.min; // Current Time in Minutes
                unsigned int dur_total = myEvent[i].duration_hour * 60 + myEvent[i].duration_min; // Duration in Minutes

                if (dur_total == 0)
                    dur_total = 1; // Minimum 1 Minute if Duration is 0

                if ((curr_total - start_total) >= dur_total)
                {
                    BUZZER = 0; // Turn OFF Buzzer
                    alarm_on[i] = 0;
                }
            }
        }
    }
}

// Function: Increment_Time
// Purpose: Increment Clock by 1 Second, Handle Rollover and AM/PM Toggle
void Increment_Time()
{
    myTime.sec++;

    if (myTime.sec >= 60)
    {
        myTime.sec = 0;
        myTime.min++;

        if (myTime.min >= 60)
        {
            myTime.min = 0;
            myTime.hour++;

            // Toggle AM/PM When Hour Reaches 12
            if (myTime.hour == 12)
            {
                if (myTime.am_pm == 0)
                    myTime.am_pm = 1;
                else
                    myTime.am_pm = 0;
            }

            if (myTime.hour > 12)
            {
                myTime.hour = 1; // Hour Wrap 1-12
            }
        }
    }
}

// Function: stop_alarm
// Purpose: Stop Buzzer Manually When Any Key is Pressed
void stop_alarm()
{
    if (key == SW1 || key == SW2)
        BUZZER = 0;
}