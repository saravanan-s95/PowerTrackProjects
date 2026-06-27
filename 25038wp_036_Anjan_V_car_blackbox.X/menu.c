#include "main.h"


char *str[] =
{
    "View Log",
    "Clear Log",
    "Download Log",
    "Set Time",
    "Change Pass"
};

char password[5] = "1100";
char data[5][17];
unsigned char event_count = 0;
unsigned char old_menu_pos = 255;


unsigned char login(unsigned char key)
{
    static unsigned char once = 1;
    static unsigned char index = 0;
    static unsigned char attempts = 3;

    static unsigned char blocked = 0;
    static unsigned char sec_left = 30;

    static unsigned long int delay = 0;

    static unsigned int blink = 0;
    static unsigned char blink_state = 0;

    static char entered[5];

    /* User Blocked after 3 failed attempts */
    if(blocked)
    {
        clcd_print("User Blocked    ", LINE1(0));
        clcd_print("Sec Left : ", LINE2(0));
        clcd_putch((sec_left / 10) + '0', LINE2(11));
        clcd_putch((sec_left % 10) + '0', LINE2(12));
        
        /* Countdown timer */
        if(delay++ >= 5)
        {
            delay = 0;
            if(sec_left > 0)
            {
                sec_left--;
            }
            /* Unblock user after timeout */
            if(sec_left == 0)
            {
                blocked = 0;
                sec_left = 30;
                attempts = 3;
                once = 1;
                index = 0;
                operation = 0;

                clcd_print("                ", LINE1(0));
                clcd_print("                ", LINE2(0));
                return 2;
            }
            
        }
        return 0;
    }

    /* Display Login Screen */
    if(once)
    {
        clcd_print("Enter Password ", LINE1(0));
        clcd_print("                ", LINE2(0));
        
        index = 0;
        blink = 0;
        blink_state = 0;

        clcd_putch('_', LINE2(0));
        
        once = 0;
    }

    /* Blinking Cursor */
    if(index < 4)
    {
        if(++blink >= 200)
        {
            blink = 0;
            if(blink_state)
                clcd_putch('_', LINE2(index));
            else
                clcd_putch(' ', LINE2(index));
            blink_state = !blink_state;
        }
    }

     /* SW1 represents binary digit '1' */
    if(key == SW1 && index < 4)
    {
        entered[index] = '1';

        clcd_putch('*', LINE2(index));
        
        index++;
        if(index < 4)
        {
            clcd_putch('_', LINE2(index));
        }
    }

     /* SW2 represents binary digit '0' */
    else if(key == SW2 && index < 4)
    {
        entered[index] = '0';

        clcd_putch('*', LINE2(index));

        index++;
        if(index < 4)
        {
            clcd_putch('_', LINE2(index));
        }
    }

    /* Password Complete */
    if(index == 4)
    {
        entered[4] = '\0';
        if(strcmp(entered, password) == 0)
        {
            attempts = 3;

            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));

            once = 1;
            index = 0;
            return 1;
        }
        else
        {
            /* Wrong password */
            attempts--;

            clcd_print("Wrong Password ", LINE1(0));
            clcd_print("Attempt Left:  ", LINE2(0));
            clcd_putch(attempts + '0', LINE2(14));

            for(unsigned long int i = 0; i < 100000; i++);

            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));

            if(attempts == 0)
            {
                blocked = 1;
                sec_left = 30;
                delay = 0;
            }
            index = 0;
            once = 1;
        }
    }

    return 0;
}

