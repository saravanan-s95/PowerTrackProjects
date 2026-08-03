#include "main.h"
unsigned char alarm_menu(unsigned char key)
{
    static unsigned char index = 0;

    const char *alarm_menu_list[] =
    {
        "Set Alarm      ",
        "View Alarm     ",
        "Delete Alarm   ",
        "Alarm ON/OFF   ",
        "Back           "
    };

    if(key == UP_KEY)
    {
        if(index == 0)
            index = 4;
        else
            index--;
    }
    else if(key == DOWN_KEY)
    {
        if(index == 4)
            index = 0;
        else
            index++;
    }
    else if(key == ENTER_KEY)
    {
        switch(index)
        {
            case 0:
                return SET_ALARM_OPTION;

            case 1:
                return VIEW_ALARM_OPTION;

            case 2:
                return DELETE_ALARM_OPTION;

            case 3:
                return ENABLE_DISABLE_OPTION;

            case 4:
                return BACK_OPTION;
        }
    }
    else if(key == BACK_KEY)
    {
        return BACK_OPTION;
    }

    clcd_print(" ALARM MENU ", LINE1(2));

    clcd_putch((index == 0) ? '>' : ' ', LINE2(0));
    clcd_print(alarm_menu_list[index], LINE2(1));

    return MENU_WAIT;
}