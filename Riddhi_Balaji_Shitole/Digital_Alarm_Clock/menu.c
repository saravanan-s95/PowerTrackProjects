#include "main.h"

unsigned char menu(unsigned char key)
{
    static unsigned char index = 0;

    const char *menu_list[] =
    {
        "Set Time      ",
        "Set Date      ",
        "Alarm Menu    ",
        "Back          "
    };

    if(key == UP_KEY)
    {
        if(index == 0)
            index = 3;
        else
            index--;
    }
    else if(key == DOWN_KEY)
    {
        if(index == 3)
            index = 0;
        else
            index++;
    }
    else if(key == ENTER_KEY)
    {
        switch(index)
        {
            case 0:
                return SET_TIME_OPTION;

            case 1:
                return SET_DATE_OPTION;

            case 2:
                return ALARM_MENU_OPTION;

            case 3:
                return BACK_OPTION;
        }
    }
    else if(key == BACK_KEY)
    {
        return BACK_OPTION;
    }

    clcd_print(" MAIN MENU ", LINE1(3));

    clcd_putch('>', LINE2(0));
    clcd_print(menu_list[index], LINE2(1));

    return MENU_WAIT;
}