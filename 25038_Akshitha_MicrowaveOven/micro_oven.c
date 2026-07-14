#include "mainn.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "micro_oven.h"
#include "timers.h"

signed char min, sec;
unsigned char timer_flag;

static void set_time(unsigned char key)
{
    static unsigned char delay = 6, field_flag = 0, place_flag = 0;
    
    clcd_print("SET TIME <MM:SS>", LINE1(0));
    clcd_print("TIME: ", LINE2(0));
    
    if(key != ALL_RELEASED)
    {
        if(key != '*' && key != '#' && (place_flag == 0 || place_flag == 1))
        {
            sec = (sec * 10) + key;
            place_flag++;
            
            if(place_flag == 2)
            {
                field_flag = 1;
            }
        }
        else if(key != '*' && key != '#' && (place_flag == 2 || place_flag == 3))
        {
            min = (min * 10) + key;
            place_flag++;
        }
        else if(key == '*')
        {
            min = 0;
            sec = 0;
            field_flag = 0;
            place_flag = 0;
        }
        else if(key == '#')
        {
            field_flag = 0;
            place_flag = 0;
            timer_flag = 1;
            clear_screen();
            TMR2ON = 1;
            RC2 = 1;
            return;
        }
    }
    
    if(field_flag == 0)
    {
        clcd_putch((min / 10) + '0', LINE2(6));
        clcd_putch((min % 10) + '0', LINE2(7));
        clcd_putch(':', LINE2(8));
        
        if(--delay > 3)
        {
            clcd_putch((sec / 10) + '0', LINE2(9));
            clcd_putch((sec % 10) + '0', LINE2(10));
        }
        else if(delay > 0 && delay < 3)
        {
            clcd_print("  ", LINE2(9));
        }
        else if(delay <= 0)
        {
            delay = 6;
        }
    }
    else if(field_flag == 1)
    {
        if(--delay > 3)
        {
            clcd_putch((min / 10) + '0', LINE2(6));
            clcd_putch((min % 10) + '0', LINE2(7));
        }
        else if(delay > 0 && delay < 3)
        {
            clcd_print("  ", LINE2(6));
        }
        else if(delay <= 0)
        {
            delay = 6;
        }
        
        clcd_putch(':', LINE2(8));
        clcd_putch((sec / 10) + '0', LINE2(9));
        clcd_putch((sec % 10) + '0', LINE2(10));
    }
    
    clcd_print("*:CLEAR  #:ENTER", LINE4(0));
}

static void timer(unsigned char key)
{
    static unsigned char pause_flag = 0;
    
    if(key == 4 && pause_flag == 0)
    {
        sec = sec + 30;
        if(sec > 60)
        {
            min = min + 1;
            sec = sec % 60;
        }
    }
    else if(key == 4 && pause_flag == 1)
    {
        pause_flag = 0;
        RC2 = 1;
        TMR2ON = 1;
    }
    else if(key == 5)
    {
        pause_flag = 1;
        RC2 = 0;
        TMR2ON = 0;
    }
    else if(key == 6)
    {
        RC2 = 0;
        pause_flag = 0;
        timer_flag = 3;
        clear_screen();
        TMR2ON = 0;
        return;
    }
    
    clcd_print("TIME = ", LINE1(1));
    clcd_putch((min / 10) + '0', LINE1(8));
    clcd_putch((min % 10) + '0', LINE1(9));
    clcd_putch(':', LINE1(10));
    clcd_putch((sec / 10) + '0', LINE1(11));
    clcd_putch((sec % 10) + '0', LINE1(12));
    clcd_print("4.START/RESUME", LINE2(2));
    clcd_print("5.PAUSE", LINE3(2));
    clcd_print("6.STOP", LINE4(2));
}

