/*
 * File:   dashboard.c
 * Author: Swaraj
 *
 * Created on 10 june, 2026
 */
#include <xc.h>
#include <string.h>
#include "main.h"
#include "external_eeprom.h"
#include "uart.h"


unsigned char clock_reg[3];
unsigned char speed[3];
char  *event[ ] = { "ON", "GN", "G1", "G2", "G3", "G4", "GR", "C "};
int pos = 0;
unsigned int adc_val;
unsigned int address = 0;
char password[5] = "1110";
int event_count = 0;

unsigned char menu_pos = 0;
unsigned char old_menu_pos = 255;

extern unsigned char time[9];
extern unsigned char operation;

// Display current time, event and speed
void dashboard(unsigned char key) {
        
        clcd_print(" TIME     EV  SP", LINE1(0));
        
        // display time
        get_time(clock_reg);
        display_time(clock_reg);
        
        // event display ( Gear pos and speed)
        if(key == SW1)
        {
            if(pos == 7)
            {
                pos = 1;
            }
            if(pos < 6)
            {
                pos++;
            }
            store_event();
        }
        
        else if(key == SW2)
        {
            if(pos == 7)
            {
                pos = 1;
            }
            if(pos > 1)
            {
                pos--;
            }
            store_event();
        }
        
        else if (key == SW3)
        {
            pos = 7;
            store_event();
        }
        
        clcd_print(event[pos], LINE2(10));
        
        adc_val = read_adc(CHANNEL0) / 10.23;
        
        
        speed[0] = (adc_val / 10) + '0';
        speed[1] = (adc_val % 10) + '0';
        speed[2] = '\0';
        
        clcd_print(speed, LINE2(14));
        
}

//For storing current time, event and speed into EEPROM
void store_event()
{
    for(int i = 0; i < 8; i++)  // for storing time without ':'
    {
        if(i == 2 || i == 5)
        {
            continue;
        }
        write_external_eeprom(address++, time[i]);
    }
    
    write_external_eeprom(address++, event[pos][0]);   // it stores the event 
    write_external_eeprom(address++, event[pos][1]);
    
    write_external_eeprom(address++, speed[0]);      //it stores speed
    write_external_eeprom(address++, speed[1]);
    
    if(address >= 100)    // when addrss becomes full then reset 
    {
        address = 0;
    }
    
    if(event_count < 10)  //for storing 10 log each of 10 byte
    {
    event_count++;   
    }
}

//For login access by entering 4 bit password
char login(void)
{
    static unsigned char attempts = 0;

    char pass[5];
    unsigned char index = 0;
    unsigned char key = ALL_RELEASED;

    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
    clcd_print(" ENTER PASSWORD ", LINE1(0));

    while(index < 4)    // reading pass from keypad
    {
        key = read_digital_keypad(STATE);

        if(key == SW1 || key == SW2)
        {
            if(key == SW1)
                pass[index] = '1';
            else
                pass[index] = '0';

            clcd_putch('*', LINE2(index));
            index++;

            while(read_digital_keypad(STATE) != ALL_RELEASED);
            __delay_ms(200);
        }
    }

    pass[4] = '\0';

    if(strcmp(pass, password) == 0)   //comparing pass and entered pass both are same or not
    {
        attempts = 0;

        clcd_print(" LOGIN SUCCESS ", LINE1(0));
        clcd_print("                ", LINE2(0));

        __delay_ms(1000);
        return 1;
    }

    attempts++;

    //if attempts exceeds limit then system lock for 2 min we can increase the time by delay
    if(attempts >= 5)
    {
        clcd_print(" SYSTEM LOCKED ", LINE1(0));
        clcd_print(" WAIT 120 SEC  ", LINE2(0));    //lock for 2 min

        for(unsigned int i = 0; i < 120; i++)
        {
            __delay_ms(1000);       // delay of 120 sec( 2min) if systm lock
        }

        attempts = 0;
        return 0;
    }

    clcd_print(" WRONG PASS    ", LINE1(0));

    clcd_print(" Left : ", LINE2(0));
    clcd_putch((5 - attempts) + '0', LINE2(7));   //to show how much attempts left 

    __delay_ms(1500);

    return 0;
}

char *menu_list[] =
{
    "VIEW LOG",
    "CLEAR LOG",
    "DOWNLOAD LOG",
    "SET TIME",
    "CHANGE PASS"
};

// Menu display and option selection
void menu(unsigned char key)
{   
    if(key == SW3)
    {
    operation = 0;   // Back to dashboard
    
    old_menu_pos = 255;
    
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
    
    return;
    }

    if(key == SW1)
    {
        if(menu_pos < 4)
        {
            menu_pos++;
        }
    }
    else if(key == SW2)
    {
        if(menu_pos > 0)
        {
            menu_pos--;
        }
    }

    if(key == SW4)
    {
    switch(menu_pos)
    {
        case 0:
            view_log();
            old_menu_pos = 255;
            break;

        case 1:
            clear_log();
            old_menu_pos = 255;   // Menu force refresh
            break;

        case 2:
            download_log();
            old_menu_pos = 255;
            break;

        case 3:
            set_time();
            old_menu_pos = 255;
            break;

        case 4:
            change_password();
            old_menu_pos = 255;
            break;
    }
    }
    
    if(old_menu_pos != menu_pos)
    {
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));

        clcd_print(">", LINE1(0));
        clcd_print(menu_list[menu_pos], LINE1(2));

        if(menu_pos < 4)
        {
            clcd_print(menu_list[menu_pos + 1], LINE2(2));
        }

        old_menu_pos = menu_pos;
    }
}

