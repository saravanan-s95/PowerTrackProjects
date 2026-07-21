#include <xc.h>
#include "main.h"

static unsigned char time[4] = {0,0,0,0};
volatile unsigned char one_second = 0;

void clear_clcd()
{
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
    clcd_print("                ", LINE3(0));
    clcd_print("                ", LINE4(0));
}

/*POWER ON Screen*/
void power_on_screen()
{
    unsigned char cursor_count = 16;
    
    for(int i = 0; i < cursor_count; i++)
    {
        clcd_putch(0xFF , (unsigned char)LINE1(i));
        clcd_putch(0xFF , (unsigned char)LINE4(i));
        __delay_ms(25);
    } 
    clcd_print("POWERING ON", LINE2(1));
    clcd_print("MICROWAVE OVEN", LINE3(1));
    
    __delay_ms(1000);
    
    current_state = MENU;
}


/*MENU SCREEN*/
void menu_screen(unsigned char key)
{
    static unsigned char once = 1;

    if(once)
    {
        clear_clcd();

        clcd_print("1. MICRO", LINE1(0));
        clcd_print("2. GRILL", LINE2(0));
        clcd_print("3. CONVECTION", LINE3(0));
        clcd_print("4. START", LINE4(0));

        once = 0;
    }

    if(key == SW1)
    {
        once = 1;
        current_state = MICRO;
    }
    else if(key == SW2)
    {
        once = 1;
        current_state = GRILL;
    }
    else if(key == SW3)
    {
        once = 1;
        current_state = CONVECTION;
    }
    else if(key == SW4)
    {
        once = 1;
        current_state = START;
    }
}

/*MICRO MODE SCREEN*/
void micro_screen()
{
    clear_clcd();
    clcd_print("POWER = 900W", LINE2(2));
    __delay_ms(500);
    
    current_state = SET_TIME;
    
}

/*GRILL SCREEN*/
void grill_screen()
{
    clear_clcd();
    current_state = SET_TIME;
    return;
}

/*CONVECTION SCREEN*/
void convection_screen(unsigned char key)
{
    static unsigned char once = 1;
    static unsigned char temp[] = {0,0,0};
    if(once)
    {
        clear_clcd();
        clcd_print("SET TEMP <*C>", LINE1(0));
        clcd_print("TEMP = ", LINE2(0));
        clcd_print("*:Clear  #:ENTER", LINE4(0));
        
        once = 0;
    }
    unsigned int temperature;

    temperature = temp[0] * 100 + temp[1] * 10 + temp[2];

    if(temperature > 180)
    {
        temp[0] = 1;
        temp[1] = 8;
        temp[2] = 0;
    }
 
    clcd_putch(temp[0] + '0', LINE2(7));
    clcd_putch(temp[1] + '0', LINE2(8));
    clcd_putch(temp[2] + '0', LINE2(9));
    
    if(key >= 0 && key <= 9)
    {
        temp[0] = temp[1];
        temp[1] = temp[2];
        temp[2] = key;
        
    }
    else if(key == STAR)
    {
        temp[2] = 0;
        temp[1] = 0;
        temp[0] = 0;
    }
    else if(key == HASH)
    {
        once = 1;
        temp[0] = 0;
        temp[1] = 0;
        temp[2] = 0;
        current_state = PREHEAT;
    }  
}

/*PREHEAT SCREEN*/
void preheat_screen(void)
{
    static unsigned char once = 1;
    if(once){
        clear_clcd();
        clcd_print("PREHEAT", LINE1(0));
        clcd_print("TEMP LEFT: ", LINE2(0));
        time[2] = 6;
        time[3] = 0;
        once = 0;
    }
    
    if(one_second)
    {
        one_second = 0;
            if(decrement_time())
            {
                once = 1;
                current_state = SET_TIME;
                return;
            }
    }
    clcd_putch(time[2] + '0', LINE2(10));
    clcd_putch(time[3] + '0', LINE2(11));
    clcd_print("SEC", LINE2(13));
}

void start_screen()
{
    time[0] = 0;
    time[1] = 0;
    time[2] = 3;
    time[3] = 0;
    current_state = RUN_MODE;
    return;
}

typedef enum
{
    SEC_FIELD,
    MIN_FIELD
} Time_field;

