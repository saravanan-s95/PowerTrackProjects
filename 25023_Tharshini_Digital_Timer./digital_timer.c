/*
 * Name        : THARSHINI S
 * File        : digital_timer.c
 * Description : Implements Digital Alarm Clock functionalities including
 *               time/date display, alarm management, menu navigation,
 *               alarm triggering and time/date configuration.
 */

#include "main.h"

unsigned char clock_reg[7]; //Stores hour, minute, second, day, date, month and year in BCD format
unsigned char time[9];
unsigned char date[9];
unsigned char cycle_secs=0;
unsigned char return_time;
unsigned char secs;
unsigned char blink_t=0;
unsigned int alarm_remaining_secs=0;

static void get_time_date(void)
{
    unsigned char hr24,hr12;
    
    clock_reg[0]=read_ds1307(HOUR_ADDR);
    clock_reg[1]=read_ds1307(MIN_ADDR);
    clock_reg[2]=read_ds1307(SEC_ADDR);
    clock_reg[4]=read_ds1307(DATE_ADDR);
    clock_reg[5]=read_ds1307(MONTH_ADDR);
    clock_reg[6]=read_ds1307(YEAR_ADDR);
    
    //Convert BCD hour value from RTC into binary format
    hr24=((clock_reg[0]>>4)&0x03)*10+(clock_reg[0]&0x0F);
    
    //Convert 24-hour format to 12-hour format only for display
    //RTC time remains in 24-hour format for easier alarm comparison
    if(hr24==0)
    {
        hr12=12;  //midnight -> 12 AM
    }
    else if(hr24>12)
    {
        hr12=hr24-12; //convert afternoon hours to 12-hour format
    }
    else
    {
        hr12=hr24; //12 remains PM, 1-11 remain AM
    }
    
    //Create formatted time string "HH:MM AM/PM"
    time[0]=hr12/10 + '0';
    time[1]=hr12%10 + '0';
    time[2]=':';
    time[3]=((clock_reg[1]>>4)&0x07) + '0';
    time[4]=(clock_reg[1]&0x0F) + '0';
    time[5]=' ';
    time[6]=(hr24<12)?'A':'P';
    time[7]='M';
    time[8]='\0';

    //Create formatted date string "DD-MM-YY"
    date[0]=((clock_reg[4]>>4)&0x03) + '0';
    date[1]=(clock_reg[4]&0x0F) + '0';
    date[2]='-';
    date[3]=((clock_reg[5]>>4)&0x01) + '0';
    date[4]=(clock_reg[5]&0x0F) + '0';
    date[5]='-';
    date[6]=((clock_reg[6]>>4)&0x0F) + '0';
    date[7]=(clock_reg[6]&0x0F) + '0';
    date[8]='\0';
}


void display_default_screen(void)
{
    unsigned char alarm_hr,alarm_min, trigger_type;
    unsigned char alarm;
    unsigned char disp_hr;
    
    //Display alarm ringing message
    if(BUZZER)
    {
        clcd_print("ALARM RINGING!  ",LINE1(0));
        clcd_print("Press Any Key   ",LINE2(0));
        return;
    }
    get_time_date(); //Update latest time and date from RTC
    
    if(cycle_secs<3) //Display date and time for the first three seconds
    {
        clcd_print("DATE ",LINE1(0));
        clcd_print((const char*)date,LINE1(5));
        clcd_print("TIME ",LINE2(0));
        clcd_print((const char*)time,LINE2(5));    
        clcd_print("   ", LINE2(13));
    }
    else //Display the next scheduled alarm for the remaining time
    {        
        alarm=get_next_alarm(&alarm_hr,&alarm_min,&trigger_type); //Find the nearest upcoming enabled alarm
        
        clcd_print("TIME ",LINE1(0));
        clcd_print((const char*)time,LINE1(5));
        
        if(alarm)
        {
            clcd_print("EVENT ",LINE2(0));
            if(alarm_hr==0)
            {
                disp_hr=12;
            }
            else if(alarm_hr>12)
            {
                disp_hr=alarm_hr-12;
            }
            else
            {
                disp_hr=alarm_hr;
            }
            clcd_putch((disp_hr/10)+'0',LINE2(6));
            clcd_putch((disp_hr%10)+'0',LINE2(7));
            clcd_putch(':',LINE2(8));
            clcd_putch((alarm_min/10)+'0',LINE2(9));
            clcd_putch((alarm_min%10)+'0',LINE2(10));
            clcd_putch(' ',LINE2(11));
            
            if(alarm_hr>=12)
            {
                clcd_putch('P',LINE2(12));
            }
            else
            {
                clcd_putch('A',LINE2(12));
            }
            clcd_putch('M', LINE2(13));
            clcd_putch(trigger_type,LINE2(15));
        }        
        else
        {
            clcd_print("EVENT--:-- --",LINE2(0));
        }
    }
}

