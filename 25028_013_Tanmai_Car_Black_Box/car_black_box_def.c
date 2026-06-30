#include "main.h"

unsigned char clock_reg[3];
extern unsigned char time[7]; // hh mm ss
unsigned char log[11]; //hh mm ss ev speed
char lap_index = 0;
char total_laps = 0;
char *menu[] = {"View log", "Clear log" ,"Download log", "Set Time" , "Change pwd"};
unsigned char menu_pos = 0;
static void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    // BCD to ascii 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0'; 
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
}
void display_time(void)
{
    get_time();
    clcd_putch(time[0], LINE2(0));
    clcd_putch(time[1], LINE2(1));
    clcd_putch(':', LINE2(2));
    clcd_putch(time[2], LINE2(3));
    clcd_putch(time[3], LINE2(4));
    clcd_putch(':', LINE2(5));
    clcd_putch(time[4], LINE2(6));
    clcd_putch(time[5], LINE2(7));
}
void display_dashboard(unsigned char event[], unsigned char speed)
{
    clcd_print("TIME      EV SPD",LINE1(0));
    
    display_time();
    clcd_print(event, LINE2(10));
    clcd_putch((speed / 10) + '0',LINE2(13));
    clcd_putch((speed % 10) + '0', LINE2(14)); 
}
void log_car_event()
{
    char addr = 0x05;
    addr = 5 + (lap_index * 10);
    eeprom_write_str(addr , log);
    lap_index = (lap_index + 1) % 10;
    if(total_laps < 10)
    {
        total_laps++;
    }
}
void log_event(unsigned char event[], unsigned char speed)
{
    memcpy(log, time, 6);
    memcpy(&log[6], event, 2);
    log[8] = (speed / 10) + '0';
    log[9] = (speed % 10) + '0';
    log[10] = '\0';
    
    log_car_event();
}
void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}
unsigned char log_in_password(unsigned char key, unsigned char reset_flag)
{
    static unsigned char user_pwd[4];
    static unsigned char i = 0;
    static unsigned char attempt_left;
    
    if(reset_flag == RESET_LOGIN_SCREEN)
    {
        i = 0;
        attempt_left = 3;
        user_pwd[0] = '\0';
        user_pwd[1] = '\0';
        user_pwd[2] = '\0';
        user_pwd[3] = '\0';
        key = ALL_RELEASED;
        inactive_time = 5;
    }
   
    if(key == SW4 && i < 4)
    {
        user_pwd[i] = '1';
        i++;
        clcd_putch('*', LINE2(i+4));
        inactive_time = 5;
    }
    else if(key == SW5 && i < 4)
    {
        user_pwd[i] = '0';
        i++;
        clcd_putch('*', LINE2(i+4));
        inactive_time = 5;
    }
     if(inactive_time == 0)
    {
        return RETURN_BACK;
    }
    if (i == 4)
    {
        char stored_pwd[4];
        for(char j = 0; j<4; j++)
        {
            stored_pwd[j] = read_external_eeprom(j);
        }
        if (strncmp(user_pwd, stored_pwd, 4) == 0)
        {
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            clcd_print("LOGIN SUCCESS",LINE1(0));
            __delay_ms(2000);
            return LOGIN_SUCCESS;
        }
        else
        {
            attempt_left--;
            if(attempt_left == 0)
            {
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                clcd_print("MAX ATTEMPT OVER", LINE1(0));
                clcd_print("WAIT FOR", LINE2(0));
                sec = 60;
                while (sec > 0)
                {
                    clcd_putch(sec/10 + '0', LINE2(9));
                    clcd_putch(sec%10 + '0',LINE2(10));
                    clcd_print("secs",LINE2(12));
                }
                attempt_left = 3;
            }
            else
            {
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                clcd_print("WRONG PASSWORD", LINE1(0));
                clcd_putch(attempt_left + '0', LINE2(0));
                clcd_print("ATTEMPT LEFT" , LINE2(2));
                __delay_ms(2000);
            }
            clear_screen();
            clcd_print("ENTER PASSWORD",LINE1(0));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON_BLINK, INST_MODE);
            i = 0;
        }     
    }
      return 0; 
}
unsigned char menu_screen(unsigned char key, unsigned char reset_flag)
{
    if (reset_flag == RESET_MENU_SCREEN)
    {
        clear_screen();
        menu_pos = 0;
        inactive_time = 10;
    }

    if(key == SW4 && menu_pos < 4)
    {
        menu_pos++;
        clear_screen();
        inactive_time = 10;
    }
    else if(key == SW5 && menu_pos > 0)
    {
        menu_pos--;
        clear_screen();
        inactive_time = 10;
    }
    if(inactive_time == 0)
    {
        return RETURN_BACK;
    }
    if(menu_pos == 4)
    {  
        clcd_print(menu[menu_pos - 1], LINE1(2));
        clcd_print(menu[menu_pos], LINE2(2));
        clcd_putch('*', LINE2(0));
    }
    else
    {
        clcd_putch('*', LINE1(0));
        clcd_print(menu[menu_pos], LINE1(2));
        clcd_print(menu[menu_pos + 1], LINE2(2));  
    }
    return menu_pos;
}

