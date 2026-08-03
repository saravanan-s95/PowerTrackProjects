#include "main.h"

unsigned char set_alarm(unsigned char key)
{
    static unsigned char field = 0;
    static unsigned char hour = 7;
    static unsigned char min = 0;
    static unsigned char sec = 0;
    char alarm_time[9];

    alarm_time[0] = (hour / 10) + '0';
    alarm_time[1] = (hour % 10) + '0';
    alarm_time[2] = ':';
    alarm_time[3] = (min / 10) + '0';
    alarm_time[4] = (min % 10) + '0';
    alarm_time[5] = ':';
    alarm_time[6] = (sec / 10) + '0';
    alarm_time[7] = (sec % 10) + '0';
    alarm_time[8] = '\0';

    clcd_print("SET ALARM", LINE1(3));
    clcd_print(alarm_time, LINE2(0));

    if(alarms[0].enable)
        clcd_print("ON ", LINE2(10));
    else
        clcd_print("OFF", LINE2(10));

    switch(key)
    {
        case UP_KEY:

            if(field == 0)
            {
                hour = (hour + 1) % 24;
            }
            else if(field == 1)
            {
                min = (min + 1) % 60;
            }
            else if(field == 2)
            {
                sec = (sec + 1) % 60;
            }
            else
            {
                alarms[0].enable ^= 1;
            }
            break;

        case DOWN_KEY:

            if(field == 0)
            {
                hour = (hour == 0) ? 23 : hour - 1;
            }
            else if(field == 1)
            {
                min = (min == 0) ? 59 : min - 1;
            }
            else if(field == 2)
            {
                sec = (sec == 0) ? 59 : sec - 1;
            }
            else
            {
                alarms[0].enable ^= 1;
            }
            break;

        case RIGHT_KEY:

            field++;

            if(field > 3)
                field = 0;

            break;

        case ENTER_KEY:

            alarms[0].hour = hour;
            alarms[0].min = min;
            alarms[0].sec = sec;
            alarms[0].enable = 1;

            field = 0;
            return MENU_SUCCESS;

        case BACK_KEY:

            field = 0;
            return MENU_BACK;
    }

    return MENU_WAIT;
}

unsigned char view_alarm(unsigned char key)
{
    static unsigned char index = 0;
    char alarm_time[6];

    alarm_time[0] = (alarms[index].hour / 10) + '0';
    alarm_time[1] = (alarms[index].hour % 10) + '0';
    alarm_time[2] = ':';
    alarm_time[3] = (alarms[index].min / 10) + '0';
    alarm_time[4] = (alarms[index].min % 10) + '0';
    alarm_time[5] = '\0';

    clcd_print("VIEW ALARM", LINE1(2));
    clcd_print("Alarm ", LINE2(0));
    clcd_putch(index + '1', LINE2(6));
    clcd_print(alarm_time, LINE2(8));

    if(alarms[index].enable)
        clcd_print(" ON", LINE2(14));
    else
        clcd_print("OFF", LINE2(13));

    switch(key)
    {
        case RIGHT_KEY:
            index++;
            if(index >= 5)
                index = 0;
            break;

        case LEFT_KEY:
            if(index == 0)
                index = 4;
            else
                index--;
            break;

        case BACK_KEY:
            index = 0;
            return MENU_BACK;
    }

    return MENU_WAIT;
}

unsigned char delete_alarm(unsigned char key)
{
    static unsigned char index = 0;
    char alarm_time[6];

    alarm_time[0] = (alarms[index].hour / 10) + '0';
    alarm_time[1] = (alarms[index].hour % 10) + '0';
    alarm_time[2] = ':';
    alarm_time[3] = (alarms[index].min / 10) + '0';
    alarm_time[4] = (alarms[index].min % 10) + '0';
    alarm_time[5] = '\0';

    clcd_print("DELETE ALARM", LINE1(1));
    clcd_print(alarm_time, LINE2(0));

    if(alarms[index].enable)
        clcd_print(" ON", LINE2(7));
    else
        clcd_print(" OFF", LINE2(7));

    switch(key)
    {
        case RIGHT_KEY:

            index++;

            if(index >= 5)
                index = 0;

            break;

        case LEFT_KEY:

            if(index == 0)
                index = 4;
            else
                index--;

            break;

        case ENTER_KEY:

            alarms[index].hour = 0;
            alarms[index].min = 0;
            alarms[index].enable = 0;

            clcd_clear();
            clcd_print("ALARM DELETED", LINE1(1));
            __delay_ms(1000);

            return MENU_BACK;

        case BACK_KEY:

            index = 0;
            return MENU_BACK;
    }

    return MENU_WAIT;
}

unsigned char enable_disable_alarm(unsigned char key)
{
    static unsigned char index = 0;
    char alarm_time[6];

    alarm_time[0] = (alarms[index].hour / 10) + '0';
    alarm_time[1] = (alarms[index].hour % 10) + '0';
    alarm_time[2] = ':';
    alarm_time[3] = (alarms[index].min / 10) + '0';
    alarm_time[4] = (alarms[index].min % 10) + '0';
    alarm_time[5] = '\0';

    clcd_print("ALARM STATUS", LINE1(2));
    clcd_print(alarm_time, LINE2(0));

    if(alarms[index].enable)
        clcd_print(" ON", LINE2(7));
    else
        clcd_print(" OFF", LINE2(7));

    switch(key)
    {
        case RIGHT_KEY:

            index++;

            if(index >= 5)
                index = 0;

            break;

        case LEFT_KEY:

            if(index == 0)
                index = 4;
            else
                index--;

            break;

        case ENTER_KEY:

            alarms[index].enable ^= 1;

            break;

        case BACK_KEY:

            index = 0;
            return MENU_BACK;
    }

    return MENU_WAIT;
}

unsigned char alarm_check(void)
{
    unsigned char i;
    unsigned char hour, min, sec;

    hour = (time[0] - '0') * 10 + (time[1] - '0');
    min  = (time[3] - '0') * 10 + (time[4] - '0');
    sec  = (time[6] - '0') * 10 + (time[7] - '0');

    for(i = 0; i < MAX_ALARMS; i++)
    {
        if(alarms[i].enable)
        {
            if(alarms[i].hour == hour &&
               alarms[i].min  == min &&
               alarms[i].sec  == sec)
            {
                return 1;
            }
        }
    }

    return 0;
}