#include<xc.h>
#include "Timer.h"
#include "clcd.h"
#include "digital_timer.h"
#include "ds1307.h"
#include "i2c.h"
#include "newfile.h"

unsigned char address = 0x00;

unsigned char set_event_inside = 0;
unsigned char view_event_inside = 0;
unsigned char event_count = 0;

static void get_time(unsigned char *clock_reg)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR); 
    clock_reg[2] = read_ds1307(SEC_ADDR); 
}
static void get_date(unsigned char *calender_reg)
{
    calender_reg[0] = read_ds1307(DATE_ADDR);  // DD -> BCD 
    calender_reg[1] = read_ds1307(MONTH_ADDR); // MM -> BCD 
    calender_reg[2] = read_ds1307(YEAR_ADDR);  // YY -> BCD 
}
unsigned char calender_reg[3];
unsigned char clock_reg[3];

extern unsigned char show_event_flag;
void dashboard(unsigned char key)
{
    get_time(clock_reg);
    get_date(calender_reg);
    if (show_event_flag == 0 || event_count == 0) {
        char date_str[17] = "DATE 00-00-00   ";
        date_str[5] = ((calender_reg[0] >> 4) & 0x03) + '0';
        date_str[6] = (calender_reg[0] & 0x0F) + '0';
        date_str[8] = ((calender_reg[1] >> 4) & 0x01) + '0';
        date_str[9] = (calender_reg[1] & 0x0F) + '0';
        date_str[11]= ((calender_reg[2] >> 4) & 0x0F) + '0'; 
        date_str[12]= (calender_reg[2] & 0x0F) + '0';
        
        char time_str[17] = "TIME 00:00:00   ";
        time_str[5] = ((clock_reg[0] >> 4) & 0x03) + '0';
        time_str[6] = (clock_reg[0] & 0x0F) + '0';
        time_str[8] = ((clock_reg[1] >> 4) & 0x07) + '0';
        time_str[9] = (clock_reg[1] & 0x0F) + '0';
        time_str[11]= ((clock_reg[2] >> 4) & 0x07) + '0';
        time_str[12]= (clock_reg[2] & 0x0F) + '0';

        clcd_print(date_str, LINE1(0));
        clcd_print(time_str, LINE2(0));
    } 
    
    else {
        char time_str[17] = "TIME 00:00:00   ";
        time_str[5] = ((clock_reg[0] >> 4) & 0x03) + '0';
        time_str[6] = (clock_reg[0] & 0x0F) + '0';
        time_str[8] = ((clock_reg[1] >> 4) & 0x07) + '0';
        time_str[9] = (clock_reg[1] & 0x0F) + '0';
        time_str[11]= ((clock_reg[2] >> 4) & 0x07) + '0';
        time_str[12]= (clock_reg[2] & 0x0F) + '0';
        
        unsigned char r_ev_hr  = eeprom_read(0x00);
        unsigned char r_ev_min = eeprom_read(0x01);
        unsigned char r_freq   = eeprom_read(0x04);
        
        char ev_str[17] = "EVNT 00:00     O";
        ev_str[5] = (r_ev_hr / 10) + '0';
        ev_str[6] = (r_ev_hr % 10) + '0';
        ev_str[8] = (r_ev_min / 10) + '0';
        ev_str[9] = (r_ev_min % 10) + '0';
        
        if(r_freq == 0)      ev_str[15] = 'O'; 
        else if(r_freq == 1) ev_str[15] = 'D';
        else if(r_freq == 2) ev_str[15] = 'W';

        clcd_print(time_str, LINE1(0));
        clcd_print(ev_str, LINE2(0));
    }
    
}

unsigned char cursor = 1;
extern unsigned char menu_inside;
extern unsigned char choose;
unsigned char event_menu_inside = 0;
unsigned char date_time_inside =0;

