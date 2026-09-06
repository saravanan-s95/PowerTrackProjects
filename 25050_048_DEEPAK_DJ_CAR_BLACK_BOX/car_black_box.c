#include "car_black_box.h"
#include "newfile.h"
#include "i2c.h"
#include"ds1307.h"
#include "clcd.h"
#include "adc.h"
#include "xc.h"
#include "uart.h"

extern char address;
extern char EEPROM_BUFFER[10];
extern char login;
unsigned char event_count; // Add this near your other externs at the top

void shift(void) {
    for (unsigned char i = 0; i < 90; i++) {
        char temp_byte = eeprom_read(i + 10);
        eeprom_write(i, temp_byte);          
    }
}
void store(char *clock_reg, char *gear_str, unsigned int speed) {
    
    EEPROM_BUFFER[0] = ((clock_reg[0] >> 4) & 0x03) + '0'; 
    EEPROM_BUFFER[1] = (clock_reg[0] & 0x0F) + '0';
    EEPROM_BUFFER[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    EEPROM_BUFFER[3] = (clock_reg[1] & 0x0F) + '0';
    
    EEPROM_BUFFER[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    EEPROM_BUFFER[5] = (clock_reg[2] & 0x0F) + '0';
    
    EEPROM_BUFFER[6] = gear_str[0]; 
    EEPROM_BUFFER[7] = gear_str[1]; 
    
    EEPROM_BUFFER[8] = (speed / 10) + '0';
    EEPROM_BUFFER[9] = (speed % 10) + '0';
    if (event_count == 10) {
        shift();
        address = 90; 
    } else {
        event_count++; 
    }
    for (unsigned char i = 0; i < 10; i++) {
        eeprom_write(address, EEPROM_BUFFER[i]);
        address++;
    }
}

int my_strcmp(char *str,char *str2){
    while(*str && *str2 && (*str == *str2)){
        str++;
        str2++;
    }
    return *str - *str2;
}

static void display_time(unsigned char *clock_reg)
{
    //BCD Format 
    //clock_reg[0] = HH
    //clock_reg[1] = MM
    //clock_reg[2] = SS
   
    /* To store the time in HH:MM:SS format */
    char time[9];  // "HH:MM:SS"
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    time[2] = ':';
    // MM 
    time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[4] = (clock_reg[1] & 0x0F) + '0';
    
    time[5] = ':';
    // SS
    time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[7] = (clock_reg[2] & 0x0F) + '0';
    time[8] = '\0';
    
    clcd_print(time, LINE2(1)); // HH:MM:SS 
}

static void get_time(unsigned char *clock_reg)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
}
char clock_reg[3];

void dash_board(unsigned char key){
    if(login != 1){
            static char gear_index = 0,once = 0,collision  = 0;
            char *gear[] = {"ON","GN","GR","G1","G2","G3","G4","G5","_C"};

            get_time(clock_reg);
            display_time(clock_reg);

            if(key == SW1){
                gear_index = 8;
                collision = 1;
            }else if(key == SW2){
                if(collision == 1){
                    collision = 0;
                    gear_index = 1;
                }
                else if(gear_index < 7)
                  gear_index++;
            }else if(key == SW3){

                 if(collision == 1){
                    collision = 0;
                    gear_index = 1;
                }
                 else  if(gear_index > 1)
                  gear_index--;
            }
            clcd_print(gear[gear_index],LINE2(10));


             // display speed;
            unsigned int speed = read_adc();
            speed = speed/10.23;
            clcd_putch(speed/10 +'0',LINE2(14));
            clcd_putch(speed%10 + '0',LINE2(15));


           if(key == SW1 || key == SW2 || key == SW3 || once == 0){
                store(clock_reg, gear[gear_index], speed);
                once = 1;
             }
    }
}
unsigned char flag = 0;
    char pass[5] ="0001";
    char temp[5];
    char ast[5]="";
 //   char temp_pass[5];
    unsigned char index = 0;
    unsigned char c_index = 0;
    unsigned char attempts = 3;
  //  unsigned char blink = 200;
    unsigned char led = 0;
    unsigned char delay =0;
    unsigned char cursor = 0;
    unsigned char c_delay = 0;
    unsigned char atm[17] = "3  attempts left";
    unsigned char menu_bit = 0;
void login_mode(unsigned char key){
    
    static char clr= 0;
    if(clr == 0){
        clcd_print("                ",LINE2(0));
        clr = 1;
    }
    if(flag == 2){
        delay = 60;
        while(delay-- >= 1 ){
            char wait[17]=" wait for -- sec";
            wait[11] = delay%10 + '0';
            wait[10] = delay/10 + '0';
            clcd_print("THINK better -->",LINE1(0));
            clcd_print(wait,LINE2(0));
            for(volatile unsigned int  i = 0;i<1000;i++);
           
        }
        delay = 0;
        flag = 0;
        index = 0;
        clcd_print("                ",LINE2(0));
        attempts = 3;
        ast[index]='\0';
        temp[index] ='\0';
    }
    if(flag == 1){
       static unsigned char s_delay = 0;
       
       if(menu_bit == 0){
           while(s_delay++ >= 20)
               clcd_print("CORRECT PASSWORD",LINE1(0));
       }
          menu(key);
          // static char S_delay = 0;
        
//        if(S_delay++ >= 5){
//            login = 0;
//            flag = 0;       
//            S_delay = 0; 
//            clcd_print(" TIME     EV  SP ", LINE1(0)); 
//            clcd_print("                ", LINE2(0));
//        }
          s_delay = 0;
          return;
    }
    if(key == SW6){
        login = 0;
         flag = 0;
         index = 0;
         ast[0] = '\0';
         clr = 0;
         clcd_print(" TIME     EV  SP ", LINE1(0)); 
         clcd_print("                ", LINE2(0));
        return;
    }
        clcd_print("ENTER  PASSWORD ",LINE1(0));
        
        clcd_print(ast,LINE2(0));
        
        if(c_delay++ >= 10)
        {
         c_delay = 0;

          if(cursor == 0)
          {
            clcd_putch('_', LINE2(index));
            cursor = 1;
          }
          else
          {
           clcd_putch(' ', LINE2(index));
           cursor = 0;
          }
         }
       // unsigned char key  = read_digital_keypad(STATE);
        if(key == SW1){
            ast[index]='*';
            ast[index+1] ='\0';
            temp[index++] = '0';
        }else if(key == SW2){
             ast[index]='*';
             ast[index+1]='\0';
            temp[index++] = '1';
        }
        if(index == 4){
            temp[index]='\0';
            if(!my_strcmp(temp,pass)){
                flag = 1;
                clcd_print("                ", LINE2(0));
                ast[0]='\0';
            }else{
                index = 0;
                attempts--;
                atm[0] = attempts + '0';
                while(delay++ <= 20){
                    clcd_print("WRONG PASSWORD",LINE1(0));
                    clcd_print(atm,LINE2(0));
                }
                ast[0]='\0';
                clcd_print("                ", LINE2(0));
                delay = 0;
            }
        }
        if(attempts == 0){
            flag = 2;
        }
}
 static unsigned char menu_inside = 0;
void menu(unsigned char key)
{
    menu_bit = 1;
    char *menu_items[5] = {
        "view log      ",
        "clear log     ",
        "DOWNLOAD LOG  ",
        "SET TIME      ",
        "change passwd "
    };
   
    static unsigned char menu_index = 0; 
    static unsigned char star_pos = 0;   
    if(key == SW6 && menu_inside ==0){
        login = 0;
         flag = 0;
         index = 0;
         ast[0] = '\0';
        // clr = 0;
         menu_index = 0;
         star_pos = 0;
         clcd_print(" TIME     EV  SP ", LINE1(0)); 
         clcd_print("                ", LINE2(0));
        return;
    }
   // if(menu_inside == 1){
        if (key == SW3 && menu_inside == 0)
        {
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
            menu_inside = 1;
        }
        if(menu_inside == 1){
            switch (menu_index)
            {
                case 0:
                    view_log(key);
                    break;
                case 1:   
                    clear_log(key);
                    break;
                case 2:
                    download_log(key);
                    break;
                case 3:
                    set_time(key);
                    break;
                case 4:
                     change_passwd(key);
                    break;
            }
           // menu_inside = 0;
            return; 
        }
    

    else if (key == SW2)
    {
        if (menu_index < 4) 
        {
            menu_index++;
            if (star_pos < 1) 
            {
                star_pos++;
            }
        }
    }
    else if (key == SW1)
    {
        if (menu_index > 0) 
        {
            menu_index--;
            if (star_pos > 0) 
            {
                star_pos--;
            }
        }
    }
  //  if(menu_inside != 1){
    unsigned char top_index = menu_index - star_pos;
    if (star_pos == 0) {
        clcd_print("* ", LINE1(0));
    } else {
        clcd_print("  ", LINE1(0)); 
    }
    clcd_print(menu_items[top_index], LINE1(2)); 
    if (star_pos == 1) {
        clcd_print("* ", LINE2(0)); 
    } else {
        clcd_print("  ", LINE2(0)); 
    }
    clcd_print(menu_items[top_index + 1], LINE2(2)); 
}

void change_passwd(unsigned char key)
{
     static char temp_pass[5] = ""; 
    static unsigned char cp_index = 0;
    static unsigned char c_delay = 0;
    static unsigned char cursor = 0;

    clcd_print("ENTER NEW PASSWD", LINE1(0));
    clcd_print(temp_pass, LINE2(0));

    if (c_delay++ >= 10)
    {
        c_delay = 0;
        if (cursor == 0)
        {
            clcd_putch('_', LINE2(cp_index));
            cursor = 1;
        }
        else
        {
            clcd_putch(' ', LINE2(cp_index));
            cursor = 0;
        }
    }

    if (key == SW1)
    {
        temp_pass[cp_index] = '0';
        cp_index++;
        temp_pass[cp_index] = '\0';
    }
    else if (key == SW2)
    {
        temp_pass[cp_index] = '1';
        cp_index++;
        temp_pass[cp_index] = '\0';
    }

    if (cp_index == 4)
    {
        for (int i = 0; i < 5; i++)
        {
            pass[i] = temp_pass[i];
        }

        clcd_print(" PASSWD CHANGED ", LINE1(0));
        clcd_print("  SUCCESSFULLY  ", LINE2(0));

        for (volatile unsigned long int i = 0; i < 200000; i++); 
        cp_index = 0;
        temp_pass[0] = '\0'; 
        menu_inside = 0;
        login = 0;  
        flag = 0;
        clcd_print(" TIME     EV  SP ", LINE1(0));
        clcd_print("                ", LINE2(0)); 
    }
}
char view_log_index = 1;

void view_log(unsigned char key){
     clcd_print("  TIME    EV  SP ", LINE1(0));
     
     if (key == SW2 && view_log_index < (event_count - 1))
        {
            view_log_index++;
        }
        else if (key == SW1 && view_log_index > 0)
        {
            view_log_index--;
        }
        unsigned char addr = view_log_index * 10;
        char log_str[17];
        log_str[0] = view_log_index + '0';
        log_str[1] = '.';
        log_str[2] = eeprom_read(addr + 0);  
        log_str[3] = eeprom_read(addr + 1); 
        log_str[4] = ':';
        log_str[5] = eeprom_read(addr + 2); 
        log_str[6] = eeprom_read(addr + 3);
        log_str[7] = ':';
        log_str[8] = eeprom_read(addr + 4);
        log_str[9] = eeprom_read(addr + 5); 
        log_str[10] = ' ';                                        
        log_str[11] = eeprom_read(addr + 6); 
        log_str[12] = eeprom_read(addr + 7); 
        log_str[13] = ' ';                                  
        log_str[14] = eeprom_read(addr + 8); 
        log_str[15] = eeprom_read(addr + 9); 
        log_str[16]='\0';
       // log_str[17] = '\0';                 
        clcd_print(log_str, LINE2(0));
     if(key == SW6)
    {
        menu_inside = 0;
        view_log_index = 0; 
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));
    }
}

