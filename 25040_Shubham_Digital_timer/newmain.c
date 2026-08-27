
#include <xc.h>
#include"main.h"
extern unsigned char add[];
extern unsigned char date[11];
extern unsigned char time[9];
extern int count1;
unsigned int sec;
extern int rear;
extern unsigned char alarm[10];
void init_con() //configuration of all peripheral
{
    
    init_clcd();
    init_i2c();
    init_matrix_keypad();
    init_ds1307();
   // timer2();
    
}
void main(void) {
    CLEAR_DISP_SCREEN;
    init_con();
    unsigned char key;
    unsigned int mode=0,long_p=0,short_p=0,temp=0,temp1,i=0,f1=0,f2=0;
    if((read_external_eeprom(0x02))=='1') //extra validation
    {
        rear=read_external_eeprom(0x01)-'0';
        count1=read_external_eeprom(0x00)-'0';
    }
    int flag=0;
    while(1)
    {
        key=read_switches(LEVEL_CHANGE);     
        if(key == SW1)  
        {
            if(temp++ == 300) //long press
            {
                long_p=1;
                short_p=0;
                mode++;
                f1=1;
                CLEAR_DISP_SCREEN;
            }
        }
        else
        {
            if(temp <300 && temp> 0 && f1==0) //short press
            {
                short_p=1;
                if(mode==1)
                {
                    if(i<1)
                        i++;
                }
                CLEAR_DISP_SCREEN;
            }
            temp=0;
            long_p=0;
            f1=0;
        }
        if(key == SW2)
        {
            if(temp1++ == 300)
            {
                long_p=1;
                short_p=0;
                mode--;
                f2=1;
                CLEAR_DISP_SCREEN;
            }
        }
        else
        {
            if(temp1 <300 && temp1> 0 && f2==0)
            {
                short_p=1;
                if(mode==1)
                {
                    if(i>0)
                        i--;
                }
                CLEAR_DISP_SCREEN;
            }
            f2=0;
            temp1=0;
            long_p=0;
        }
        if(mode==0)
        {
            get_time();
            get_date();
            clcd_print("Date ",LINE1(0));
            clcd_print(date,LINE1(5));
            clcd_print("Time ",LINE2(0));
            clcd_print(time,LINE2(5));
        }
        
        else
        {
            
            clcd_print("set date/time",LINE2(3));
            clcd_print("set/View event",LINE1(3));

            if(i==0 && mode==1)
            {
                clcd_print("=>",LINE1(0));
                clcd_print("  ",LINE2(0));
            }
            else if(i==1 && mode==1)
            {
                clcd_print("=>",LINE2(0));
                clcd_print("  ",LINE1(0));
            }
            if(mode==2) 
            {
                switch(i)
                {
                    case 0:CLEAR_DISP_SCREEN;
                        select_view_or_set_event(); //set the event
                    mode--;
                        break;
                    case 1:CLEAR_DISP_SCREEN;
                        set_time_and_date(); //set the RTC
                    mode--;
                }
            }
        }
        
    }
    return;
}