unsigned char get_next_alarm(unsigned char *hour,unsigned char *min,unsigned char *trigger_type)
{
    alarm_t a;
    unsigned char i;
    unsigned char cur_hr,cur_min,cur_day;
    unsigned int now_total_min,alarm_total_min,days_away,wait_min;
    unsigned int best_wait = 0xFFFF;
    unsigned char found = 0;

    //Read current RTC time and convert from BCD to decimal
    cur_hr=((read_ds1307(HOUR_ADDR)>>4)&0x03)*10 + (read_ds1307(HOUR_ADDR)&0x0F);
    cur_min=((read_ds1307(MIN_ADDR)>>4)&0x07)*10 + (read_ds1307(MIN_ADDR)&0x0F);
    cur_day=(read_ds1307(DAY_ADDR)) & 0x07;
    now_total_min=(unsigned int)cur_hr*60 + cur_min;
    
    //Find the nearest enabled alarm
    for(unsigned char i=0;i<MAX_ALARM;i++)
    {
        read_alarm(i,&a);
        if(a.enable==0)
        {
            continue; //Skip disabled alarms
        }
        alarm_total_min=(unsigned int)a.hour*60 + a.min;

        //Calculate waiting time for weekly alarms
        if(a.trigger_type==TRIGGER_WEEKLY)
        {
            days_away=(a.day_of_week>=cur_day)?(a.day_of_week-cur_day):(7-cur_day+a.day_of_week);
    
            if(days_away==0 && alarm_total_min<now_total_min)
            {
                days_away=7;
            }
            wait_min=days_away*1440+alarm_total_min-now_total_min;
        }
        else //Calculate remaining minutes until alarm triggers
        {
            wait_min=(alarm_total_min>=now_total_min)?(alarm_total_min-now_total_min):(alarm_total_min-now_total_min+1440);
        }
        //Store the nearest upcoming alarm
        if(wait_min<best_wait)
        {
            best_wait=wait_min;
            *hour=a.hour;
            *min=a.min;
            *trigger_type=a.trigger_type;
            found=1;
        }
    }
    return found;
}

//Read one alarm record from EEPROM into the alarm structure
static void read_alarm(unsigned char index,alarm_t *alrm)
{
    unsigned char addr = ALARM_BASE_ADDR + (index*ALARM_SIZE);
    alrm->hour=read_external_eeprom(addr+0);
    alrm->min=read_external_eeprom(addr+1);
    alrm->dur_hr=read_external_eeprom(addr+2);
    alrm->dur_min=read_external_eeprom(addr+3);
    alrm->dur_sec=read_external_eeprom(addr+4);
    alrm->trigger_type=read_external_eeprom(addr+5);
    alrm->day_of_week=read_external_eeprom(addr+6);
    alrm->enable=read_external_eeprom(addr+7);
}

