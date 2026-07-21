/*
 * File:   dashboard.c
 * Author: 25014
 *
 * Created on 31 December, 2025
 */
#include <xc.h>
#include "main.h"
#include "external_eeprom.h"
#include "uart.h"

#define EVENT_COUNT_ADDR   100
#define WRITE_ADDR         101

unsigned char clock_reg[3], speed[3] = {0};
char  *event[ ] = { "ON",  "GN", "G1", "G2", "G3", "G4", "GR", "C "};
int pos = 0;
unsigned int adc_val;
unsigned event_count = 0;
extern char time[];
char main_password[5] = "1111";

void dashboard(unsigned char key) {
    
        clcd_print(" TIME     EV  SP", LINE1(0));
        
        // display time
        get_time(clock_reg);
        display_time(clock_reg);
        
        // event display ( Gear pos and speed )
        if(key == SW1){
            if(pos == 7){
                pos = 1;
                store_event();
            }
            if(pos < 6){
                pos++;
                store_event();
            }
        }else if(key == SW2){
            if(pos == 7){
                pos = 1;
                store_event();
            }
            if(pos > 1){
                pos--;
                store_event();
            }
        }else if(key == SW3){
            pos = 7;
            store_event();
        }
        
        clcd_print(event[pos] , LINE2(10));
        
        //adc_value using potentiometer
//        adc_val = read_adc(CHANNEL0)/10.23;
        adc_val = (read_adc(CHANNEL0) * 100) / 1023;
        if(adc_val == 100){
           adc_val = 99;
        }
        speed[0] = (unsigned char)(adc_val / 10) + '0';
        speed[1] = (unsigned char)(adc_val % 10) + '0';
        speed[2] = '\0';
        
        clcd_print((char *)speed , LINE2(14));        
}

void store_event()
{
    static unsigned char address = 0x00;
    
    /*Store time*/
    for(int i = 0; i < 8; i++)
    {
        if(i == 2 || i == 5){
            continue;
        }
        write_external_eeprom(address++ , time[i]);
    }
    
    /*Store event*/
    write_external_eeprom(address++ , event[pos][0]);
    write_external_eeprom(address++ , event[pos][1]);
     
    /*Store speed*/
    write_external_eeprom(address++ , speed[0]);
    write_external_eeprom(address++ , speed[1]);
    
    if(address > 99){
        address = 0x00;
    }
    
    event_count++;
    if(event_count > 10)
    {
        event_count = 10;
    }
}

int my_strcmp(char *s1 , char *s2)
{
    while(*s1 != '\0' || *s2 != '\0')
    {
        if(*s1 != *s2)
        {
            return 1;
        }

        s1++;
        s2++;
    }

    return 0;
}