void read_log_from_eeprom(unsigned char index, unsigned char *log)
{
    unsigned char addr = 0x05 + (index * 10);

    for(unsigned char i = 0; i < 10; i++)
    {
        log[i] = read_external_eeprom(addr+i);
    }
}

void event_scroll(unsigned char key, unsigned char reset_flag)
{
    static unsigned char slide_index;
    unsigned char start_index;
    unsigned char next_slide;
    if(reset_flag == RESET_VIEW_LOG)
    {
        slide_index = 0;     // start from first log
        key = ALL_RELEASED;
    }

    unsigned char log1[10];
    unsigned char log2[10];
    
    if(total_laps >= 1)
    {
        if(total_laps < 10)
        {
            start_index = 0;
        }
        else
        {
            start_index = lap_index;   // oldest log
        }
        if(key == SW4)
        {
            slide_index++;
            if(slide_index >= total_laps)
                slide_index = 0;
        }

        if(key == SW5)
        {
            if(slide_index == 0)
                slide_index = total_laps - 1;
            else
                slide_index--;
        }
        
        unsigned char index1 = (start_index + slide_index) % total_laps;
        unsigned char index2;
        read_log_from_eeprom(index1, log1);
        clcd_putch(slide_index + '0', LINE1(0));
        clcd_putch(' ', LINE1(1));

        clcd_putch(log1[0], LINE1(2));
        clcd_putch(log1[1], LINE1(3));
        clcd_putch(':', LINE1(4));

        clcd_putch(log1[2], LINE1(5));
        clcd_putch(log1[3], LINE1(6));
        clcd_putch(':', LINE1(7));

        clcd_putch(log1[4], LINE1(8));
        clcd_putch(log1[5], LINE1(9));

        clcd_putch(' ', LINE1(10));

        clcd_putch(log1[6], LINE1(11));
        clcd_putch(log1[7], LINE1(12));

        clcd_putch(' ', LINE1(13));

        clcd_putch(log1[8], LINE1(14));
        clcd_putch(log1[9], LINE1(15));


       if(total_laps > 1) // Only display LINE2 if more than 1 log
        {
             next_slide = (slide_index + 1) % total_laps;
             index2 = (start_index + next_slide) % total_laps;
            read_log_from_eeprom(index2, log2);

            clcd_putch(next_slide + '0', LINE2(0));
            clcd_putch(' ', LINE2(1));

            clcd_putch(log2[0], LINE2(2));
            clcd_putch(log2[1], LINE2(3));
            clcd_putch(':', LINE2(4));

            clcd_putch(log2[2], LINE2(5));
            clcd_putch(log2[3], LINE2(6));
            clcd_putch(':', LINE2(7));

            clcd_putch(log2[4], LINE2(8));
            clcd_putch(log2[5], LINE2(9));

            clcd_putch(' ', LINE2(10));

            clcd_putch(log2[6], LINE2(11));
            clcd_putch(log2[7], LINE2(12));

            clcd_putch(' ', LINE2(13));

            clcd_putch(log2[8], LINE2(14));
            clcd_putch(log2[9], LINE2(15));
        }
        else
        {
            clcd_print("                ", LINE2(0)); // Clear LINE2
        }
    }
    else
    {
        clcd_print("NO LOGS FOUND",LINE1(0));
    }
}
void clear_logs(void)
{
    for(int i = 5; i < 105; i++)
    {
        write_external_eeprom(i, 0xFF);
    }

    total_laps = 0;
    lap_index = 0;
}
void download_logs(void)
{

    puts("\r\n----LOG DATA----\r\n");
    if(total_laps <= 0)
    {
        puts("\r\nNO LOGS RECORDED.....\r\n");
    }
    else
    {
        puts("\r\nlog    Time    event speed\r\n");
        unsigned char log[10]; 
        unsigned char start_index;
        unsigned char index;
        if(total_laps < 10)
        {
            start_index = 0;
        }
        else
        {
            start_index = lap_index;  
        }
            for(unsigned char i = 0; i < total_laps; i++)
            {
                index = (start_index + i) % 10;
                unsigned char addr = 5 + index * 10;
                for(unsigned char j = 0; j < 10; j++)
                {
                    log[j] = read_external_eeprom(addr + j);
                }
                putchar(i + '0');
                puts("   ");

                putchar(log[0]);
                putchar(log[1]);
                putchar(':');

                putchar(log[2]);
                putchar(log[3]);
                putchar(':');

                putchar(log[4]);
                putchar(log[5]);

                puts("    ");

                putchar(log[6]);
                putchar(log[7]);

                puts("     ");

                putchar(log[8]);
                putchar(log[9]);

                puts("\r\n");
         }
    }
        puts("----END----\r\n");
}
unsigned char change_password(unsigned char key, unsigned char reset_flag)
{
    static unsigned char step;
    static unsigned char index;
    static char pwd1[4];
    static char pwd2[4];

    if(reset_flag == RESET_CHANGE_PWD)
    {
        step = 0;
        index = 0;
        clear_screen();
        clcd_print("ENTER NEW PWD", LINE1(0));
        clcd_write(LINE2(4), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON_BLINK, INST_MODE);
        key = ALL_RELEASED;
    }
    if(key == SW4 || key == SW5)
    {
        clcd_putch('*', LINE2(4 + index));

        if(step == 0)
        {
            if(key == SW4)
                pwd1[index] = '1';
            else
                pwd1[index] = '0';
        }
        else
        {
            if(key == SW4)
                pwd2[index] = '1';
            else
                pwd2[index] = '0';
        }
        index++;
    }
    if(index == 4)
    {
        if(step == 0)
        {
            step = 1;
            index = 0;

            clear_screen();
            clcd_print("RE-ENTER PWD", LINE1(0));
            clcd_write(LINE2(4), INST_MODE);
        }
        else
        {
            if(strncmp(pwd1, pwd2, 4) == 0)
            {
                for(unsigned char i = 0; i < 4; i++)
                {
                    write_external_eeprom(i, pwd1[i]);
                }
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                clcd_print("PWD CHANGED", LINE1(2));
                __delay_ms(2000);
                return PASSWORD_CHANGED;
            }
            else
            {
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                clcd_print("PWD MISMATCH", LINE1(1));
                __delay_ms(2000);
                return PASSWORD_FAILED;
            }
        }
    }

    return PASSWORD_WAIT;
}