static void pre_heat(unsigned char key)
{
    static unsigned char delay = 6;
    static int temp = 0;
    
    clcd_print("SET Temp <*C>", LINE1(0));
    clcd_print("TEMP = ", LINE2(0));
    
    if(key != ALL_RELEASED)
    {
        if(key != '*' && key != '#')
        {
            temp = (temp * 10) + key;
            if(temp > 180)
            {
                clear_screen();
                clcd_print("Temp should not", LINE2(1));
                clcd_print("exceed 180*C", LINE3(2));
                __delay_ms(3000);
                clear_screen();
                temp = 0;
            }
        }
        else if(key == '*')
        {
            temp = 0;
        }
        else if(key == '#' && temp >= 25)
        {
            clear_screen();
            clcd_print("Pre-heating", LINE1(2));
            clcd_print("Time left: 0", LINE2(0));
            clcd_putch('s', LINE2(14));
            sec = 59;
            TMR2ON = 1;
            while(sec)
            {
                clcd_putch((sec / 10) + '0', LINE2(12));
                clcd_putch((sec % 10) + '0', LINE2(13));
            }
            TMR2ON = 0;
            timer_flag = 0;
            clear_screen();
            return;
        }
    }
    
    if(--delay > 3)
    {
        clcd_putch((temp / 100) + '0', LINE2(7));
        clcd_putch(((temp / 10) % 10) + '0', LINE2(8));
        clcd_putch((temp % 10) + '0', LINE2(9));
    }
    else if(delay > 0 && delay < 3)
    {
        clcd_print("   ", LINE2(7));
    }
    else if(delay <= 0)
    {
        delay = 6;
    }
    
    clcd_print("*:CLEAR  #:ENTER", LINE4(0));
}

void power_up(void)
{
    for(int i = 0; i < 16; i++)
    {
        clcd_putch(0xff, LINE1(i));
        __delay_ms(100);
    }
    
    clcd_print("Powering On", LINE2(2));
    clcd_print("Microwave Oven", LINE3(1));
    
    for(int i = 0; i < 16; i++)
    {
        clcd_putch(0xff, LINE4(i));
        __delay_ms(100);
    }
    
    clear_screen();
}

void menu_screen(void)
{
    clcd_print("1. Micro", LINE1(0));
    clcd_print("2. Grill", LINE2(0));
    clcd_print("3. Convection", LINE3(0));
    clcd_print("4. Start", LINE4(0));
}

unsigned char micro(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_MICRO)
    {
        timer_flag = 0;
        key = ALL_RELEASED;
    }
    
    if(timer_flag == 0)
    {
        set_time(key);
    }
    else if(timer_flag == 1)
    {
        timer(key);
    }
    else if(timer_flag == 2)
    {
        clear_screen();
        TMR2ON = 0;
        RC1 = 1;
        clcd_print("TIME UP!!!", LINE2(3));
        clcd_print("Enjoy your meal", LINE3(0));
        __delay_ms(2000);
        RC1 = 0;
        return RETURN_BACK;
    }
    else if(timer_flag == 3)
    {
        TMR2ON = 0;
        min = 0;
        sec = 0;
        return RETURN_BACK;
    }
}

unsigned char grill(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_GRILL)
    {
        timer_flag = 0;
        key = ALL_RELEASED;
    }
    
    if(timer_flag == 0)
    {
        set_time(key);
    }
    else if(timer_flag == 1)
    {
        timer(key);
    }
    else if(timer_flag == 2)
    {
        clear_screen();
        TMR2ON = 0;  
        RC1 = 1;
        clcd_print("TIME UP!!!", LINE2(3));
        clcd_print("Enjoy your meal", LINE3(0));
        __delay_ms(2000);
        RC1 = 0;
        return RETURN_BACK;
    }
    else if(timer_flag == 3)
    {
        TMR2ON = 0;
        min = 0;
        sec = 0;
        return RETURN_BACK;
    }
}

unsigned char convection(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_CONVEC)
    {
        timer_flag = 4;
        key = ALL_RELEASED;
    }
    
    if(timer_flag == 4)
    {
        pre_heat(key);
    }
    else if(timer_flag == 0)
    {
        set_time(key);
    }
    else if(timer_flag == 1)
    {
        timer(key);
    }
    else if(timer_flag == 2)
    {
        clear_screen();
        TMR2ON = 0;
        RC1 = 1;
        clcd_print("TIME UP!!!", LINE2(3));
        clcd_print("Enjoy your meal", LINE3(0));
        __delay_ms(2000);
        RC1 = 0;
        return RETURN_BACK;
    }
    else if(timer_flag == 3)
    {
        TMR2ON = 0;
        min = 0;
        sec = 0;
        return RETURN_BACK;
    }
}

unsigned char start(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_START)
    {
        sec = 30;
        TMR2ON = 1;
        timer_flag = 1;
        key = ALL_RELEASED;
    }
    
    if(timer_flag == 1)
    {
        timer(key);
    }
    else if(timer_flag == 2)
    {
        RC1 = 1;
        TMR2ON = 0;
        clcd_print("TIME UP!!!", LINE2(3));
        clcd_print("Enjoy your meal", LINE3(0));
        __delay_ms(2000);
        RC1 = 0;
        return RETURN_BACK;
    }
    else if(timer_flag == 3)
    {
        TMR2ON = 0;
        min = 0;
        sec = 0;
        return RETURN_BACK;
    }
}