//Initialize buzzer and alarm storage
void init_alarms(void)
{
    unsigned char i;
    BUZZER_DDR = 0;//output
    BUZZER = 0;//buzzer off
    //Initialize EEPROM only during the first power-up
    if(read_external_eeprom(EEPROM_INIT_FLAG_ADDR)!=EEPROM_INIT_MAGIC)
    {
        for(i=0;i<MAX_ALARM;i++)
        {
            delete_alarm(i);   //Disable alarm slot
        }
        write_external_eeprom(EEPROM_INIT_FLAG_ADDR, EEPROM_INIT_MAGIC);
    }
}

//Save alarm parameters into EEPROM
void save_alarm(unsigned char index,unsigned char hour,unsigned char min,unsigned char dur_hr,unsigned char dur_min,unsigned char dur_sec,unsigned char trigger_type,unsigned char day_of_week)
{
    unsigned char addr=ALARM_BASE_ADDR + (index*ALARM_SIZE);
    write_external_eeprom(addr+0,hour);
    write_external_eeprom(addr+1,min);
    write_external_eeprom(addr+2,dur_hr);
    write_external_eeprom(addr+3,dur_min);
    write_external_eeprom(addr+4,dur_sec);
    write_external_eeprom(addr+5,trigger_type);
    write_external_eeprom(addr+6,day_of_week);
    write_external_eeprom(addr+7,1);//enabled
}

//Disable the selected alarm
void delete_alarm(unsigned char index)
{
    unsigned char addr=ALARM_BASE_ADDR+(index*ALARM_SIZE);
    write_external_eeprom(addr+7,0);//disabled
}

void trigger_alarms(void)
{
    alarm_t a;
    unsigned char i;
    unsigned char cur_hr,cur_min,cur_day;
    static unsigned char last_trigg_min=0xFF;

    cur_hr=((read_ds1307(HOUR_ADDR)>>4)&0x03)*10+(read_ds1307(HOUR_ADDR)&0x0F);
    cur_min=((read_ds1307(MIN_ADDR)>>4)&0x07)*10+(read_ds1307(MIN_ADDR)&0x0F);
    cur_day=(read_ds1307(DAY_ADDR))&0x07;
    
    if(cur_min==last_trigg_min)
    {
        return; //Prevent multiple triggers within the same minute
    }
    
    for(i=0;i<MAX_ALARM;i++)
    {
        read_alarm(i,&a); //Read alarm configuration from EEPROM
        if(a.enable==0)
        {
            continue;
        }
        
        //Check whether current time matches alarm time
        if(a.hour==cur_hr && a.min==cur_min)
        {
            if(a.trigger_type==TRIGGER_ONCE)
            {
                BUZZER=1;
                alarm_remaining_secs=(unsigned int)a.dur_hr*3600+(unsigned int)a.dur_min*60+a.dur_sec; //Convert alarm duration into total seconds
                delete_alarm(i); //Disable one-time alarm after it rings
                last_trigg_min=cur_min;
            }
            else if(a.trigger_type==TRIGGER_DAILY)
            {
                BUZZER=1;
                alarm_remaining_secs=(unsigned int)a.dur_hr*3600+(unsigned int)a.dur_min*60+a.dur_sec;
                last_trigg_min=cur_min;
            }
            else if(a.trigger_type==TRIGGER_WEEKLY &&a.day_of_week==cur_day)
            {
               BUZZER=1;
               alarm_remaining_secs=(unsigned int)a.dur_hr*3600+(unsigned int)a.dur_min*60+a.dur_sec;
               last_trigg_min=cur_min; 
            }
        }
        
    }
}