/*Login screen*/
unsigned char login_screen()
{
    unsigned char key;
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
//    char password[5] = "1111";
    char user_password[5];

    int i = 0;
    char attempts = 3;
    int flag = 0;

    unsigned long int blink = 0;
    unsigned char toggle = 0;

    while (1)
    {
        if(flag == 0)
        {
            clcd_print("ENTER PASSWORD", LINE1(0));

            key = read_digital_keypad(STATE);

            if(blink++ == 30000)
            {
                blink = 0;

                toggle = !toggle;

                if(toggle)
                {
                    clcd_putch(0xFF , (unsigned char)LINE2(i));
                }
                else
                {
                    clcd_putch(' ', (unsigned char)LINE2(i));
                }
            }
            
            if(key == SW1 && i < 4)
            {
                user_password[i] = '1';

                clcd_putch('*', (unsigned char)LINE2(i));

                i++;
            }
            else if(key == SW2 && i < 4)
            {
                user_password[i] = '0';

                clcd_putch('*', (unsigned char)LINE2(i));

                i++;
            }

            if(i == 4)
            {
                user_password[4] = '\0';

                if(my_strcmp(main_password , user_password) == 0)
                {
                    flag = 1;
                }
                else
                {
                    attempts--;

                    clcd_print("                ", LINE1(0));
                    clcd_print("                ", LINE2(0));

                    clcd_print("WRONG PASSWORD", LINE1(0));

                    clcd_putch((char)(attempts + '0'), LINE2(0));
                    clcd_print("ATTEMPTS LEFT", LINE2(2));

                    for(unsigned long int wait = 50000; wait--;);

                    if(attempts == 0)
                    {
                        flag = 2;
                    }

                    clcd_print("                ", LINE1(0));
                    clcd_print("                ", LINE2(0));

                    i = 0;
                }
            }
        }

        else if(flag == 1)
        {
            clcd_print("ACCESS GRANTED", LINE1(0));
            clcd_print("              ", LINE2(0));
           
        for(unsigned long int wait = 50000; wait--;); // small delay

        return 1;
        }

        else if(flag == 2)
        {
            unsigned char clock_reg[3];
            unsigned char start_sec, current_sec;
            unsigned char elapsed, remaining;

            get_time(clock_reg);

            start_sec = ((clock_reg[2] >> 4) * 10) + (clock_reg[2] & 0x0F);

            while(1)
            {
                get_time(clock_reg);

                current_sec = ((clock_reg[2] >> 4) * 10) + (clock_reg[2] & 0x0F);

                if(current_sec >= start_sec)
                     elapsed = current_sec - start_sec;
                else
                    elapsed = (60 - start_sec) + current_sec;

                if(elapsed >= 30)
                {
                    flag = 0;
                    attempts = 3;
                    i = 0;

                    clcd_print("                ", LINE1(0));
                    clcd_print("                ", LINE2(0));

                    break;
                }

                remaining = 30 - elapsed;

                clcd_print("User Blocked    ", LINE1(0));

                clcd_putch((remaining / 10) + '0', LINE2(0));
                clcd_putch((remaining % 10) + '0', LINE2(1));

                clcd_print("SEC LEFT", LINE2(3));
            }
        }
    }
    return 0;
}

/*Main menu screen*/
unsigned char main_menu()
{
    const char *str[] = {"View log" , "Clear log" , "Download log" , "Set time" , "Change password"};

    static unsigned char i = 0;
    static unsigned char first_entry = 1;
    static unsigned char prev_i = 255;

    unsigned char key;

    while(1)
    {
        key = read_digital_keypad(STATE);

        if(first_entry || prev_i != i)
        {
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));

            clcd_print("*", LINE1(0));
            clcd_print(str[i], LINE1(2));

            if(i < 4)
                clcd_print(str[i + 1], LINE2(2));

            prev_i = i;
            first_entry = 0;
        }

        if(key == SW1 && i > 0)
        {
            i--;
        }
        else if(key == SW2 && i < 4)
        {
            i++;
        }
        else if(key == SW4)
        {
            switch(i)
            {
                case 0:
                    view_log();
                    break;

                case 1:
                    clear_log();
                    break;

                case 2:
                    download_log();
                    break;

                case 3:
                    //set_time();
                    if(set_time() == 1)
                    {
                        first_entry = 1;
                        pos = 0;
                        i = 0;
                        return 1;
                    }
                    break;

                case 4:
                    change_password();
                    break;
            }

            first_entry = 1;
        }
        else if(key == SW5)
        {
            first_entry = 1;
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
            pos = 0;
            i = 0;
            return 1;
        }
    }
}

void read_logs(char *str , unsigned char address){
    for(int i = 0; i < 14; i++)
    {
        if(i == 2 || i == 5)
        str[i] = ':';
        else if(i == 8 || i == 11)
            str[i] = ' ';
        else
            str[i] = read_external_eeprom(address++);
    }
     str[14] = '\0';
}