void menu(unsigned char key)
{
    static unsigned char top = 0;
    static unsigned char select = 0;
    static unsigned char redraw = 1;

    /* SW1 : UP */
    if(key == SW1)
    {
        if(select == 1)
        {
            select = 0;
            redraw = 1;
        }
        else
        {
            if(top > 0)
            {
                top--;
                redraw = 1;
            }
        }
    }
    /* SW2 : DOWN */
    else if(key == SW2)
    {
        if(select == 0)
        {
            select = 1;
            redraw = 1;
        }
        else
        {
            if(top < 3)
            {
                top++;
                redraw = 1;
            }
        }
    }
    /* ENTER */
    else if(key == SW4)
    {
        unsigned char menu_pos;

        menu_pos = top + select;
        /*menus*/
        switch(menu_pos)
        {
            case 0:
                while(1)
                {
                    key = read_digital_keypad(STATE);
                    view_log(key);

                    if(key == SW5)
                    {
                        break;
                    }
                }
                break;

            case 1:
                clear_log();
                break;

            case 2:
                download_log();
                break;

            case 3:
                /* set time */
                clcd_print("                ", LINE1(0));
                clcd_print("                ", LINE2(0));
                while(1)
                {
                    key = read_digital_keypad(STATE);
                    int ret = set_time(key);

                    if(ret == 1)     // save
                    {
                        operation = 0;   // dashboard
                        return;
                    }
                    else if(ret == 2)    // cancel
                    {
                        break;           // back to menu
                    }
                }
                break;

            case 4:
                /* change password */
                while(1)
                {
                    key = read_digital_keypad(STATE);
                    int ret = change_password(key);
                    if(ret == 1)
                    {
                        break;      // back to menu
                    }
                    if(key == SW5)
                    {
                        break;
                    }
                }
                break;
        }

        redraw = 1;
    }
    /* Back to menu on LCD */
    if(redraw)
    {
        redraw = 0;

        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));

        if(select == 0)
        {
            clcd_putch('*', LINE1(0));
            clcd_print(str[top], LINE1(2));

            if(top < 4)
                clcd_print(str[top + 1], LINE2(2));
        }
        else
        {
            clcd_print(str[top], LINE1(2));

            clcd_putch('*', LINE2(0));

            if(top < 4)
                clcd_print(str[top + 1], LINE2(2));
        }
    }
}

void view_log(unsigned char key)
{
    // stores the currently displayed log number 
    static unsigned char log_pos = 0;
    unsigned char log[17];
    /* no logs present */
    if(event_count == 0)
    {
        clcd_print(" NO LOGS FOUND ", LINE1(0));
        clcd_print("                ", LINE2(0));
        // reset position when returning to menu 
        if(key == SW5)
        {
            log_pos = 0;
        }
        return;
    }
    /* scroll Up */
    if(key == SW1)
    {
        // prevent moving before first log 
        if(log_pos > 0)
        {
            log_pos--;
        }
    }
    
    /* scroll Down */
    else if(key == SW2)
    {
        // prevent moving beyond last stored log 
        if(log_pos < (event_count - 1))
        {
            log_pos++;
        }
    }

    // back to Menu 
    else if(key == SW5)
    {
        log_pos = 0;
        return;
    }

    unsigned char addr = log_pos * 10;   // to calculate eeprom adress of selected log

    clcd_print("#  TIME    EV SP   ", LINE1(0));
    /* Log Number */
    log[0]  = log_pos + '0';
    log[1]  = ' ';
    /* time */
    log[2]  = read_external_eeprom(addr++);
    log[3]  = read_external_eeprom(addr++);
    log[4]  = ':';
    log[5]  = read_external_eeprom(addr++);
    log[6]  = read_external_eeprom(addr++);
    log[7]  = ':';
    log[8]  = read_external_eeprom(addr++);
    log[9]  = read_external_eeprom(addr++);
    log[10] = ' ';
    /* Event */
    log[11] = read_external_eeprom(addr++);
    log[12] = read_external_eeprom(addr++);
    log[13] = ' ';
    /* Speed */
    log[14] = read_external_eeprom(addr++);
    log[15] = read_external_eeprom(addr++);
    log[16] = '\0';

    clcd_print((char *)log, LINE2(0));
}