unsigned char config_screen(unsigned char reset_flag,unsigned char key)
{
    static unsigned char menu_pos;
    unsigned char arrow=0;
    if(reset_flag==RESET_CONFIG)
    {
        menu_pos=0;
        return_time=5;
        clear_screen();
        arrow=1;
    }
    
    if(key==SW_UP && menu_pos>0)
    {
        menu_pos--;
        return_time=5;
        __delay_ms(200);
        arrow=1;
    }
    else if(key==SW_DOWN && menu_pos<1)
    {
        menu_pos++;
        return_time=5;
        __delay_ms(200);
        arrow=1;
    }
    if(return_time==0)
    {
        return RETURN_BACK;
    }
    if(arrow)
    {
        clcd_putch(' ',LINE1(0));
        clcd_putch(' ',LINE2(0));
        
        clcd_print("SET/VIEW ALARM ",LINE1(2));
        clcd_print("SET TIME/DATE  ",LINE2(2)); 
        if(menu_pos==0)
        {
            clcd_putch(0x7E, LINE1(0));
        }
        else
        {
            clcd_putch(0x7E,LINE2(0));
        }       
    }  
    return menu_pos; //0 = Set/View Alarm, 1 = Set Time/Date
    
}
unsigned char event_submenu_screen(unsigned char reset_flag, unsigned char key)
{
    static unsigned char menu_pos;
    unsigned char arrow=0;
    if(reset_flag==RESET_EVENT_SUBMENU)
    {
        menu_pos=0;
        return_time=5;
        clear_screen();  
        arrow=1;
    }
    
    //long press DOWN->go back to Config Screen
    if(key==SW_DOWN_L)
    {
        return RETURN_BACK;
    }
    
    if(key==SW_UP&&menu_pos>0)
    {
        menu_pos--;
        return_time=5;
        __delay_ms(200);
        arrow=1;
    }
    else if(key==SW_DOWN&&menu_pos<1)
    {
        menu_pos++;
        return_time=5;
        __delay_ms(200);
        arrow=1;
    }
    if(return_time==0)
    {
        return RETURN_BACK;
    }
    
    if(arrow)
    {
        clcd_putch(' ',LINE1(0));
        clcd_putch(' ',LINE2(0));
        clcd_print("SET EVENT      ",LINE1(2));
        clcd_print("VIEW EVENT     ",LINE2(2));

        if(menu_pos==0)
        {
           clcd_putch(0x7E, LINE1(0));
        }
        else
        {
           clcd_putch(0x7E, LINE2(0));
        }  
    }
    return menu_pos; //0=Set Event, 1=View Event
}

unsigned char set_view_alarms(unsigned char key,unsigned char reset_flag)
{
    static unsigned char index;
    alarm_t a;
    unsigned char count=0;
    unsigned char i,flag=0;
    unsigned char disp_hr;
    
    if(reset_flag==RESET_SET_VIEW_ALARMS)
    {
        index=0;
        clear_screen();
        flag=1;
    }
    
    //count how many alarms currently exist
    for(i=0;i<MAX_ALARM;i++)
    {
        read_alarm(i,&a);
        if(a.enable)
        {
            count++;
        }
    }
    
    if(count==0)
    {
        clcd_print("No Alarms Set   ",LINE1(0));
        clcd_print("                ",LINE2(0));
        __delay_ms(1000);
        return RETURN_BACK;
    }
    //scroll through indexes
    if(key==SW_RIGHT_L)
    {
        return RETURN_BACK;
    }
    if(key==SW_DOWN)
    {
        do
        {
            index=(index+1)%MAX_ALARM;
            read_alarm(index,&a);
        } while(a.enable==0);
        __delay_ms(200);
        flag=1;
    }
    else if(key==SW_UP)
    {
        do
        {
            index=(index==0)?(MAX_ALARM-1):(index-1);
            read_alarm(index,&a);
        } while(a.enable==0);
        __delay_ms(200);
        flag=1;
    }
    //Left key=delete this alarm
    else if(key==SW_LEFT)
    {
        delete_alarm(index);
        clcd_print("Alarm Deleted   ",LINE1(0));
        clcd_print("                ",LINE2(0));
        __delay_ms(1000);
        clear_screen();
        flag=1;
    }
    
    if(flag)
    {
    read_alarm(index,&a);
    disp_hr=(a.hour==0)?12 : (a.hour>12)?(a.hour-12) : a.hour;
    //Line1-Display alarm time
    clcd_putch('#', LINE1(0));// Alarm number symbol    
    clcd_putch('T', LINE1(2));
    clcd_putch('-', LINE1(4));
    
    clcd_putch((disp_hr/10)+'0',LINE1(6));
    clcd_putch((disp_hr%10)+'0',LINE1(7));
    clcd_putch(':',LINE1(8));
    clcd_putch((a.min/10)+'0',LINE1(9));
    clcd_putch((a.min%10)+'0',LINE1(10));
    clcd_putch(' ', LINE1(11));  
    
    if(a.hour>=12)
    {
        clcd_print("PM", LINE1(12));
    }
    else
    {
        clcd_print("AM", LINE1(12));
    }
    clcd_putch(0x7E,LINE1(15)); //arrow indicator
    
    //LINE2-Display alarm duration and trigger type
    clcd_putch(index+'0',LINE2(0));
    clcd_putch('D',LINE2(2));
    clcd_putch('-', LINE2(4));
    clcd_putch((a.dur_hr/10)+'0',LINE2(6));
    clcd_putch((a.dur_hr%10)+'0',LINE2(7));
    clcd_putch(':',LINE2(8));
    
    clcd_putch((a.dur_min/10)+'0',LINE2(9));
    clcd_putch((a.dur_min%10)+'0',LINE2(10));
    clcd_putch(':',LINE2(11));
    
    clcd_putch((a.dur_sec/10)+'0',LINE2(12));
    clcd_putch((a.dur_sec%10)+'0',LINE2(13));
    clcd_putch(a.trigger_type,LINE2(15));
    }
    
    return RETURN_SUCCESS;
}