void menu(unsigned char key){
    if (event_menu_inside == 1) {
        event(key);
        return; 
    }
    if (date_time_inside == 1) {
        date_time(key);
        return;
    }
    if(menu_inside == 0){
        menu_inside = 1;
        cursor = 1;
        clcd_print("->", LINE1(0));
        clcd_print("  ", LINE2(0));
        clcd_print("SET/VIEW EVNT",LINE1(3));
        clcd_print("SET TIME/DATE",LINE2(3));
    }
    if(key == SW2){
        clcd_print("  ",LINE1(0));
        clcd_print("->",LINE2(0));
        cursor = 2;
    }
    if(key == SW1){
        clcd_print("  ",LINE2(0));
        clcd_print("->",LINE1(0));
        cursor = 1;
    }
    if(key == SW3){
       if(cursor == 1){
            event_menu_inside = 1;
            clcd_print("                ",LINE1(0)); 
            clcd_print("                ",LINE2(0));
        }
        else if(cursor == 2){
            date_time_inside = 1;
            clcd_print("                ",LINE1(0)); 
            clcd_print("                ",LINE2(0));
        }
    }
    if(key == SW6){
        clcd_print("                ",LINE1(0));
        clcd_print("                ",LINE2(0));
        choose = 0;
        menu_inside = 0; 
    }
    
   
}
unsigned char event_cursor = 1;
void event(unsigned char key){
        
    if (set_event_inside == 1) {
        set_event(key);
        return; 
    }
    if (view_event_inside == 1) {
        view_event(key);
        return; 
    }
    
    if(key == SW2){
        event_cursor = 2;
    }
    if(key == SW1){
        event_cursor = 1;
    }
    if (event_cursor == 1) {
        clcd_print("->", LINE1(0));
        clcd_print("  ", LINE2(0));
    } else {
        clcd_print("  ", LINE1(0));
        clcd_print("->", LINE2(0));
    }
  
    clcd_print("SET EVENT ",LINE1(3));
    clcd_print("VIEW EVENT",LINE2(3));
    
    if(key == SW3){
        if(event_cursor == 1){
            set_event_inside = 1;
            clcd_print("                ",LINE1(0)); 
            clcd_print("                ",LINE2(0));
        }
        else if(event_cursor == 2){
            view_event_inside = 1;
            clcd_print("                ",LINE1(0));
            clcd_print("                ",LINE2(0));
        }
    }
    if(key == SW6){
        clcd_print("                ",LINE1(0)); 
        clcd_print("                ",LINE2(0));
        event_menu_inside = 0;
        menu_inside = 0;  
    }
}
unsigned char set_time_inside = 0; 
unsigned char set_date_inside = 0;
unsigned char dt_cursor = 1;

void date_time(unsigned char key){
    if (set_time_inside == 1) {
        set_time(key);
        return; 
    }
    if (set_date_inside == 1) {
        set_date(key);
        return; 
    }
    if(key == SW2) dt_cursor = 2;
    if(key == SW1) dt_cursor = 1; 
    if (dt_cursor == 1) {
        clcd_print("->SET TIME      ", LINE1(0));
        clcd_print("  SET DATE      ", LINE2(0));
    } else {
        clcd_print("  SET TIME      ", LINE1(0));
        clcd_print("->SET DATE      ", LINE2(0));
    }
    
    if(key == SW3){
        clcd_print("                ",LINE1(0)); 
        clcd_print("                ",LINE2(0));
        if(dt_cursor == 1){
            set_time_inside = 1;
        }
        else if(dt_cursor == 2){
            set_date_inside = 1;
        }
    }
    if(key == SW6){
        clcd_print("                ",LINE1(0)); 
        clcd_print("                ",LINE2(0));
        date_time_inside = 0; 
        menu_inside = 0;
    }
}