void clear_log(void)
{
    static unsigned int delay = 0;
    static unsigned char once = 1;

    /* execute only once */
    if(once)
    {
        // clear all logs
        event_count = 0;

        clcd_print("Logs are      ", LINE1(0));
        clcd_print("clearing      ", LINE2(0));

        once = 0;
        delay = 0;
    }

    // non-blocking delay
    if(delay++ < 50000)
    {
        return;
    }
    // restore for next use
    once = 1;

    // return to menu
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
}

// Download all logs through UART
void download_log(void)
{
    unsigned char i;
    unsigned char addr = 0;

    clcd_print("DOWNLOADING... ", LINE1(0));
    clcd_print("PLEASE WAIT    ", LINE2(0));

    uart_puts("\r\n");   //Cursor starts from next line 
    uart_puts("TIME     EV SP\r\n");
    uart_puts("----------------\r\n");

    if(event_count == 0)
    {
        uart_puts("NO LOGS FOUND\r\n");

        clcd_print("NO LOGS        ", LINE1(0));
        clcd_print("FOUND          ", LINE2(0));

        __delay_ms(1500);
        return;
    }

    for(i = 0; i < event_count; i++)  // Read each log and displays via UART serial terminal
    {
        //hour
        uart_putch(read_external_eeprom(addr++));
        uart_putch(read_external_eeprom(addr++));
        uart_putch(':');

        //min
        uart_putch(read_external_eeprom(addr++));
        uart_putch(read_external_eeprom(addr++));
        uart_putch(':');

        //sec
        uart_putch(read_external_eeprom(addr++));
        uart_putch(read_external_eeprom(addr++));
        uart_putch(' ');

        //event
        uart_putch(read_external_eeprom(addr++));
        uart_putch(read_external_eeprom(addr++));
        uart_putch(' ');

        //speed
        uart_putch(read_external_eeprom(addr++));
        uart_putch(read_external_eeprom(addr++));

        uart_puts("\r\n");
    }

    clcd_print("DOWNLOAD DONE  ", LINE1(0));
    clcd_print("SUCCESS        ", LINE2(0));

    __delay_ms(1500);
}

unsigned char set_time(unsigned char key)
{
    static unsigned char once = 1;
    static unsigned char field = 0;
    static unsigned int blink = 0;
    static unsigned char visible = 1;
    static unsigned char hours;
    static unsigned char mins;
    static unsigned char secs;

    char temp[9];

    if(once)
    {
        get_time(clock_reg);

        hours = (((clock_reg[0] & 0x3F) >> 4) * 10) +
                 (clock_reg[0] & 0x0F);

        mins = (((clock_reg[1] & 0x7F) >> 4) * 10) +
                (clock_reg[1] & 0x0F);

        secs = (((clock_reg[2] & 0x7F) >> 4) * 10) +
                (clock_reg[2] & 0x0F);

        once = 0;
    }

    /* Increment selected field */
    if(key == SW1)
    {
        if(field == 0)
        {
            hours++;
            if(hours > 23)
            {
                hours = 0;
            }
        }
        else if(field == 1)
        {
            mins++;
            if(mins > 59)
            {
                mins = 0;
            }
        }
        else
        {
            secs++;
            if(secs > 59)
            {
                secs = 0;
            }
        }
    }

    /* Change field HH -> MM -> SS -> HH */
    if(key == SW3)
    {
        field++;

        if(field > 2)
        {
            field = 0;
        }
    }

    /* Decrement selected field */
    if(key == SW2)
    {
        if(field == 0)
        {
            if(hours == 0)
                hours = 23;
            else
                hours--;
        }
        else if(field == 1)
        {
            if(mins == 0)
                mins = 59;
            else
                mins--;
        }
        else
        {
            if(secs == 0)
                secs = 59;
            else
                secs--;
        }
    }

    temp[0] = (hours / 10) + '0';
    temp[1] = (hours % 10) + '0';
    temp[2] = ':';

    temp[3] = (mins / 10) + '0';
    temp[4] = (mins % 10) + '0';
    temp[5] = ':';

    temp[6] = (secs / 10) + '0';
    temp[7] = (secs % 10) + '0';
    temp[8] = '\0';

    clcd_print("HH:MM:SS        ", LINE1(4));

    blink++;
    /* Blink selected field */
    if(blink >= 5)
    {
        blink = 0;
        visible = !visible;
    }

    if(visible)
    {
        clcd_print(temp, LINE2(4));
    }
    else
    {
        switch(field)
        {
            case 0:
                temp[0] = ' ';
                temp[1] = ' ';
                break;

            case 1:
                temp[3] = ' ';
                temp[4] = ' ';
                break;

            case 2:
                temp[6] = ' ';
                temp[7] = ' ';
                break;
        }

        clcd_print(temp, LINE2(4));
    }

    /* Save and go to dashboard */
    if(key == SW4)
    {
        write_ds1307(HOUR_ADDR,
                     ((hours / 10) << 4) | (hours % 10));

        write_ds1307(MIN_ADDR,
                     ((mins / 10) << 4) | (mins % 10));

        write_ds1307(SEC_ADDR,
             (((secs / 10) << 4) | (secs % 10)) & 0x7F);

        once = 1;
        field = 0;
        blink = 0;
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));

        return 1;
    }

    /* Exit without saving */
    if(key == SW5)
    {
        once = 1;
        field = 0;
        blink = 0;
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));
        return 2;
    }

    return 0;
}