unsigned char edit_alarm(unsigned char key,unsigned char reset_flag)
{
    static unsigned char hour,min,dur_hr,dur_min,dur_sec,trigger_type,day_of_week,am_pm;
    static unsigned char field; //0=hr,1=min,2=dur_hr,3=dur_min,4=dur_sec,5=trigger_type
    static unsigned char slot;
    
    if(reset_flag==RESET_EDIT_ALARM)
    {
        am_pm=1;
        hour=12;
        min=0;
        dur_hr=0;
        dur_min=30;
        dur_sec=0;
        trigger_type=TRIGGER_ONCE;
        day_of_week=1;
        field=0;
        alarm_t temp;
        unsigned char i;
        
        //Find the first free EEPROM slot for storing a new alarm
        for(i = 0;i<MAX_ALARM;i++)
        {
            read_alarm(i,&temp);
            if(temp.enable==0)
            {
                slot = i; // first empty slot
                break;
            }
        }
        
        blink_t=1;
        TMR2ON=1;
        clear_screen();
        clcd_print("TIME-           ",LINE1(0));
        clcd_print("DUR -           ",LINE2(0));
        return RETURN_SUCCESS;
    }
    
    //display editable fields with blinking cursor
    //Line1: TIME- HH:MM 
    if(field==0&&blink_t==0)
    {
        clcd_print("  ",LINE1(5));
    }
    else
    {
        clcd_putch((hour/10)+'0',LINE1(5));
        clcd_putch((hour%10)+'0',LINE1(6));
    }
    clcd_putch(':',LINE1(7));

    if(field==1&&blink_t==0)
    {
        clcd_print("  ",LINE1(8));
    }
    else
    {
        clcd_putch((min/10)+'0',LINE1(8));
        clcd_putch((min%10)+'0',LINE1(9));
    }
    
    if(field==2&&blink_t==0)
    {
        clcd_print("  ",LINE1(11));
    }
    else
    {
        if(am_pm==0)
        {
            clcd_print("AM",LINE1(11));            
        }  
        else
        {
            clcd_print("PM",LINE1(11));            
        }
    }
    
     //Line2: DUR - HH:MM:SS
    if(field==3&&blink_t==0)
    {
        clcd_print("  ",LINE2(5));
    }
    else
    {
        clcd_putch((dur_hr/10)+'0',LINE2(5));
        clcd_putch((dur_hr%10)+'0',LINE2(6));
    }
    clcd_putch(':',LINE2(7));

    if(field==4&&blink_t==0)
    {
        clcd_print("  ",LINE2(8));
    }
    else
    {
        clcd_putch((dur_min/10)+'0',LINE2(8));
        clcd_putch((dur_min%10)+'0',LINE2(9));
    }
    clcd_putch(':',LINE2(10));
    
    if(field==5&&blink_t==0)
    {
        clcd_print("  ",LINE2(11));
    }
    else
    {
        clcd_putch((dur_sec/10)+'0',LINE2(11));
        clcd_putch((dur_sec%10)+'0',LINE2(12));
    }
    
    
    if(field==6&&blink_t==0)
    {
        clcd_putch(' ',LINE2(15));
    }
    else
    {
        clcd_putch(trigger_type,LINE2(15));
    }
    
    //Left/Right move between fields
    if(key==SW_RIGHT)
    {
        field=(field+1)%7; 
        __delay_ms(200);
    }
    else if(key==SW_LEFT)
    {
        field=(field==0)?6:(field-1);
        __delay_ms(200);
    }
    //Up key increments whichever field is selected
    else if(key==SW_UP)
    {
        switch(field)
        {
            case 0:
            {
                hour=(hour==12)?1:(hour+1);
                break;
            }
            case 1:
            {
                min=(min+1)%60;
                break;
            }
            case 2:
            {
                am_pm=!am_pm;
                break;
            }
            case 3:
            {
                dur_hr=(dur_hr+1)%24;
                break;
            }
            case 4:
            {
                dur_min=(dur_min+1)%60;
                break;
            }
            case 5:
            {
                dur_sec=(dur_sec+1)%60;
                break;
            }
            case 6:
            {
            if(trigger_type==TRIGGER_ONCE)
            {
                trigger_type=TRIGGER_DAILY;
            }
            else if(trigger_type==TRIGGER_DAILY)
            {
                trigger_type=TRIGGER_WEEKLY;
            }
            else
            {
                trigger_type=TRIGGER_ONCE;
            }
            break;
            }
        }
        __delay_ms(200);
    }
    //Down key decrements whichever field is selected
    else if(key==SW_DOWN)
    {
        switch(field)
        {
            case 0:
            {
                hour=(hour==0)?12:(hour-1);
                break;
            }
            case 1:
            {
                min=(min==0)?59:(min-1);
                break;
            }
            case 2:
            {
                am_pm=!am_pm;
                break;
            }
            case 3:
            {
                dur_hr=(dur_hr==0)?23:(dur_hr-1);
                break;
            }
            case 4:
            {
                dur_min=(dur_min==0)?59:(dur_min-1);
                break;
            }
            case 5:
            {
                dur_sec=(dur_sec==0)?59:(dur_sec-1);
                break;
            }
            case 6:
            {
            if(trigger_type==TRIGGER_ONCE)
            {
                trigger_type=TRIGGER_WEEKLY;
            }
            else if(trigger_type==TRIGGER_DAILY)
            {
                trigger_type=TRIGGER_ONCE;
            }
            else
            {
                trigger_type=TRIGGER_DAILY;
            }
            break;
            }
        }
        __delay_ms(200);
    }
    //long press UP = save and go back
    else if(key==SW_UP_L)
    {
        unsigned char hour24;
        if(am_pm==0) //am
        {
            hour24=(hour==12)?0:hour;
        }
        else //pm
        {
           hour24=(hour==12)?12:(hour+12); 
        }
        save_alarm(slot,hour24,min,dur_hr,dur_min,dur_sec,trigger_type,day_of_week);
        clear_screen();
        clcd_print("Alarm Saved!    ",LINE1(2));
        clcd_print("                ",LINE2(0));
        __delay_ms(1000);        
       // TMR2ON=0;
        return RETURN_BACK;
    }

    return RETURN_SUCCESS;
}