void set_date(unsigned char key){
    static unsigned char first_entry = 1;
    static unsigned char dt_day, dt_month, dt_year;
    static unsigned char field_index = 0; 
    static unsigned char blink_delay = 0, blink_state = 0; 

    if (first_entry == 1) {
        unsigned char bcd_day = read_ds1307(DATE_ADDR);
        unsigned char bcd_mon = read_ds1307(MONTH_ADDR);
        unsigned char bcd_yr  = read_ds1307(YEAR_ADDR);

        dt_day  = ((bcd_day >> 4) & 0x03) * 10 + (bcd_day & 0x0F);
        dt_month= ((bcd_mon >> 4) & 0x01) * 10 + (bcd_mon & 0x0F);
        dt_year = ((bcd_yr >> 4) & 0x0F)  * 10 + (bcd_yr & 0x0F);
        
        first_entry = 0; field_index = 0;
    }

    if (blink_delay++ >= 3) { blink_delay = 0; blink_state = !blink_state; }

    char line1[17] = "  DATE SETUP    "; 
    char line2[17] = "    00-00-00    "; 

    line2[4] = (dt_day / 10) + '0';  line2[5] = (dt_day % 10) + '0';
    line2[7] = (dt_month / 10) + '0';line2[8] = (dt_month % 10) + '0';
    line2[10]= (dt_year / 10) + '0'; line2[11]= (dt_year % 10) + '0';
    
    if (blink_state == 1) {
        if(field_index == 0)      { line2[4] = ' '; line2[5] = ' '; }
        else if(field_index == 1) { line2[7] = ' '; line2[8] = ' '; }
        else if(field_index == 2) { line2[10]= ' '; line2[11]= ' '; }
    }

    clcd_print(line1, LINE1(0));
    clcd_print(line2, LINE2(0));

    if (key == SW3) { field_index++; if (field_index > 2) field_index = 0; blink_state = 0; }
    if (key == SW2) { 
        if(field_index == 0)      { dt_day++;   if(dt_day > 31) dt_day = 1; }
        else if(field_index == 1) { dt_month++; if(dt_month > 12) dt_month = 1; }
        else if(field_index == 2) { dt_year++;  if(dt_year > 99) dt_year = 0; }
    }
    if (key == SW1) { 
        if(field_index == 0)      { if(dt_day <= 1) dt_day = 31; else dt_day--; }
        else if(field_index == 1) { if(dt_month <= 1) dt_month = 12; else dt_month--; }
        else if(field_index == 2) { if(dt_year == 0) dt_year = 99; else dt_year--; }
    }
    if (key == SW4 || key == SW6) { 
        if (key == SW4) { 
            unsigned char bcd_day = ((dt_day / 10) << 4) | (dt_day % 10);
            unsigned char bcd_mon = ((dt_month / 10) << 4) | (dt_month % 10);
            unsigned char bcd_yr  = ((dt_year / 10) << 4) | (dt_year % 10);

            write_ds1307(DATE_ADDR, bcd_day);
            write_ds1307(MONTH_ADDR, bcd_mon);
            write_ds1307(YEAR_ADDR, bcd_yr);

            clcd_print("  DATE SAVED!   ", LINE1(0));
            clcd_print("                ", LINE2(0));
            for(unsigned long int i = 600000; i--; );
        }
        
        clcd_print("                ", LINE1(0)); 
        clcd_print("                ", LINE2(0));
        first_entry = 1; set_date_inside = 0; 
    }
}
void set_time(unsigned char key){
    static unsigned char first_entry = 1;
    static unsigned char dt_hr, dt_min, dt_sec;
    static unsigned char field_index = 0; 
    static unsigned char blink_delay = 0, blink_state = 0;
    
    if (first_entry == 1) {
        unsigned char bcd_hr  = read_ds1307(HOUR_ADDR);
        unsigned char bcd_min = read_ds1307(MIN_ADDR);
        unsigned char bcd_sec = read_ds1307(SEC_ADDR);

        dt_hr   = ((bcd_hr >> 4) & 0x03) * 10 + (bcd_hr & 0x0F);
        dt_min  = ((bcd_min >> 4) & 0x07) * 10 + (bcd_min & 0x0F);
        dt_sec  = ((bcd_sec >> 4) & 0x07) * 10 + (bcd_sec & 0x0F);
        
        first_entry = 0; field_index = 0;
    }

    if (blink_delay++ >= 3) { blink_delay = 0; blink_state = !blink_state; }

    char line1[17] = "  TIME SETUP    "; 
    char line2[17] = "    00:00:00    "; 

    line2[4] = (dt_hr / 10) + '0';   line2[5] = (dt_hr % 10) + '0';
    line2[7] = (dt_min / 10) + '0';  line2[8] = (dt_min % 10) + '0';
    line2[10]= (dt_sec / 10) + '0';  line2[11]= (dt_sec % 10) + '0';
    
    if (blink_state == 1) {
        if(field_index == 0)      { line2[4] = ' '; line2[5] = ' '; } 
        else if(field_index == 1) { line2[7] = ' '; line2[8] = ' '; } 
        else if(field_index == 2) { line2[10]= ' '; line2[11]= ' '; } 
    }

    clcd_print(line1, LINE1(0));
    clcd_print(line2, LINE2(0));

    if (key == SW3) { field_index++; if (field_index > 2) field_index = 0; blink_state = 0; }
    if (key == SW2) { 
        if(field_index == 0)      { dt_hr++;  if(dt_hr > 23) dt_hr = 0; }
        else if(field_index == 1) { dt_min++; if(dt_min > 59) dt_min = 0; }
        else if(field_index == 2) { dt_sec++; if(dt_sec > 59) dt_sec = 0; }
    }
    if (key == SW1) { 
        if(field_index == 0)      { if(dt_hr == 0) dt_hr = 23; else dt_hr--; }
        else if(field_index == 1) { if(dt_min == 0) dt_min = 59; else dt_min--; }
        else if(field_index == 2) { if(dt_sec == 0) dt_sec = 59; else dt_sec--; }
    }
    if (key == SW4 || key == SW6) { 
        if (key == SW4) { 
            unsigned char bcd_hr  = ((dt_hr / 10) << 4) | (dt_hr % 10);
            unsigned char bcd_min = ((dt_min / 10) << 4) | (dt_min % 10);
            unsigned char bcd_sec = ((dt_sec / 10) << 4) | (dt_sec % 10);

            write_ds1307(HOUR_ADDR, bcd_hr);
            write_ds1307(MIN_ADDR, bcd_min);
            write_ds1307(SEC_ADDR, bcd_sec);

            clcd_print("  TIME SAVED!   ", LINE1(0));
            clcd_print("                ", LINE2(0));
            for(unsigned long int i = 600000; i--; ); 
        }
        
        clcd_print("                ", LINE1(0)); 
        clcd_print("                ", LINE2(0));
        first_entry = 1; set_time_inside = 0;
    }
}
unsigned char time_succ = 0;