void clear_log(unsigned char key)
{
    event_count = 0;
    address = 0;
    clcd_print("ALL LOGS DELETED", LINE1(0));
    clcd_print("                ", LINE2(0));
    //for (volatile unsigned long int i = 0; i < 200000; i++); 
    if(key == SW6){
     menu_inside = 0; 
     clcd_print("                ", LINE1(0));
     clcd_print("                ", LINE2(0));
     }
}

extern unsigned char ch;

void download_log(unsigned char key){
       static unsigned char downloaded = 0;
       if (downloaded == 0) {
                clcd_print(" DOWNLOADING... ", LINE1(0));
                clcd_print("                ", LINE2(0));
                puts("NO. TIME     EV  SP\r\n");
                for (unsigned char i = 0; i < event_count; i++)
                {
                    unsigned char addr = i * 10;
                    char log_str[19];

                    log_str[0] = i + '0'; 
                    log_str[1] = '.';
                    log_str[2] = eeprom_read(addr + 0);  
                    log_str[3] = eeprom_read(addr + 1);  
                    log_str[4] = ':';
                    log_str[5] = eeprom_read(addr + 2); 
                    log_str[6] = eeprom_read(addr + 3);  
                    log_str[7] = ':';
                    log_str[8] = eeprom_read(addr + 4);  
                    log_str[9] = eeprom_read(addr + 5); 
                    log_str[10] = ' ';                   
                    log_str[11] = eeprom_read(addr + 6); 
                    log_str[12] = eeprom_read(addr + 7); 
                    log_str[13] = ' ';                   
                    log_str[14] = eeprom_read(addr + 8); 
                    log_str[15] = eeprom_read(addr + 9);
                    log_str[16] = '\r';
                    log_str[17] = '\n'; 
                    log_str[18] = '\0';
                    puts(log_str); 
                }
                downloaded = 1;
                clcd_print("DOWNLOAD SUCCESS", LINE2(0));
            }
    
    if(key == SW6){
        menu_inside = 0;
        downloaded = 0;
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));
    }
}

