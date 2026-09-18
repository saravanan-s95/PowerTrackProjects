#include "main.h"

extern unsigned char sec, min, temp;
extern unsigned char screen_flag;

static unsigned char door_aware_delay_ms(unsigned int total_ms)
{
    unsigned int elapsed;

    for (elapsed = 0; elapsed < total_ms; elapsed += 50)
    {
        __delay_ms(50);
        door_check();
        if (DOOR == OPEN)
        {
            return 1;
        }
    }
    return 0;
}

void power_on_screen(void)
{
    unsigned char i;

    for (i = 0; i < 16; i++)
    {
        clcd_putch(BAR, LINE1(i));
        __delay_ms(50);
        door_check(); 
    }
    clcd_print(" Powering ON  ", LINE2(1));
    clcd_print("Microwave Oven", LINE3(1));
    for (i = 0; i < 16; i++)
    {
        clcd_putch(BAR, LINE4(i));
        __delay_ms(50);
        door_check();
    }
    door_aware_delay_ms(1000);
}

void menu_display_screen(unsigned char reset_flag)
{
    /* Purely static text - only needs to be written once on entry,
     * or again if the door interlock wiped the screen. */
    if (reset_flag == RESET_MODE || reset_flag == REDRAW_ONLY)
    {
        clcd_print("1.Micro         ", LINE1(0));
        clcd_print("2.Grill         ", LINE2(0));
        clcd_print("3.Convection    ", LINE3(0));
        clcd_print("4.Quick 30s     ", LINE4(0));
    }
}

void set_time(unsigned char key, unsigned char reset_flag)
{
    static unsigned char key_count;
    static unsigned char blink_pos;  
    static unsigned int  blink_delay;
    static unsigned char blink_on;

    if (reset_flag == RESET_MODE)
    {
        min = 0;
        sec = 0;
        key_count = 0;
        blink_pos = 0;
        blink_delay = 0;
        blink_on = 0;
    }

    if (reset_flag == RESET_MODE || reset_flag == REDRAW_ONLY)
    {
        clcd_print("SET TIME (MM:SS)", LINE1(0));
        clcd_print("TIME:   :       ", LINE2(0));
        clcd_print("*:CLEAR   #:ENTER", LINE4(0));
    }

    if (key != '*' && key != '#' && key != ALL_RELEASED)
    {
        if (key_count < 2)             
        {
            min = (min * 10) + key;
            blink_pos = 0;
            key_count++;
        }
        else if (key_count == 2)        
        {
            if (key <= 5)
            {
                sec = key * 10;
                blink_pos = 1;
                key_count++;
            }
            
        }
        else if (key_count == 3)        
        {
            sec = (sec - (sec % 10)) + key;
            blink_pos = 1;
            key_count++;
        }
        blink_on = 0; 
    }
    else if (key == '*')
    {
        min = 0;
        sec = 0;
        key_count = 0;
        blink_pos = 0;
    }
    else if (key == '#')
    {
        if (min == 0 && sec == 0)
        {
            return; /* ignore Enter on an empty time */
        }
        clear_screen();
        FAN = ON;
        TMR2ON = 1;
        screen_flag = DISPLAY_TIME;
        return;
    }

    if (++blink_delay >= 150)
    {
        blink_delay = 0;
        blink_on = !blink_on;
    }

    if (blink_on)
    {
        if (blink_pos == 0)
        {
            clcd_print("  ", LINE2(5));
        }
        else
        {
            clcd_print("  ", LINE2(8));
        }
    }
    else
    {
        clcd_putch(min / 10 + '0', LINE2(5));
        clcd_putch(min % 10 + '0', LINE2(6));
        clcd_putch(sec / 10 + '0', LINE2(8));
        clcd_putch(sec % 10 + '0', LINE2(9));
    }
}

static void draw_temp_screen(void)
{
    clcd_print("SET TEMP ( C)", LINE1(0));
    clcd_putch(DEGREE, LINE1(13));
    clcd_print("TEMP =    ", LINE2(0));
    clcd_print("LIMIT: 180 C", LINE3(0));
    clcd_putch(DEGREE, LINE3(11));
    clcd_print("*:CLEAR   #:ENTER", LINE4(0));
}