void set_time_screen(unsigned char key)
{
    static unsigned char once = 1;
    static Time_field field = SEC_FIELD;
    static unsigned char digits = 0;
    static unsigned long blink = 0;
    static unsigned char toggle = 0;

    if(once)
    {
        clear_clcd();

        clcd_print("SET TIME <MM:SS>", LINE1(0));
        clcd_print("TIME : ", LINE2(0));
        clcd_print("*:CLEAR  #:ENTER", LINE4(0));

        once = 0;
    }
    
    if(++blink >= 15)
    {
        blink = 0;
        toggle ^= 1;
    }

    clcd_putch(':', LINE2(9));

    if(field == MIN_FIELD && toggle)
    {
        clcd_putch(' ', (unsigned char)LINE2(7));
        clcd_putch(' ', (unsigned char)LINE2(8));
    }
    else
    {
        clcd_putch(time[0] + '0', LINE2(7));
        clcd_putch(time[1] + '0', LINE2(8));
    }

    if(field == SEC_FIELD && toggle)
    {
        clcd_putch(' ', (unsigned char)LINE2(10));
        clcd_putch(' ', (unsigned char)LINE2(11));
    }
    else
    {
        clcd_putch(time[2] + '0', LINE2(10));
        clcd_putch(time[3] + '0', LINE2(11));
    }

    if(key >= 0 && key <= 9)
    {
        if(field == SEC_FIELD)
        {
            time[2] = time[3];
            time[3] = key;

            digits++;

            if(digits == 2)
                field = MIN_FIELD;
        }
        else
        {
            time[0] = time[1];
            time[1] = key;

            digits++;

            if(digits == 4)
            {
                digits = 0;
                field = SEC_FIELD;
            }
        }
    }
    else if(key == STAR)
    {
        if(field == SEC_FIELD)
        {
            digits = 0;
            time[2] = 0;
            time[3] = 0;
        }
        else
        {
            digits = 0;
            time[0] = 0;
            time[1] = 0;
        }
    }
    else if(key == HASH)
    {
        once = 1;
        digits = 0;
        field = SEC_FIELD;
        current_state = RUN_MODE;
    }
}

void run_mode_screen(unsigned char key)
{
    static unsigned char once = 1;
    static unsigned char pause = 0;
    
    if(once)
    {
        clear_clcd(); 
        FAN = ON;
        clcd_print("TIME = ", LINE1(0));

        once = 0;
    }
    
    if(key == SW4)
    {
        if(pause){
            FAN = ON;
            pause = 0;
        }
        else
        {
            unsigned char sec = time[2] * 10 + time[3];
            unsigned char min = time[0] * 10 + time[1];

            sec += 30;

            if(sec >= 60)
            {
                sec -= 60;
                min++;
            }

            time[0] = min / 10;
            time[1] = min % 10;
            time[2] = sec / 10;
            time[3] = sec % 10;
        }
    }
    else if(key == SW5)
    {
        FAN = OFF;
        pause = 1;
    }
    else if(key == SW6)
    {
        time[0] = 0;
        time[1] = 0;
        time[2] = 0;
        time[3] = 0;
        FAN = OFF;
        once = 1;
        pause = 0;
        current_state = MENU;
        return;
    }
    
    if(one_second)
    {
        one_second = 0;
        if(pause == 0){
            if(decrement_time())
            {
                once = 1;
                current_state = TIME_UP;
                return;
            }
        }
    }

    clcd_putch(time[0] + '0', LINE1(7));
    clcd_putch(time[1] + '0', LINE1(8));
    clcd_putch(':',           LINE1(9));
    clcd_putch(time[2] + '0', LINE1(10));
    clcd_putch(time[3] + '0', LINE1(11));

    clcd_print("4.START/RESUME", LINE2(0));
    clcd_print("5.PAUSE",        LINE3(0));
    clcd_print("6.STOP",         LINE4(0));
}

unsigned char decrement_time(void)
{
    if(time[0] == 0 && time[1] == 0 && time[2] == 0 && time[3] == 0)
    {
        return 1; 
    }

    if(time[3] > 0)
    {
        time[3]--;
    }
    else
    {
        time[3] = 9;

        if(time[2] > 0)
        {
            time[2]--;
        }
        else
        {
            time[2] = 5;

            if(time[1] > 0)
            {
                time[1]--;
            }
            else
            {
                time[1] = 9;

                if(time[0] > 0)
                {
                    time[0]--;
                }
            }
        }
    }

    return 0;
}

/*TIME UP SCREEN*/
void time_up_screen()
{
    clear_clcd();
    clcd_print("TIME UP!!", LINE2(0));
    clcd_print("ENJOY YOUR MEAL", LINE3(0));
    FAN = OFF;
    BUZZER = ON;
    __delay_ms(1000);
    current_state = MENU;
    return;
}
