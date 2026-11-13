#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "matrix_keypad.h"
#include "alarm.h"
#include "menu.h"

static void set_time_date(void)
{
    uint8_t field = 0;
    char key;

    while (1)
    {
        RTC_Time now;
        ds1307_read(&now);
        clcd_clear();
        clcd_goto(0,0); clcd_print("SET TIME/DATE");
        clcd_goto(1,0);
        clcd_print2(now.hour); clcd_putc(':'); clcd_print2(now.min);
        clcd_putc(':'); clcd_print2(now.sec); clcd_putc(' ');
        clcd_print2(now.date); clcd_putc('/'); clcd_print2(now.month);
        key = keypad_get_event();

        if (key == '2' || key == 'U' || key == '8' || key == 'V')
        {
            uint8_t *value, limit;
            switch (field)
            {
                case 0: value=&now.hour; limit=24; break;
                case 1: value=&now.min; limit=60; break;
                case 2: value=&now.sec; limit=60; break;
                case 3: value=&now.date; limit=32; break;
                case 4: value=&now.month; limit=13; break;
                default: value=&now.year; limit=100; break;
            }
            if (key == '2' || key == 'U')
            {
                (*value)++;
                if (*value >= limit) *value = (field == 3 || field == 4) ? 1 : 0;
            }
            else
            {
                if (*value == 0) *value = (uint8_t)(limit - 1);
                else (*value)--;
            }
            ds1307_write(&now);
        }
        else if (key == '4') field = (field == 0) ? 5 : (uint8_t)(field - 1);
        else if (key == '6') field = (uint8_t)((field + 1) % 6);
        else if (key == 'S' || key == '5' || key == 'd' || key == 'D') return;
    }
}

void menu_config(void)
{
    uint8_t selected = 0;
    char key;
    while (1)
    {
        clcd_clear();
        clcd_goto(0,0); clcd_print("CONFIG MENU");
        clcd_goto(1,0);
        clcd_print(selected == 0 ? ">TIME/DATE" : " TIME/DATE");
        clcd_goto(1,10);
        clcd_print(selected == 1 ? ">ALARM" : " ALARM");
        key = keypad_get_event();

        if (key == '2' || key == '8') selected ^= 1;
        else if (key == '5' || key == 'S' || key == 'U')
        {
            if (selected == 0) set_time_date();
            else alarm_view_menu();
        }
        else if (key == 'D' || key == 'd' || key == 'V') return;
    }
}
