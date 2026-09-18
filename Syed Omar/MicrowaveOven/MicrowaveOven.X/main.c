#include "main.h"

/* ---------- Globals ---------- */
unsigned char sec = 0, min = 0;     /* shared mm:ss - cook timer AND preheat countdown */
unsigned char temp = 0;             /* convection target temperature */
unsigned char screen_flag = POWER_SCREEN;

/* Door-interlock bookkeeping */
static unsigned char door_active  = 0;
static unsigned char saved_fan    = 0;
static unsigned char saved_tmr    = 0;
static unsigned char saved_screen = MENU_SCREEN;
static unsigned char force_redraw = 0; /* set by door_check() on close */

static void init_config(void)
{
    init_clcd();
    init_matrix_keypad();
    init_timer2();

    FAN_DDR = 0;      /* RC2 output */
    FAN = OFF;

    BUZZER_DDR = 0;   /* RC1 output */
    BUZZER = OFF;

    DOOR_DDR = 1;     /* RB3 input  */

    PEIE = 1;
    GIE  = 1;
}

void door_check(void)
{
    if (DOOR == OPEN)
    {
        if (!door_active)
        {
            door_active  = 1;
            saved_fan    = FAN;
            saved_tmr    = TMR2ON;
            saved_screen = screen_flag;

            FAN    = OFF;
            TMR2ON = 0;
            BUZZER = ON;

            clear_screen();
            clcd_print(" DOOR IS OPEN   ", LINE2(0));
            clcd_print(" Please close   ", LINE3(0));
        }
    }
    else if (door_active)
    {
        door_active = 0;
        BUZZER = OFF;
        if (saved_screen == DISPLAY_TIME || saved_screen == CONV_PREHEAT)
        {
            FAN    = saved_fan;
            TMR2ON = saved_tmr;
        }

        clear_screen();
        force_redraw = 1;   
    }
}

void main(void)
{
    unsigned char key;
    unsigned char reset_flag;
    unsigned char prev_screen = 0xFF; 
    init_config();

    while (1)
    {
        door_check();

        if (door_active)
        {
            continue;
        }

        key = read_matrix_keypad(STATE);

        
        if (screen_flag == MENU_SCREEN)
        {
            if (key == 1)
            {
                screen_flag = MICRO_SET_TIME;
            }
            else if (key == 2)
            {
                screen_flag = GRILL_SET_TIME;
            }
            else if (key == 3)
            {
                screen_flag = CONV_SET_TEMP;
            }
            else if (key == 4) /* quick-start shortcut: 30s */
            {
                min = 0;
                sec = 30;
                FAN = ON;
                TMR2ON = 1;
                screen_flag = DISPLAY_TIME;
            }
        }
        else if (screen_flag == DISPLAY_TIME)
        {
            if (key == 4) /* add 30 seconds */
            {
                sec += 30;
                if (sec >= 60)
                {
                    min += sec / 60;
                    sec %= 60;
                }
                FAN = ON;
                TMR2ON = 1;
            }
            else if (key == 5) /* pause */
            {
                screen_flag = PAUSE_SCREEN;
            }
            else if (key == 6) /* stop */
            {
                screen_flag = STOP_SCREEN;
            }
        }
        else if (screen_flag == PAUSE_SCREEN)
        {
            if (key == 4) 
            {
                FAN = ON;
                TMR2ON = 1;
                screen_flag = DISPLAY_TIME;
            }
            else if (key == 6) 
            {
                screen_flag = STOP_SCREEN;
            }
        }

        if (screen_flag != prev_screen)
        {
            reset_flag = RESET_MODE;
            key = ALL_RELEASED; 
        }
        else if (force_redraw)
        {
            reset_flag = REDRAW_ONLY;
        }
        else
        {
            reset_flag = RESET_NOTHING;
        }
        force_redraw = 0;
        prev_screen  = screen_flag;

        switch (screen_flag)
        {
            case POWER_SCREEN:
                power_on_screen();
                clear_screen();
                screen_flag = MENU_SCREEN;
                break;

            case MENU_SCREEN:
                menu_display_screen(reset_flag);
                break;

            case MICRO_SET_TIME:
            case GRILL_SET_TIME:
            case CONV_SET_TIME:
                set_time(key, reset_flag);
                break;

            case CONV_SET_TEMP:
                set_temp(key, reset_flag);
                break;

            case CONV_PREHEAT:
                preheat_screen(reset_flag);
                break;

            case DISPLAY_TIME:
                time_display_screen(reset_flag);
                break;

            case PAUSE_SCREEN:
                if (reset_flag == RESET_MODE || reset_flag == REDRAW_ONLY)
                {
                    clear_screen();
                    clcd_print("     PAUSED     ", LINE2(0));
                    clcd_print("4:Resume 6:Stop ", LINE3(0));
                }
                FAN = OFF;
                TMR2ON = 0;
                break;

            case STOP_SCREEN:
                FAN = OFF;
                TMR2ON = 0;
                clear_screen();
                screen_flag = MENU_SCREEN;
                break;
        }
    }
}