#include "main.h"

char date[11];

char dummy_date[11];

#define DATE_FIELD   0
#define MONTH_FIELD  1
#define YEAR_FIELD   2

void load_dummy_date(void)
{
    unsigned char i;

    for(i = 0; i < 11; i++)
    {
        dummy_date[i] = date[i];
    }
}

void increment_date(unsigned char field)
{
    unsigned int value;

    switch(field)
    {
        case DATE_FIELD:

            value = ((dummy_date[0] - '0') * 10) + (dummy_date[1] - '0');
            value = (value == 31) ? 1 : value + 1;

            dummy_date[0] = (value / 10) + '0';
            dummy_date[1] = (value % 10) + '0';
            break;

        case MONTH_FIELD:

            value = ((dummy_date[3] - '0') * 10) + (dummy_date[4] - '0');
            value = (value == 12) ? 1 : value + 1;

            dummy_date[3] = (value / 10) + '0';
            dummy_date[4] = (value % 10) + '0';
            break;

        case YEAR_FIELD:

            value = ((dummy_date[6] - '0') * 1000) +
                    ((dummy_date[7] - '0') * 100) +
                    ((dummy_date[8] - '0') * 10) +
                    (dummy_date[9] - '0');

            if(value < 2099)
                value++;

            dummy_date[6] = (value / 1000) % 10 + '0';
            dummy_date[7] = (value / 100) % 10 + '0';
            dummy_date[8] = (value / 10) % 10 + '0';
            dummy_date[9] = value % 10 + '0';
            break;
    }
}

void decrement_date(unsigned char field)
{
    unsigned int value;

    switch(field)
    {
        case DATE_FIELD:

            value = ((dummy_date[0] - '0') * 10) + (dummy_date[1] - '0');
            value = (value == 1) ? 31 : value - 1;

            dummy_date[0] = (value / 10) + '0';
            dummy_date[1] = (value % 10) + '0';
            break;

        case MONTH_FIELD:

            value = ((dummy_date[3] - '0') * 10) + (dummy_date[4] - '0');
            value = (value == 1) ? 12 : value - 1;

            dummy_date[3] = (value / 10) + '0';
            dummy_date[4] = (value % 10) + '0';
            break;

        case YEAR_FIELD:

            value = ((dummy_date[6] - '0') * 1000) +
                    ((dummy_date[7] - '0') * 100) +
                    ((dummy_date[8] - '0') * 10) +
                    (dummy_date[9] - '0');

            if(value > 2000)
                value--;

            dummy_date[6] = (value / 1000) % 10 + '0';
            dummy_date[7] = (value / 100) % 10 + '0';
            dummy_date[8] = (value / 10) % 10 + '0';
            dummy_date[9] = value % 10 + '0';
            break;
    }
}

unsigned char set_date(unsigned char key)
{
    static unsigned char field = DATE_FIELD;
    static unsigned int blink;

    blink++;

    if(blink >= 1000)
        blink = 0;

    clcd_print("SET DATE", LINE1(3));
    clcd_print(dummy_date, LINE2(0));

    if(blink < 500)
    {
        switch(field)
        {
            case DATE_FIELD:
                clcd_putch(' ', LINE2(0));
                clcd_putch(' ', LINE2(1));
                break;

            case MONTH_FIELD:
                clcd_putch(' ', LINE2(3));
                clcd_putch(' ', LINE2(4));
                break;

            case YEAR_FIELD:
                clcd_putch(' ', LINE2(6));
                clcd_putch(' ', LINE2(7));
                clcd_putch(' ', LINE2(8));
                clcd_putch(' ', LINE2(9));
                break;
        }
    }

    switch(key)
    {
        case RIGHT_KEY:
            field = (field + 1) % 3;
            blink = 0;
            break;

        case UP_KEY:
            increment_date(field);
            blink = 0;
            break;

        case DOWN_KEY:
            decrement_date(field);
            blink = 0;
            break;

        case ENTER_KEY:
            field = DATE_FIELD;
            return 1;

        case BACK_KEY:
            field = DATE_FIELD;
            return 2;
    }

    return 0;
}