void set_event(unsigned char key) {
    
    if(event_count >= 3){
        clcd_print("MAX EVENTS SET  ",LINE1(0));
        clcd_print("press SW6 back  ",LINE2(0));
        
        if(key == SW6){
            clcd_print("                ",LINE1(0));
            clcd_print("                ",LINE2(0));
            set_event_inside = 0;
        }
        return; 
    }
    
    static unsigned char ev_hr = 0, ev_min = 0; 
    static unsigned char dur_hr = 0, dur_min = 30; 
    static unsigned char freq = 2;
    
    static unsigned char field_index = 0; 
    static unsigned char blink_delay = 0;
    static unsigned char blink_state = 0; 
    if (blink_delay++ >= 3) { 
        blink_delay = 0;
        blink_state = !blink_state;
    }

    char line1[17] = "TIME- 00:00     "; 
    char line2[17] = "DUR - 00:30    W"; 
    line1[6] = (ev_hr / 10) + '0';
    line1[7] = (ev_hr % 10) + '0';
    line1[9] = (ev_min / 10) + '0';
    line1[10]= (ev_min % 10) + '0';
    line2[6] = (dur_hr / 10) + '0';
    line2[7] = (dur_hr % 10) + '0';
    line2[9] = (dur_min / 10) + '0';
    line2[10]= (dur_min % 10) + '0';
    
    if(freq == 0) line2[15] = 'O';      
    else if(freq == 1) line2[15] = 'D'; 
    else if(freq == 2) line2[15] = 'W'; 
    
    if (blink_state == 1) {
        switch (field_index) {
            case 0: line1[6] = ' '; line1[7] = ' '; break; 
            case 1: line1[9] = ' '; line1[10]= ' '; break; 
            case 2: line2[6] = ' '; line2[7] = ' '; break; 
            case 3: line2[9] = ' '; line2[10]= ' '; break;
            case 4: line2[15]= ' '; break;                
        }
    }

    clcd_print(line1, LINE1(0));
    clcd_print(line2, LINE2(0));

    if (key == SW3) { // Move Right
        field_index++;
        if (field_index > 4) field_index = 0;
        blink_state = 0; 
        blink_delay = 0;
    }

    if (key == SW2) { // Increment
        if(field_index == 0) {
            ev_hr++;
            if(ev_hr > 23) ev_hr = 0; 
        }
        else if (field_index == 1){
            ev_min++; 
            if(ev_min > 59) ev_min = 0;
        }
        else if (field_index == 2){ 
            dur_hr++;
            if(dur_hr > 23) dur_hr = 0; 
        }
        else if (field_index == 3){ 
            dur_min++; 
            if(dur_min > 59) dur_min = 0; 
        }
        else if (field_index == 4){ 
            freq++; 
            if(freq > 2) freq = 0; 
        }
    }
    
    if (key == SW1) { // Decrement
        if (field_index == 0){ 
            if(ev_hr == 0) ev_hr = 23; 
            else ev_hr--; 
        }
        else if (field_index == 1){ 
            if(ev_min == 0) ev_min = 59; 
            else ev_min--; 
        }
        else if (field_index == 2){ 
            if(dur_hr == 0) dur_hr = 23;
            else dur_hr--; 
        }
        else if (field_index == 3){ 
            if(dur_min == 0) dur_min = 59; 
            else dur_min--; 
        }
        else if (field_index == 4){ 
            if(freq == 0) freq = 2; 
            else freq--; 
        }
    }
    if (key == SW4) {
       eeprom_write(address, ev_hr);
        eeprom_write(address + 1, ev_min);
        eeprom_write(address + 2, dur_hr);
        eeprom_write(address + 3, dur_min);
        eeprom_write(address + 4, freq);
        
        address += 5;
        event_count++;
        clcd_print("  EVENT SAVED!  ", LINE1(0));
        clcd_print("                ", LINE2(0));
        for(unsigned long int i = 600000; i--; );
        
        clcd_print("                ", LINE1(0)); 
        clcd_print("                ", LINE2(0));
        set_event_inside = 0;
    }
   
    if(key == SW6){
        clcd_print("                ",LINE1(0));
        clcd_print("                ",LINE2(0));
        set_event_inside = 0;
    }
}