unsigned char time_date_submenu_screen(unsigned char reset_flag, unsigned char key)
{
    static unsigned char menu_pos;
    unsigned char arrow=0;
    if(reset_flag==RESET_TIME_DATE_SUBMENU)
    {
        menu_pos=0;
        return_time=5;
        clear_screen();
        arrow=1;
    }

    if(key==SW_DOWN_L)
    {
        return RETURN_BACK;
    }

    if(key==SW_UP && menu_pos>0)
    {
        menu_pos--;
        return_time=5;
        __delay_ms(200);
        arrow=1;
    }
    else if(key==SW_DOWN&&menu_pos<1)
    {
        menu_pos++;
        return_time=5;
        __delay_ms(200);
        arrow=1;
    }

    if(return_time==0)
    {
        return RETURN_BACK;
    }

    if(arrow)
    {
    clcd_putch(' ',LINE1(0));
    clcd_putch(' ',LINE2(0));
    clcd_print("SET TIME       ",LINE1(2));
    clcd_print("SET DATE       ",LINE2(2));

    if(menu_pos==0)
    {
        clcd_putch(0x7E, LINE1(0));
    }
    
    else
    {
        clcd_putch(0x7E, LINE2(0));
    }
    }

    return menu_pos;//0=Set Time, 1=Set Date
}