void set_temp(unsigned char key, unsigned char reset_flag)
{
    static unsigned char key_count;
    static unsigned int  blink_delay;
    static unsigned char blink_on;

    if (reset_flag == RESET_MODE)
    {
        temp = 0;
        key_count = 0;
        blink_delay = 0;
        blink_on = 0;
    }

    if (reset_flag == RESET_MODE || reset_flag == REDRAW_ONLY)
    {
        draw_temp_screen();
    }

    if (key != '*' && key != '#' && key != ALL_RELEASED)
    {
        if (key_count < 3)
        {
            temp = (temp * 10) + key;
            key_count++;
        }
    }
    else if (key == '*')
    {
        temp = 0;
        key_count = 0;
    }
    else if (key == '#')
    {
        if (temp == 0 || temp > MAX_TEMP)
        {
            clear_screen();
            clcd_print(" Temp Invalid   ", LINE2(0));
            BUZZER = ON;
            if (door_aware_delay_ms(1200))
            {
                
                BUZZER = OFF;
                return;
            }
            BUZZER = OFF;
            clear_screen();
            temp = 0;
            key_count = 0;
            draw_temp_screen();
            return;
        }

     
        clear_screen();
        screen_flag = CONV_PREHEAT;
        return;
    }

    if (++blink_delay >= 150)
    {
        blink_delay = 0;
        blink_on = !blink_on;
    }

    if (blink_on)
    {
        clcd_print("   ", LINE2(7));
    }
    else
    {
        clcd_putch(temp / 100 + '0', LINE2(7));
        clcd_putch((temp / 10) % 10 + '0', LINE2(8));
        clcd_putch(temp % 10 + '0', LINE2(9));
    }
}

void preheat_screen(unsigned char reset_flag)
{
    if (reset_flag == RESET_MODE)
    {
        min = PREHEAT_SECONDS / 60;
        sec = PREHEAT_SECONDS % 60;
        FAN = ON;
        TMR2ON = 1;
    }

    if (reset_flag == RESET_MODE || reset_flag == REDRAW_ONLY)
    {
        clcd_print(" Pre-Heating... ", LINE1(0));
        clcd_print("Temp  =    ", LINE2(0));
        clcd_putch(DEGREE, LINE2(10));
        clcd_print("Time Left:      ", LINE3(0));

        clcd_putch(temp / 100 + '0', LINE2(7));
        clcd_putch((temp / 10) % 10 + '0', LINE2(8));
        clcd_putch(temp % 10 + '0', LINE2(9));
    }

    clcd_putch(min / 10 + '0', LINE3(11));
    clcd_putch(min % 10 + '0', LINE3(12));
    clcd_putch(':', LINE3(13));
    clcd_putch(sec / 10 + '0', LINE3(14));
    clcd_putch(sec % 10 + '0', LINE3(15));

    if (min == 0 && sec == 0)
    {
        TMR2ON = 0;
        FAN = OFF;
        BUZZER = ON;
        clear_screen();
        clcd_print(" Preheat Done   ", LINE2(0));
        clcd_print(" Set the Time   ", LINE3(0));
        if (door_aware_delay_ms(1200))
        {
            /* door opened mid-alert - leave screen_flag on CONV_PREHEAT;
               once the door closes, force_redraw brings us back here with
               min==sec==0 and we cleanly re-run this same alert */
            BUZZER = OFF;
            return;
        }
        BUZZER = OFF;
        clear_screen();
        screen_flag = CONV_SET_TIME; /* re-use the MM:SS entry screen */
    }
}

void time_display_screen(unsigned char reset_flag)
{
    if (reset_flag == RESET_MODE || reset_flag == REDRAW_ONLY)
    {
        clcd_print("TIME =", LINE1(0));
        clcd_print("4.Start 5.Pause ", LINE2(0));
        clcd_print("6.Stop          ", LINE3(0));
        clcd_print("                ", LINE4(0));
    }

    clcd_putch(min / 10 + '0', LINE1(7));
    clcd_putch(min % 10 + '0', LINE1(8));
    clcd_putch(':', LINE1(9));
    clcd_putch(sec / 10 + '0', LINE1(10));
    clcd_putch(sec % 10 + '0', LINE1(11));

    if (min == 0 && sec == 0)
    {
        TMR2ON = 0;
        FAN = OFF;
        BUZZER = ON;
        clear_screen();
        clcd_print("   TIME UP!!!   ", LINE2(0));
        clcd_print(" Enjoy your meal", LINE3(0));
        if (door_aware_delay_ms(1500))
        {
            /* door opened mid-alert - leave screen_flag on DISPLAY_TIME;
               once the door closes, force_redraw brings us back here with
               min==sec==0 and we cleanly re-run this same alert */
            BUZZER = OFF;
            return;
        }
        BUZZER = OFF;
        clear_screen();
        screen_flag = MENU_SCREEN;
    }
}