void set_time(unsigned char key, unsigned char reset_flag)
{
    static unsigned char field;
    static unsigned char blink;

    if(reset_flag == RESET_TIME)
    {
        get_time();
        field = 2; // seconds first
        key = ALL_RELEASED;
        clear_screen();
        clcd_print("SET TIME", LINE1(2));
    }

    if(key == SW4) // UP key
    {
        if(field == 0) // HH
        {
            unsigned char hh = (time[0]-'0')*10 + (time[1]-'0');
            hh = (hh + 1) % 24;

            time[0] = (hh/10) + '0';
            time[1] = (hh%10) + '0';
        }
        else if(field == 1) // MM
        {
            unsigned char mm = (time[2]-'0')*10 + (time[3]-'0');
            mm = (mm + 1) % 60;

            time[2] = (mm/10) + '0';
            time[3] = (mm%10) + '0';
        }
        else // SS
        {
            unsigned char ss = (time[4]-'0')*10 + (time[5]-'0');
            ss = (ss + 1) % 60;

            time[4] = (ss/10) + '0';
            time[5] = (ss%10) + '0';
        }
    }

    if(key == SW5) // DOWN key ? next field
    {
        field = (field - 1) % 3;
    }

    blink++;

    /* Display */

    if(field == 0 && blink % 2)
        clcd_print("  ", LINE2(3));
    else
    {
        clcd_putch(time[0], LINE2(3));
        clcd_putch(time[1], LINE2(4));
    }

    clcd_putch(':', LINE2(5));

    if(field == 1 && blink % 2)
        clcd_print("  ", LINE2(6));
    else
    {
        clcd_putch(time[2], LINE2(6));
        clcd_putch(time[3], LINE2(7));
    }

    clcd_putch(':', LINE2(8));

    if(field == 2 && blink % 2)
        clcd_print("  ", LINE2(9));
    else
    {
        clcd_putch(time[4], LINE2(9));
        clcd_putch(time[5], LINE2(10));
    }
}