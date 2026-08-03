#include "main.h"

extern char time[9];

char dummy_time[9];

#define HOUR_FIELD 0
#define MIN_FIELD  1
#define SEC_FIELD  2

void load_dummy_time(void)
{
    unsigned char i;

    for(i = 0; i < 9; i++)
    {
        dummy_time[i] = time[i];
    }
}

void increment_time(unsigned char field)
{
    unsigned char value;

    switch(field)
    {
        case HOUR_FIELD:

            value = ((dummy_time[0] - '0') * 10) + (dummy_time[1] - '0');
            value = (value + 1) % 24;

            dummy_time[0] = (value / 10) + '0';
            dummy_time[1] = (value % 10) + '0';
            break;

        case MIN_FIELD:

            value = ((dummy_time[3] - '0') * 10) + (dummy_time[4] - '0');
            value = (value + 1) % 60;

            dummy_time[3] = (value / 10) + '0';
            dummy_time[4] = (value % 10) + '0';
            break;

        case SEC_FIELD:

            value = ((dummy_time[6] - '0') * 10) + (dummy_time[7] - '0');
            value = (value + 1) % 60;

            dummy_time[6] = (value / 10) + '0';
            dummy_time[7] = (value % 10) + '0';
            break;
    }
}

void decrement_time(unsigned char field)
{
    unsigned char value;

    switch(field)
    {
        case HOUR_FIELD:

            value = ((dummy_time[0] - '0') * 10) + (dummy_time[1] - '0');

            if(value == 0)
                value = 23;
            else
                value--;

            dummy_time[0] = (value / 10) + '0';
            dummy_time[1] = (value % 10) + '0';
            break;

        case MIN_FIELD:

            value = ((dummy_time[3] - '0') * 10) + (dummy_time[4] - '0');

            if(value == 0)
                value = 59;
            else
                value--;

            dummy_time[3] = (value / 10) + '0';
            dummy_time[4] = (value % 10) + '0';
            break;

        case SEC_FIELD:

            value = ((dummy_time[6] - '0') * 10) + (dummy_time[7] - '0');

            if(value == 0)
                value = 59;
            else
                value--;

            dummy_time[6] = (value / 10) + '0';
            dummy_time[7] = (value % 10) + '0';
            break;
    }
}

void save_time(void)
{
    write_ds1307(HOUR_ADDR,
            (((dummy_time[0] - '0') << 4) |
             (dummy_time[1] - '0')));

    write_ds1307(MIN_ADDR,
            (((dummy_time[3] - '0') << 4) |
             (dummy_time[4] - '0')));

    write_ds1307(SEC_ADDR,
            ((((dummy_time[6] - '0') << 4) |
             (dummy_time[7] - '0')) & 0x7F));
}

unsigned char set_time(unsigned char key)
{
    static unsigned char field = HOUR_FIELD;
    static unsigned int blink = 0;

    blink++;

    if(blink >= 1000)
        blink = 0;

    clcd_print("SET TIME", LINE1(3));
    clcd_print(dummy_time, LINE2(0));

    if(blink < 500)
    {
        switch(field)
        {
            case HOUR_FIELD:
                clcd_putch(' ', LINE2(0));
                clcd_putch(' ', LINE2(1));
                break;

            case MIN_FIELD:
                clcd_putch(' ', LINE2(3));
                clcd_putch(' ', LINE2(4));
                break;

            case SEC_FIELD:
                clcd_putch(' ', LINE2(6));
                clcd_putch(' ', LINE2(7));
                break;
        }
    }

    switch(key)
    {
        case RIGHT_KEY:

            field++;

            if(field > SEC_FIELD)
                field = HOUR_FIELD;

            blink = 0;
            break;

        case UP_KEY:

            increment_time(field);
            blink = 0;
            break;

        case DOWN_KEY:

            decrement_time(field);
            blink = 0;
            break;

        case ENTER_KEY:

            save_time();

            field = HOUR_FIELD;

            clcd_clear();
            clcd_print("TIME UPDATED", LINE1(2));

            __delay_ms(1000);

            return 1;

        case BACK_KEY:

            field = HOUR_FIELD;

            clcd_clear();

            return 2;
    }

    return 0;
}