#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "buzzer.h"
#include "alarm.h"

static Alarm alarm_list[MAX_ALARMS];
static uint8_t alarm_count = 0;
static uint8_t last_date = 255, last_hour = 255, last_minute = 255;

void alarm_init(void)
{
    uint8_t i;
    alarm_count = 0;
    for (i = 0; i < MAX_ALARMS; i++)
        alarm_list[i].enabled = 0;
}

uint8_t alarm_get_count(void)
{
    return alarm_count;
}

static void alarm_editor(uint8_t index, bool add)
{
    Alarm a;
    uint8_t field = 0;
    char key;

    if (add)
    {
        a.hour = 6; a.minute = 0; a.weekday = 1;
        a.type = 1; a.enabled = 1;
    }
    else a = alarm_list[index];

    while (1)
    {
        clcd_clear();
        clcd_goto(0,0); clcd_print(add ? "ADD ALARM" : "EDIT ALARM");
        clcd_goto(1,0); clcd_print2(a.hour); clcd_putc(':');
        clcd_print2(a.minute); clcd_putc(' ');
        clcd_print(a.type == 0 ? "ONCE" : (a.type == 1 ? "DAILY" : "WEEK"));
        key = keypad_get_event();

        if (key == '2' || key == 'U' || key == '8' || key == 'V')
        {
            uint8_t up = (key == '2' || key == 'U');
            if (field == 0) a.hour = (uint8_t)((a.hour + (up ? 1 : 23)) % 24);
            else if (field == 1) a.minute = (uint8_t)((a.minute + (up ? 1 : 59)) % 60);
            else if (field == 2) a.type = (uint8_t)((a.type + (up ? 1 : 2)) % 3);
            else a.weekday = (uint8_t)(((a.weekday - 1 + (up ? 1 : 6)) % 7) + 1);
        }
        else if (key == '4') field = (field == 0) ? 3 : (uint8_t)(field - 1);
        else if (key == '6') field = (uint8_t)((field + 1) % 4);
        else if (key == '5' || key == 'S')
        {
            a.enabled = 1;
            if (add && alarm_count < MAX_ALARMS) alarm_list[alarm_count++] = a;
            else if (!add) alarm_list[index] = a;
            return;
        }
        else if (key == 'D' || key == 'd') return;
    }
}

void alarm_view_menu(void)
{
    uint8_t selected = 0;
    char key;

    while (1)
    {
        clcd_clear();
        clcd_goto(0,0); clcd_print("ALARMS ");
        clcd_putc((char)('0' + alarm_count)); clcd_print("/5");
        if (alarm_count == 0)
        {
            clcd_goto(1,0); clcd_print("5=ADD D=BACK");
        }
        else
        {
            clcd_goto(1,0);
            clcd_print2(alarm_list[selected].hour);
            clcd_putc(':'); clcd_print2(alarm_list[selected].minute);
            clcd_putc(' ');
            clcd_print(alarm_list[selected].type == 0 ? "ONCE" :
                       (alarm_list[selected].type == 1 ? "DAILY" : "WEEK"));
        }

        key = keypad_get_event();
        if (key == '2' && alarm_count) selected = (uint8_t)((selected + 1) % alarm_count);
        else if (key == '8' && alarm_count)
            selected = (selected == 0) ? (uint8_t)(alarm_count - 1) : (uint8_t)(selected - 1);
        else if ((key == '5' || key == 'S') && alarm_count < MAX_ALARMS)
            alarm_editor(alarm_count, true);
        else if (key == '6' && alarm_count) alarm_editor(selected, false);
        else if (key == '4' && alarm_count)
        {
            uint8_t i;
            for (i = selected; i + 1 < alarm_count; i++) alarm_list[i] = alarm_list[i + 1];
            alarm_count--;
            if (selected >= alarm_count && alarm_count) selected = (uint8_t)(alarm_count - 1);
        }
        else if (key == 'D' || key == 'd' || key == 'U') return;
    }
}

void alarm_check(const RTC_Time *now)
{
    uint8_t i;
    for (i = 0; i < alarm_count; i++)
    {
        Alarm *a = &alarm_list[i];
        if (a->enabled && a->hour == now->hour && a->minute == now->min &&
            now->sec == 0 &&
            !(last_date == now->date && last_hour == now->hour && last_minute == now->min))
        {
            if (a->type == 0 || a->type == 1 ||
                (a->type == 2 && a->weekday == now->weekday))
            {
                uint16_t count;
                clcd_clear(); clcd_goto(0,0); clcd_print("  ALARM ACTIVE");
                clcd_goto(1,0); clcd_print("5 OR D TO STOP");
                buzzer_on();
                for (count = 0; count < 300; count++)
                {
                    char key = keypad_scan();
                    if (key == '5' || key == 'D') break;
                    __delay_ms(10);
                }
                buzzer_off();
                if (a->type == 0) a->enabled = 0;
                last_date = now->date; last_hour = now->hour; last_minute = now->min;
                return;
            }
        }
    }
}
