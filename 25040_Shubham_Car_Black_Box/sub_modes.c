/*
 * File:   sub_modes.c
 * Author: 91902
 *
 * Created on 4 July, 2026, 9:11 PM
 */
#include <xc.h>
#include "main.h"
#include"uart.h"
extern unsigned char add[];
extern int count,rear;
static unsigned char str[16]={'0'};
void view_entrys(unsigned char add);
extern unsigned short sub_mode;
void clear_log();
void download_log();
int sub_node(unsigned short mode)
{
    CLEAR_DISP_SCREEN;
    unsigned char ptr[16];
    unsigned int temp=0,i=0;
    switch(mode)
    {
        case 0:view_entrys(add[i++]);
        while(1)
        {
            clcd_print("SPD G  RTC",LINE1(0));
            unsigned char key=read_switches(STATE_CHANGE);
            if(count==0)
            {
                clcd_print("No log",LINE2(0));
            }
            else
            {
            if(key==SW1)
            {
                if(i<count)
                    view_entrys(add[i++]);
               
            }
            else if(SW2==key)
            {
                if(i>0)
                    view_entrys(add[--i]);
            }
            
            clcd_print(str,LINE2(0));
            }
            if(SW12==key)
            {
                sub_mode--;
                return 0;
            }   
        }
        case 3:clear_log();
        
        clcd_print("log Cleared",LINE1(4));
        for(unsigned long int i=0xfffff;i--;);
        sub_mode--;
        return 0;
        case 1:download_log();
        
        clcd_print("log Download successful",LINE1(0));
        for(unsigned long int i=0xfffff;i--;);
        sub_mode--;
            return 0;
        case 2:set_log();
        return 0;
    
    }
}
void view_entrys(unsigned char add)
{
    
    for(int j=0;j<15;j++)
    {
        str[j]=read_external_eeprom(add+j);
    }
    str[15]='\0';
}
void clear_log()
{
    
    count=0;
    rear=-1;
    write_external_eeprom(0x00, count+'0');
    write_external_eeprom(0x02, rear+'0');
}
void download_log()
{
    string_transmit_uart("SPD G   RTC\n\r");
    if(count==0)
    {
        string_transmit_uart("No log to download\n\r");
    }
    for(int i=0;i<count;i++)
    {
        view_entrys(add[i]);
        char_transmit_uart('\n');
        char_transmit_uart('\r');
        
        string_transmit_uart(str);
        if(str[14]=='C')
        {
            
            string_transmit_uart("  Collision");
        }
        
    }
}