//edit RTC time
unsigned char set_time_screen(unsigned char key, unsigned char reset_flag)
{
    static unsigned char field;
    static unsigned char hr,min,sec;
    
    if(reset_flag==RESET_SET_TIME_SCREEN)
    {     
        hr=((read_ds1307(HOUR_ADDR)>>4)&0x03)*10+(read_ds1307(HOUR_ADDR)&0x0F);
        min=((read_ds1307(MIN_ADDR)>>4)&0x07)*10+(read_ds1307(MIN_ADDR)&0x0F);
        sec=((read_ds1307(SEC_ADDR)>>4)&0x07)*10+(read_ds1307(SEC_ADDR)&0x0F);
        
        field=0;  //hours blinks first
        blink_t=1;
        TMR2ON=1;
        clear_screen();
        clcd_print("HH:MM:SS        ",LINE1(4));
        clcd_print("                ",LINE2(0));
        return RETURN_SUCCESS;
    }
    //HH:MM:SS 
    if(field==0&&blink_t==0)
    {
        clcd_print("  ",LINE2(4));
    }
    else
    {
        clcd_putch((hr/10)+'0',LINE2(4));
        clcd_putch((hr%10)+'0',LINE2(5));
    }
    clcd_putch(':',LINE2(6));

    if(field==1&&blink_t==0)
    {
        clcd_print("  ",LINE2(7));
    }
    else
    {
        clcd_putch((min/10)+'0',LINE2(7));
        clcd_putch((min%10)+'0',LINE2(8));
    }
    clcd_putch(':',LINE2(9));

    if(field==2&&blink_t==0)
    {
        clcd_print("  ",LINE2(10));
    }
    else
    {
        clcd_putch((sec/10)+'0',LINE2(10));
        clcd_putch((sec%10)+'0',LINE2(11));
    }
    
     if(key==SW_RIGHT)
    {
        field=(field+1)%3;
        __delay_ms(200);
    }
    else if(key==SW_LEFT)
    {
        field=(field==0)?2:(field-1);
        __delay_ms(200);
    }
    
    else if(key==SW_UP)
    {
        switch(field)
        {
            case 0:
                hr=(hr+1)%24; 
                break;
            case 1: 
                min=(min+1)%60;
                break;
            case 2:
                sec=(sec+1)%60; 
                break;
        }
        __delay_ms(200);
    }
    
    else if(key==SW_DOWN)
    {
        switch(field)
        {
            case 0:
                hr=(hr==0)?23:(hr-1); 
                break;
            case 1: 
                min=(min==0)?59:(min-1);
                break;
            case 2:
                sec=(sec==0)?59:(sec-1); 
                break;
        }
        __delay_ms(200);
    }
    // Save updated time into RTC and return to Time/Date Menu
    else if(key==SW_UP_L)
    {
        write_ds1307(HOUR_ADDR,((hr/10)<<4)|(hr%10));
        write_ds1307(MIN_ADDR,((min/10)<<4)|(min%10));
        write_ds1307(SEC_ADDR,((sec/10)<<4)|(sec%10));
        clcd_print("Time Set!       ",LINE1(3));
        clcd_print("                ",LINE2(0));
        __delay_ms(1000);
        //TMR2ON=0;
        return RETURN_BACK;//back to Time/Date submenu
    }
    return RETURN_SUCCESS;   
}