// Display stored logs from EEPROM
void view_log(void)
{
    unsigned char log[17];
    unsigned char key;
    static unsigned char index = 0;
    
    //if no logs are present
    if(event_count == 0)
    {
    clcd_print(" NO LOGS FOUND ", LINE1(0));
    clcd_print("                ", LINE2(0));

    __delay_ms(1500);
    return;
    }

    while(1)
    {
        key = read_digital_keypad(STATE);
        
        // Back to Menu
        if(key == SW3)
        {  
            while(read_digital_keypad(STATE) != ALL_RELEASED);  // to come back from view_log
            return;
        }
        
        // Previous Log
        if(key == SW1)
        {
           if(index < (event_count - 1))
            {
                index++;
            } 
        }

        // Next Log
        if(key == SW2)
        {
            if(index > 0)
            {
                index--;
            }
        }

        unsigned char addr = index * 10;   // to calculate eeprom adress of selected log

        clcd_print("#  TIME    EV SP   ", LINE1(0));

        log[0]  = index + '0';
        log[1]  = ' ';
        log[2]  = read_external_eeprom(addr++);
        log[3]  = read_external_eeprom(addr++);
        log[4]  = ':';
        log[5]  = read_external_eeprom(addr++);
        log[6]  = read_external_eeprom(addr++);
        log[7]  = ':';
        log[8]  = read_external_eeprom(addr++);
        log[9]  = read_external_eeprom(addr++);
        log[10] = ' ';
        log[11] = read_external_eeprom(addr++);
        log[12] = read_external_eeprom(addr++);
        log[13] = ' ';
        log[14] = read_external_eeprom(addr++);
        log[15] = read_external_eeprom(addr++);
        log[16] = '\0';

        clcd_print((char *)log, LINE2(0));
    }
}

//Clearing the logs 
void clear_log(void)
{
    for(unsigned char i = 0; i < 100; i++)
    {
        write_external_eeprom(i, 0xFF);  // Clearing from EEPROM as well
    }

    event_count = 0;  // Reset the event count
    address = 0;      // Reset the EEPROM write address

    clcd_print(" CLEAR LOG     ", LINE1(0));
    clcd_print(" SUCCESS       ", LINE2(0));

    __delay_ms(1000);
    return;
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

// Modify RTC time and save into DS1307
void set_time(void)
{
    unsigned char hr, min, sec;
    unsigned char key;
    unsigned char field = 0;   // 0-Hour, 1-Minute, 2-Second

    hr  = ((read_ds1307(HOUR_ADDR) >> 4) * 10) + (read_ds1307(HOUR_ADDR) & 0x0F);
    min = ((read_ds1307(MIN_ADDR)  >> 4) * 10) + (read_ds1307(MIN_ADDR)  & 0x0F);
    sec = ((read_ds1307(SEC_ADDR)  >> 4) * 10) + (read_ds1307(SEC_ADDR)  & 0x0F);

    while(1)
    {
        key = read_digital_keypad(STATE);

        clcd_print("SET TIME       ", LINE1(0));

        clcd_putch((hr / 10) + '0', LINE2(0));
        clcd_putch((hr % 10) + '0', LINE2(1));
        clcd_putch(':', LINE2(2));

        clcd_putch((min / 10) + '0', LINE2(3));
        clcd_putch((min % 10) + '0', LINE2(4));
        clcd_putch(':', LINE2(5));

        clcd_putch((sec / 10) + '0', LINE2(6));
        clcd_putch((sec % 10) + '0', LINE2(7));

        // SW1 -> Increment
        if(key == SW1)
        {
            if(field == 0)
                hr = (hr + 1) % 24;
            else if(field == 1)
                min = (min + 1) % 60;
            else
                sec = (sec + 1) % 60;

            while(read_digital_keypad(STATE) != ALL_RELEASED);  // since we are using STATE
        }

        // SW2 -> Decrement
        if(key == SW2)
        {
            if(field == 0)
                hr = (hr == 0) ? 23 : hr - 1;
            else if(field == 1)
                min = (min == 0) ? 59 : min - 1;
            else
                sec = (sec == 0) ? 59 : sec - 1;

            while(read_digital_keypad(STATE) != ALL_RELEASED);
        }

        // SW4 -> Next / Save
        if(key == SW4)
        {
            while(read_digital_keypad(STATE) != ALL_RELEASED);

            field++;

            if(field >= 3)
            {
                write_ds1307(HOUR_ADDR,
                             ((hr / 10) << 4) | (hr % 10));
                write_ds1307(MIN_ADDR,
                             ((min / 10) << 4) | (min % 10));
                write_ds1307(SEC_ADDR,
                             ((sec / 10) << 4) | (sec % 10));

                __delay_ms(100);
                return;
            }
        }
    }
}

void change_password(void)
{
    unsigned char key;
    unsigned char index = 0;

    clcd_print(" NEW PASS       ", LINE1(0));
    clcd_print("                ", LINE2(0));

    while(index < 4)
    {
        key = read_digital_keypad(STATE);

        //setting SW1 for 1
        if(key == SW1)
        {
            password[index++] = '1';
            clcd_putch('*', LINE2(index - 1));

            while(read_digital_keypad(STATE) != ALL_RELEASED);
        }
        //setting SW2 for 2
        else if(key == SW2)
        {
            password[index++] = '0';
            clcd_putch('*', LINE2(index - 1));

            while(read_digital_keypad(STATE) != ALL_RELEASED);
        }
    }

    password[4] = '\0';

    clcd_print("PASS CHANGED   ", LINE1(0));
    clcd_print("SUCCESS        ", LINE2(0));

    __delay_ms(1500);

    operation = 0;   // back to dashboard

    return;
}