void view_event(unsigned char key){
     
    if (event_count == 0) {
        clcd_print("NO EVENTS SAVED ", LINE1(0));
        clcd_print("press SW6 back  ", LINE2(0));
        
        if(key == SW6){
            clcd_print("                ",LINE1(0));
            clcd_print("                ",LINE2(0));
            view_event_inside = 0;
        }
        return;
    }

    static unsigned char view_index = 0;

    if (key == SW2) { 
        if (view_index < (event_count - 1)) {
            view_index++;
        }
    }
    if (key == SW1) {
        if (view_index > 0) {
            view_index--;
        }
    }
    unsigned char read_addr = view_index * 5; 

    unsigned char r_ev_hr   = eeprom_read(read_addr);
    unsigned char r_ev_min  = eeprom_read(read_addr + 1);
    unsigned char r_dur_hr  = eeprom_read(read_addr + 2);
    unsigned char r_dur_min = eeprom_read(read_addr + 3);
    unsigned char r_freq    = eeprom_read(read_addr + 4);
    
    char buffer1[17] = "E - 00:00       ";
    char buffer2[17] = "DUR-00:00      W";

    // Set event number (1, 2, or 3)
    buffer1[1] = (view_index + 1) + '0'; 

    // Set Time
    buffer1[4] = (r_ev_hr / 10) + '0';
    buffer1[5] = (r_ev_hr % 10) + '0';
    buffer1[7] = (r_ev_min / 10) + '0';
    buffer1[8] = (r_ev_min % 10) + '0';

    // Set Duration
    buffer2[4] = (r_dur_hr / 10) + '0';
    buffer2[5] = (r_dur_hr % 10) + '0';
    buffer2[7] = (r_dur_min / 10) + '0';
    buffer2[8] = (r_dur_min % 10) + '0';

    // Set Frequency
    if(r_freq == 0)      buffer2[15] = 'O'; 
    else if(r_freq == 1) buffer2[15] = 'D';
    else if(r_freq == 2) buffer2[15] = 'W'; 
    clcd_print(buffer1, LINE1(0));
    clcd_print(buffer2, LINE2(0));
    
     if(key == SW6){
        clcd_print("                ",LINE1(0));
        clcd_print("                ",LINE2(0));
        view_index =0;
        view_event_inside = 0;
    }
}