/*View log screen*/
void view_log()
{
    clcd_print("# TIME     EV SP", LINE1(0));
    if(event_count == 0)
    {
        clcd_print("NO LOGS FOUND", LINE2(0));
        while(read_digital_keypad(STATE) != SW5);
        return;
    }
    char str[17]; 
    unsigned char log_no = 0;
    unsigned char address = 0;
    unsigned char key;
    while(1){
       key = read_digital_keypad(STATE);
    
        if(log_no == 0){
           read_logs(str , address);
           log_no++;
        }

        if(key == SW1){
            if(log_no < event_count){
                log_no++;
                address = (log_no - 1) * 10;
                read_logs(str , address);
            }
        }

        else if(key == SW2){
            if(log_no > 1)
            {
                log_no--;
                address = (log_no - 1) * 10;
                read_logs(str, address);
            }
        }
        else if(key == SW5){;
           return;
        }
        clcd_putch((log_no - 1) + '0', LINE2(0));
        clcd_print(str, LINE2(2));   
    }  
}

/*Clear log screen*/
void clear_log()
{
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
    event_count = 0;
    clcd_print("Logs are", LINE1(0)); 
    clcd_print("Clearing...." , LINE2(0));
    for(unsigned long int wait = 50000; wait--;);
}

/*Download log screen*/
void download_log()
{
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
    clcd_print("Logs are", LINE1(0)); 
    clcd_print("downloading...." , LINE2(0));
    for(unsigned long int wait = 100000; wait--;);
    clcd_print("Logs Downloaded", LINE1(0)); 
    clcd_print("Open serial port" , LINE2(0));
    for(unsigned long int wait = 100000; wait--;);
    
    puts("# TIME    EV SP\n\r");
    if(event_count == 0)
    {
        puts("NO LOGS FOUND\n\r");
    }
    
    char str[17];
    char log_index[3];
    unsigned char address = 0;
    unsigned char log_no = 1;
    for(int i = 0; i < event_count; i++){
        read_logs(str , address);
        log_index[0] = log_no + '0';
        log_index[1] = ' ';
        log_index[2] = '\0';

        puts(log_index);
        address = log_no * 10;
        log_no++;
        puts(str);
        puts("\n\r");
    }
}

/*Set time screen*/
unsigned char set_time(void)
{
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));

    clcd_print("HH : MM : SS", LINE1(0));

    unsigned char time_arr[3];
    unsigned char key;

    unsigned char field = 0;     // 0 = HH, 1 = MM, 2 = SS
    unsigned char pos;

    unsigned long int blink = 0;
    unsigned char toggle = 0;

    unsigned char hour;
    unsigned char minute;
    unsigned char second;

    get_time(clock_reg);

    for(int i = 0; i < 3; i++)
    {
        time_arr[i] = clock_reg[i];
    }

    hour   = ((time_arr[0] >> 4) * 10) + (time_arr[0] & 0x0F);
    minute = ((time_arr[1] >> 4) * 10) + (time_arr[1] & 0x0F);
    second = ((time_arr[2] >> 4) * 10) + (time_arr[2] & 0x0F);

    display_time(time_arr);

    while(1)
    {
        key = read_digital_keypad(STATE);
        
        if(field == 0)
            pos = 0;
        else if(field == 1)
            pos = 3;
        else
            pos = 6;

        if(++blink >= 30000)
        {
            blink = 0;
            toggle = !toggle;

            if(toggle)
            {
                clcd_putch(0xFF,LINE2(pos));
                clcd_putch(0xFF,LINE2(pos+1));
            }
            else
            {
                clcd_putch(((time_arr[field] >> 4) & 0x0F) + '0', LINE2(pos));

                clcd_putch((time_arr[field] & 0x0F) + '0', LINE2(pos + 1));
            }
        }

        if(key != ALL_RELEASED)
        {
            __delay_ms(20);

            while((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED);

            if(key == SW1)
            {
                if(field == 0)
                {
                    hour++;

                    if(hour > 23)
                        hour = 0;

                    time_arr[0] =(unsigned char)(((hour / 10) << 4) | (hour % 10));
                }
                else if(field == 1)
                {
                    minute++;

                    if(minute > 59)
                        minute = 0;

                    time_arr[1] = (unsigned char)(((minute / 10) << 4) | (minute % 10));
                }
                else
                {
                    second++;

                    if(second > 59)
                        second = 0;

                    time_arr[2] = (unsigned char)(((second / 10) << 4) | (second % 10));
                }

                display_time(time_arr);

                toggle = 0;
                blink = 0;
            }
            else if(key == SW2)
            {
                field++;

                if(field > 2)
                    field = 0;

                display_time(time_arr);

                toggle = 0;
                blink = 0;
            }
            else if(key == SW4)
            {
                write_ds1307(HOUR_ADDR, time_arr[0]);
                write_ds1307(MIN_ADDR,  time_arr[1]);
                write_ds1307(SEC_ADDR,  time_arr[2]);

                clcd_print(" TIME SAVED     ", LINE1(0));
                clcd_print("                ", LINE2(0));

                for(unsigned long int wait = 50000; wait--;);

                return 0;
            }           
            else if(key == SW5)
            {
                return 1;
            }
        }
    }
}

