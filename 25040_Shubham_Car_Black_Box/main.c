
#include <xc.h>
#include "main.h"
int count,front,rear=-1;
unsigned char add[]={0x03,0x13,0x23,0x33,0x43,0x53,0x63,0x73,0x83,0x93,0xa3};
extern unsigned char time[9];
unsigned short read_gear(unsigned short gear,unsigned char sw);
unsigned char *arr[]={"View log        ","Download log    ","Set log        ","Clear log       "};
void init_con()
{
    init_adc();
    init_clcd();
    init_i2c();
    init_matrix_keypad();
    init_ds1307();
    init_con_uart();
}
unsigned char str[5];
unsigned short speed=0,gear,sub_mode=0;
int i=0;
void main(void) {
    init_con();
    if((read_external_eeprom(0x01))=='1')
    {
        rear=read_external_eeprom(0x02)-'0';
        count=read_external_eeprom(0x00)-'0';
    }
    unsigned short temp=0,flag=0,temp2=0,flag1=0;
    unsigned char key;
    while(1)
    {
        key=read_switches(STATE_CHANGE);
        get_time();
       

        if(SW11==key)
        {
            
            sub_mode++;
            temp2=0,flag1=0;
            
        }
       
        if(sub_mode==0)
        {
            
            sub_mode=0;
            speed = (read_adc(CHANNEL4)/10.23);
            if(gear==0)
            {
                speed=0;
            }
            else if(gear==1 && speed>30)
            {
                speed=30;
            }
            else if(gear ==2 && speed>45)
            {
                speed=45;
            }
            else if(gear==3 && speed>75)
            {
                speed=75;
            }
            else if(gear==6 && speed>40)
            {
                speed=40;
            }
            if(SW1==key || SW2==key || SW3)
            {
                gear=read_gear(gear,key);
            }
            clcd_print("SPD",LINE1(0));
            //my_atoi(speed);
            str[0]=(speed/100)+'0';
            str[1]=((speed/10)%10)+'0';
            str[2]=(speed%10)+'0';
            str[3]='\0';
            print_gear(gear);
            clcd_print(str,LINE2(0));
            clcd_putch('G', LINE1(4));
            clcd_print("RTC",LINE1(9));
            display_time();
            
            
        }
        else
        {
            if(SW1==key && sub_mode==1)
            {
                if(i<3)
                {
                    i++;
                    if(temp<3)
                        temp++;
                }
//                if(temp<3)
//                  temp++;
                temp2=0;
                if(temp==1)
                {
                    i--;
                }
            }
            else if(SW2==key && sub_mode==1)
            {
                if(i>0)
                {
                    i--;
                    
                    if(temp2<3)
                        temp2++;
                }
                temp=0;
                if(temp2==1)
                {
                    i++;
                }
            }
            
            if(temp==3)
            {
                clcd_print("   ",LINE1(0));
                clcd_print("-> ",LINE2(0));
                clcd_print(arr[temp-1],LINE1(3));
                clcd_print(arr[temp],LINE2(3));
                clcd_print("   ",LINE1(0));
                
            }
            if(temp2==1 && i==3)
            {
                clcd_print("   ",LINE2(0));
                clcd_print("-> ",LINE1(0));
                clcd_print(arr[i-1],LINE1(3));
                clcd_print(arr[i],LINE2(3));
                clcd_print("   ",LINE1(0));
                i--;
            }
            else if(temp>=1 && temp<3)
            {
                clcd_print("   ",LINE1(0));
                clcd_print("-> ",LINE2(0));
                clcd_print(arr[i],LINE1(3));
                clcd_print(arr[i+1],LINE2(3));
                clcd_print("   ",LINE1(0));
                //temp++;
            }
            else if(temp2>=1)
            {
                clcd_print("   ",LINE2(0));
                clcd_print("-> ",LINE1(0));
                clcd_print(arr[i],LINE1(3));
                clcd_print(arr[i+1],LINE2(3));
                clcd_print("   ",LINE1(0));
                //temp2++;
            }
            if(temp==0 && temp2==0)
            {
                clcd_print("   ",LINE2(0));
                clcd_print("-> ",LINE1(0));
                clcd_print(arr[i],LINE1(3));
                clcd_print(arr[i+1],LINE2(3));
                clcd_print("   ",LINE1(0));
            }
            
            if(sub_mode==2)
            {
                if(temp<=3 && temp>0)
                   sub_node(temp);
                else
                    sub_node(i);
            }
        }
        if(SW12==key)
        {
            if(sub_mode>0)
            {
                sub_mode--;
                CLEAR_DISP_SCREEN;
            }
        }
    }
    return;
}

unsigned short read_gear(unsigned short gear,unsigned char sw)
{
    int flag=0;
    if(SW1==sw && gear<6)
    {
            gear++;
            flag=1;
    }
    if(SW2 ==sw && gear>0)
    {
        if(gear == 6)
        {
            gear=1;
            flag=0;
        }
        else
        {
            gear--;  
            flag=1;
        }
    }
    int cl=0;
    if(SW3==sw)
    {
        cl=1;
        flag=1;
    }
    if(flag==1)
    {
        write_external_eeprom(0x00, count+'0');
        write_external_eeprom(0x01, '1');
        write_external_eeprom(0x02, rear+'0');
        rear=(rear+1)%10;
        unsigned char ch = add[rear];
    int i=0,j=0;
    while(str[i])
    {
        write_external_eeprom(ch+j, str[i]);
        i++;
        j++;
    }
    
    write_external_eeprom(ch+j,' ');
    j++;
    if(gear==6)
    {
        write_external_eeprom(ch+j, 'R');
        j++;
    }
    else if(gear==0)
    {
        write_external_eeprom(ch+j, 'N');
        j++;
    }
    else
    {
        write_external_eeprom(ch+j, gear+'0');
        j++;
    }
    i=0;
    write_external_eeprom(ch+j,' ');
    j++;
    while(time[i])
    {
        write_external_eeprom(ch+j, time[i]);
        i++;
        j++;
    }
    if(cl==1)
    {
        write_external_eeprom(ch+j, 'C');
        j++;
    }
    else 
    {
        write_external_eeprom(ch+j, ' ');
        j++;
    }
    count++;
    }
    return gear;
}