int change_password(unsigned char key)
{
    static unsigned char state = 0;
    static unsigned char index = 0;
    static unsigned char once = 1;
    static unsigned long int delay;

    static char temp[5];
    static char temp2[5];

    switch(state)
    {
        /* Enter new password */
        case 0:

            if(once)
            {
                clcd_print("                ", LINE1(0));clcd_print("                ", LINE2(0));

                clcd_print("ENTER NEW PASS ", LINE1(0));
                clcd_putch('_', LINE2(0));

                index = 0;
                once = 0;
            }

            if(key == SW1 && index < 4)
            {
                temp[index++] = '1';
                clcd_putch('*', LINE2(index - 1));
            }
            else if(key == SW2 && index < 4)
            {
                temp[index++] = '0';
                clcd_putch('*', LINE2(index - 1));
            }

            if(index == 4)
            {
                temp[4] = '\0';

                state = 1;
                index = 0;
                once = 1;
            }

            break;

        /* Re-enter password */
        case 1:

            if(once)
            {
                clcd_print("                ", LINE1(0));clcd_print("                ", LINE2(0));

                clcd_print("RE-ENTER PASS  ", LINE1(0));
                clcd_putch('_', LINE2(0));

                once = 0;
            }

            if(key == SW1 && index < 4)
            {
                temp2[index++] = '1';
                clcd_putch('*', LINE2(index - 1));
            }
            else if(key == SW2 && index < 4)
            {
                temp2[index++] = '0';
                clcd_putch('*', LINE2(index - 1));
            }

            if(index == 4)
            {
                temp2[4] = '\0';

                clcd_print("                ", LINE1(0));clcd_print("                ", LINE2(0));

                if(strcmp(temp, temp2) == 0)
                {
                    strcpy(password, temp);

                    clcd_print("PASSWORD IS    ", LINE1(0));
                    clcd_print("CHANGED        ", LINE2(0));
                }
                else
                {
                    clcd_print("RE-ENTERED PWD ", LINE1(0));
                    clcd_print("IS WRONG       ", LINE2(0));
                }

                delay = 0;
                state = 2;
            }

            break;

        /* Display result */
        case 2:

            if(delay++ > 100)
            {
                clcd_print("                ", LINE1(0));clcd_print("                ", LINE2(0));

                state = 0;
                once = 1;
                index = 0;
                return 1;
            }

            break;
    }
    return 0;
}