/*To copy new password to main password*/
void my_strcpy(char *main_password , char *new_password){
    while(*new_password != '\0'){
        *main_password = *new_password;
        main_password++;
        new_password++;
    }
    *main_password = '\0';
}

/*Change password screen*/
void change_password()
{
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));

    clcd_print("ENTER NEW PASWRD", LINE1(0));

    char new_password[5];
    char re_entered_password[5];

    unsigned char key;

    unsigned long int blink = 0;
    unsigned char toggle = 0;

    int i = 0;
    int j = 0;

    int flag = 0;

    while(1)
    {
        key = read_digital_keypad(STATE);

        if(blink++ == 30000)
        {
            blink = 0;

            toggle = !toggle;

            if(toggle)
            {
                if(flag == 0)
                    clcd_putch(0xFF, (unsigned char)LINE2(i));
                else
                    clcd_putch(0xFF, (unsigned char)LINE2(j));
            }
            else
            {
                if(flag == 0)
                    clcd_putch(' ', (unsigned char)LINE2(i));
                else
                    clcd_putch(' ', (unsigned char)LINE2(j));
            }
        }

        if(key != ALL_RELEASED)
        {
            __delay_ms(20);

            while((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED);

            if(flag == 0)
            {
                if(key == SW1 && i < 4)
                {
                    new_password[i] = '1';
                    clcd_putch('*', (unsigned char)LINE2(i));
                    i++;
                }
                else if(key == SW2 && i < 4)
                {
                    new_password[i] = '0';
                    clcd_putch('*', (unsigned char)LINE2(i));
                    i++;
                }

                if(i == 4)
                {
                    new_password[4] = '\0';

                    flag = 1;

                    clcd_print("                ", LINE1(0));
                    clcd_print("                ", LINE2(0));

                    clcd_print("RE-ENTER PASSWRD", LINE1(0));

                    toggle = 0;
                    blink = 0;
                }
            }
            else if(flag == 1)
            {
                if(key == SW1 && j < 4)
                {
                    re_entered_password[j] = '1';
                    clcd_putch('*', (unsigned char)LINE2(j));
                    j++;
                }
                else if(key == SW2 && j < 4)
                {
                    re_entered_password[j] = '0';
                    clcd_putch('*', (unsigned char)LINE2(j));
                    j++;
                }

                if(j == 4)
                {
                    re_entered_password[4] = '\0';

                    if(my_strcmp((char *)new_password, (char *)re_entered_password) == 0)
                    {
                        clcd_print("PASSWORD CHANGED", LINE1(0));
                        clcd_print("SUCCESSFULLY    ", LINE2(0));
                        
                        my_strcpy(main_password , new_password);

                        for(unsigned long int wait = 50000; wait--;);

                        return;
                    }
                    else
                    {
                        clcd_print("RE-ENTERED       ", LINE1(0));
                        clcd_print("PASSWRD IS WRONG", LINE2(0));

                        for(unsigned long int wait = 50000; wait--;);

                        return;
                    }
                }
            }
        }
    }
} 