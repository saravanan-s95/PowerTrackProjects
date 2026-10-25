#include <xc.h>
#include "ds1307.h"
#include "external_eeprom.h"
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "set_time.h"

unsigned char clock_reg[3];
unsigned char time[9];
extern char blink;

void get_time(void)
{
    /* Read hour, minute and second and store in clock array */
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

    /* Shift from BCD to decimal and store in time array */
	time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
    time[1] = '0' + (clock_reg[0] & 0x0F);
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void set_time()
{
    /* Setting the CH bit of the RTC to Stop the Clock */
    unsigned char sec = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR,sec | 0x80);
    
    /* Shifting the fields hour or minute or second */
    unsigned char field = 0;
    int field_delay = 0;
    int delay = 0;
    
    /* Get current time from RTC */
    get_time();
    /* From time array extract and store hour, minute and second seperately */
    unsigned char hour = (time[0] - '0')*10 + (time[1] - '0');
    unsigned char min = (time[3] - '0')*10 + (time[4] - '0');
    unsigned char seconds = (time[6] - '0')*10 + (time[7] - '0');
    
    CLEAR_DISP_SCREEN;
    clcd_print("HH:MM:ss",LINE1(3));
    
    while(1)
    {
        /* Display the value in clcd */
        clcd_putch(hour%10 + '0',LINE2(4));
        clcd_putch((hour/10)%10 + '0',LINE2(3));
        clcd_putch(':',LINE2(5));
        clcd_putch(min%10 + '0',LINE2(7));
        clcd_putch((min/10)%10 + '0',LINE2(6));
        clcd_putch(':',LINE2(8));
        clcd_putch(seconds%10 + '0',LINE2(10));
        clcd_putch((seconds/10)%10 + '0',LINE2(9));
        
        /* Blink chosen field */
        if(blink)
        {
            /* Second */
            if(field == 0)
            {
                clcd_print("  ",LINE2(9));
            }
            /* Minute */
            else if(field == 1)
            {
                clcd_print("  ",LINE2(6));
            }
            /* Hour */
            else
            {
                clcd_print("  ",LINE2(3));
            }
        }
        
        /* Read level triggered matrix keypad */
        unsigned char key = read_matrix_keypad(LEVEL);
        
        /* Switch 11 */
        if(key == SWITCH11)
        {
            /* Long press */
            if(delay++ >= LONG_PRESS)
            {
                /* Write the edited time to RTC by shifting from decimal to BCD and start RTC and return to menu */
                write_ds1307(HOUR_ADDR,(hour/10) << 4 | (hour%10));
                write_ds1307(MIN_ADDR,(min/10) << 4 | (min%10));
                write_ds1307(SEC_ADDR,((seconds/10) << 4 | (seconds%10)) & 0x7F);
                CLEAR_DISP_SCREEN;
                return;
            }
        }
        
        /* Short press */
        else if(delay > 0 && delay < LONG_PRESS)
        {
            /* Second */
            if(field == 0)
            {
                /* Decrement */
                if(seconds-- == 0)
                {
                    seconds = 59;
                }
            }
            /* Minute */
            else if(field == 1)
            {
                /* Decrement */
                if(min-- == 0)
                {
                    min = 59;
                }
            }
            /* Hour */
            else
            {
                /* Decrement */
                if(hour-- == 0)
                {
                    hour = 23;
                }
            }
            delay = 0;
        }
        
        else
        {
            delay = 0;
        }
        
        /* Switch 12 */
        if(key == SWITCH12)
        {
            /* Long press */
            if(field_delay++ >= LONG_PRESS)
            {
                /* Switch field in a circular method */
                if(field++ == 2)
                {
                    field = 0;
                }
            }
        }
        
        /* Short press */
        else if(field_delay > 0 && field_delay < LONG_PRESS)
        {
            /* Second */
            if(field == 0)
            {
                /* Increment */
                if(seconds++ == 59)
                {
                    seconds = 0;
                }
            }
            /* Minute */
            else if(field == 1)
            {
                /* Increment */
                if(min++ == 59)
                {
                    min = 0;
                }
            }
            /* Hour */
            else
            {
                /* Increment */
                if(hour++ == 23)
                {
                    hour = 0;
                }
            }
            field_delay = 0;
        }
        
        else
        {
            field_delay = 0;
        }
    }
}