unsigned char set_time_index = 4;
unsigned char time_succ = 0;
void set_time(unsigned char key){
    
//    clcd_print("    SET TIME    ",LINE1(0));
//    clcd_print("    00:00:00    ",LINE2(0));
//    if
            static unsigned char hr = 0, min = 0, sec = 0;
            static unsigned char c_delay = 0, cursor = 0;
    if(time_succ ==0){
          

            char time_str[17] = "    00:00:00    ";
            time_str[4] = (hr / 10) + '0';
            time_str[5] = (hr % 10) + '0';
            time_str[7] = (min / 10) + '0';
            time_str[8] = (min % 10) + '0';
            time_str[10] = (sec / 10) + '0';
            time_str[11] = (sec % 10) + '0';

           if(c_delay++ >= 10)
            {
                c_delay = 0;
                if(cursor == 0)
                {
                    cursor = 1;
                }
                else
                {
                    cursor = 0;
                }
            }
        if(cursor == 1)
        {
            time_str[set_time_index] = '_';
            time_str[set_time_index + 1] = '_';
        }
        clcd_print("    SET TIME    ", LINE1(0));
        clcd_print(time_str, LINE2(0));

        if(key == SW2) // Increment
        { 
            if(set_time_index == 4) 
            {
                hr++;  
                if(hr > 23) hr = 0; 
            }
            if(set_time_index == 7){
                min++; 
                if(min > 59) min = 0;
            }
            if(set_time_index == 10){
                sec++; 
                if(sec > 59) sec = 0; 
            }
        }
    
        if(key == SW1) // Decrement
        { 
            if(set_time_index == 4){ 
                if(hr == 0) hr = 23; 
                else hr--; 
            }
            if(set_time_index == 7){ 
                if(min == 0) min = 59;
                else min--;
            }
            if(set_time_index == 10){
                if(sec == 0) sec = 59;
                else sec--;
            }
        }

        if(key == SW3) // Move forward
        { 
            set_time_index += 3;
            if(set_time_index > 10) 
            {
                set_time_index = 4;
            }
        }
    }
    if(key == SW5)
    { 
        time_succ = 1;
        
        unsigned char hh_bcd = ((hr / 10) << 4) | (hr % 10);
        unsigned char mm_bcd = ((min / 10) << 4) | (min % 10);
        unsigned char ss_bcd = ((sec / 10) << 4) | (sec % 10);

        //setting time 
        write_ds1307(SEC_ADDR, 0x80);
        write_ds1307(MIN_ADDR, mm_bcd);        
        write_ds1307(HOUR_ADDR, hh_bcd);      
        write_ds1307(SEC_ADDR, ss_bcd & 0x7F); 

        clcd_print(" TIME SET DONE> ", LINE1(0));
        clcd_print("                ",LINE2(0));
        
        
    }
    
    if(key == SW6){
        menu_inside = 0;
        hr = 0;
        min = 0;
        sec = 0;
        time_succ = 0;
        set_time_index = 4;
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));
      }
    
    
}