//Edit RTC date, Updates DS1307 RTC with new values
unsigned char set_date_screen(unsigned char key,unsigned char reset_flag)
{
    static unsigned char field;
    static unsigned char dd,mon,yr;
    
    if(reset_flag==RESET_SET_DATE_SCREEN)
    {
        dd=((read_ds1307(DATE_ADDR)>>4)&0x03)*10+(read_ds1307(DATE_ADDR)&0x0F);
        mon=((read_ds1307(MONTH_ADDR)>>4)&0x07)*10+(read_ds1307(MONTH_ADDR)&0x0F);
        yr=((read_ds1307(YEAR_ADDR)>>4)&0x07)*10+(read_ds1307(YEAR_ADDR)&0x0F);
        
        field=0;  //hours blinks first
        blink_t=1;
        TMR2ON=1;
        clear_screen();
        clcd_print("DD-MM-YY        ",LINE1(4));
        clcd_print("                ",LINE2(0));
        return RETURN_SUCCESS;
    }
  
    if(field==0&&blink_t==0)
    {
        clcd_print("  ",LINE2(4));
    }
    else
    {
        clcd_putch((dd/10)+'0',LINE2(4));
        clcd_putch((dd%10)+'0',LINE2(5));
    }
    clcd_putch('-',LINE2(6));

    if(field==1&&blink_t==0)
    {
        clcd_print("  ",LINE2(7));
    }
    else
    {
        clcd_putch((mon/10)+'0',LINE2(7));
        clcd_putch((mon%10)+'0',LINE2(8));
    }
    clcd_putch('-',LINE2(9));

    if(field==2&&blink_t==0)
    {
        clcd_print("  ",LINE2(10));
    }
    else
    {
        clcd_putch((yr/10)+'0',LINE2(10));
        clcd_putch((yr%10)+'0',LINE2(11));
    }
    
     if(key==SW_RIGHT)
    {
        field=(field+1)%3;
        __delay_ms(200);
    }
    else if(key==SW_LEFT)
    {
        field=(field==0)?2:(field-1);
        __delay_ms(200);
    }
    
    else if(key==SW_UP)
    {
        switch(field)
        {
            case 0:
                dd=(dd==31)?1:(dd+1); 
                break;
            case 1: 
                mon=(mon==12)?1:(mon+1);
                break;
            case 2:
                yr=(yr==99)?0:(yr+1); 
                break;
        }
        __delay_ms(200);
    }
    
    else if(key==SW_DOWN)
    {
        switch(field)
        {
            case 0:
                dd=(dd==1)?31:(dd-1); 
                break;
            case 1: 
                mon=(mon==1)?12:(mon-1);
                break;
            case 2:
                yr=(yr==0)?99:(yr-1); 
                break;
        }
        __delay_ms(200);
    }
    // Save updated time into RTC and return to Time/Date Menu
    else if(key==SW_UP_L)
    {
        write_ds1307(DATE_ADDR,((dd/10)<<4)|(dd%10));
        write_ds1307(MONTH_ADDR,((mon/10)<<4)|(mon%10));
        write_ds1307(YEAR_ADDR,((yr/10)<<4)|(yr%10));
        clcd_print("Date Set!       ",LINE1(3));
        clcd_print("                ",LINE2(0));
        __delay_ms(1000);
       // TMR2ON=0;
        return RETURN_BACK;//back to Time/Date submenu
    }
    return RETURN_SUCCESS;   
    
}
