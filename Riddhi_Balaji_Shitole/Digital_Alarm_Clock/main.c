#include "main.h"

extern unsigned int seconds;
extern unsigned char screen_flag;

unsigned char mode;

void buzzer_on(void)
{
    BUZZER = 1;
}

void buzzer_off(void)
{
    BUZZER = 0;
}
void dashboard(unsigned char key)
{
    get_time(clock_reg);
    display_time(clock_reg);

    get_date();

    clcd_print("TIME", LINE1(0));
    clcd_print(time, LINE1(5));

    clcd_print("DATE", LINE2(0));
    clcd_print(date, LINE2(5));

    if(alarm_check())
        buzzer_on();
    else
        buzzer_off();

    if(key == ENTER_KEY)
    {
        mode = MODE_MENU;
        clcd_clear();
    }
}

void init_fun(void)
{
    init_mkp();
    init_clcd();
    init_i2c(100000);
    init_ds1307();
    init_timer();
    load_all_alarms();

    mode = MODE_DASHBOARD;
}

void main(void)
{
    unsigned char key;
    unsigned char ret;

    init_fun();

    while(1)
    {
        key = read_mkp(EDGE);

        if(mode != MODE_DASHBOARD)
        {
            if(key != ALL_RELEASED)
            {
                seconds = 0;
                screen_flag = 0;
            }

            if(screen_flag)
            {
                clcd_clear();
                seconds = 0;
                screen_flag = 0;
                mode = MODE_DASHBOARD;
            }
        }

        switch(mode)
        {
            case MODE_DASHBOARD:

                dashboard(key);

                if(key == ENTER_KEY)
                {
                    clcd_clear();
                    mode = MODE_MENU;
                }
                break;

            case MODE_MENU:

                ret = menu(key);

                switch(ret)
                {
                    case SET_TIME_OPTION:
                        clcd_clear();
                        load_dummy_time();
                        mode = MODE_SET_TIME;
                        break;

                    case SET_DATE_OPTION:
                        clcd_clear();
                        load_dummy_date();
                        mode = MODE_SET_DATE;
                        break;

                    case ALARM_MENU_OPTION:
                        clcd_clear();
                        mode = MODE_ALARM_MENU;
                        break;

                    case BACK_OPTION:
                        clcd_clear();
                        mode = MODE_DASHBOARD;
                        break;
                }
                break;

            case MODE_SET_TIME:

                ret = set_time(key);

                if(ret == MENU_SUCCESS || ret == MENU_BACK)
                {
                    clcd_clear();
                    mode = MODE_MENU;
                }
                break;

            case MODE_SET_DATE:

                ret = set_date(key);

                if(ret == MENU_SUCCESS || ret == MENU_BACK)
                {
                    clcd_clear();
                    mode = MODE_MENU;
                }
                break;

            case MODE_ALARM_MENU:

                ret = alarm_menu(key);

                switch(ret)
                {
                    case SET_ALARM_OPTION:
                        clcd_clear();
                        mode = MODE_SET_ALARM;
                        break;

                    case VIEW_ALARM_OPTION:
                        clcd_clear();
                        mode = MODE_VIEW_ALARM;
                        break;

                    case DELETE_ALARM_OPTION:
                        clcd_clear();
                        mode = MODE_DELETE_ALARM;
                        break;

                    case ENABLE_DISABLE_OPTION:
                        clcd_clear();
                        mode = MODE_ENABLE_DISABLE;
                        break;

                    case BACK_OPTION:
                        clcd_clear();
                        mode = MODE_MENU;
                        break;
                }
                break;

            case MODE_SET_ALARM:

                ret = set_alarm(key);

                if(ret == MENU_SUCCESS || ret == MENU_BACK)
                {
                    clcd_clear();
                    mode = MODE_ALARM_MENU;
                }
                break;

            case MODE_VIEW_ALARM:

                ret = view_alarm(key);

                if(ret == MENU_BACK)
                {
                    clcd_clear();
                    mode = MODE_ALARM_MENU;
                }
                break;

            case MODE_DELETE_ALARM:

                ret = delete_alarm(key);

                if(ret == MENU_BACK)
                {
                    clcd_clear();
                    mode = MODE_ALARM_MENU;
                }
                break;

            case MODE_ENABLE_DISABLE:

                ret = enable_disable_alarm(key);

                if(ret == MENU_BACK)
                {
                    clcd_clear();
                    mode = MODE_ALARM_MENU;
                }
                break;
        }
    }
}