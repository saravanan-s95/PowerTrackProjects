#include<xc.h>
#include"main.h"
unsigned char time[9];
unsigned char clock_reg[3];
extern unsigned short sub_mode;
void display_time(void)
{
	clcd_print(time, LINE2(6));

	if (clock_reg[0] & 0x40)
	{
		if (clock_reg[0] & 0x20)
		{
			clcd_print("PM", LINE2(14));
		}
		else
		{
			clcd_print("AM", LINE2(14));
		}
	}
}

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}
void set_log()
{
    unsigned char bcd[60] = {0x00,0x01, 0x02, 0x03, 0x04, 0x05,0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
    0x16, 0x17, 0x18, 0x19, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25,
    0x26, 0x27, 0x28, 0x29, 0x30,
    0x31, 0x32, 0x33, 0x34, 0x35,
    0x36, 0x37, 0x38, 0x39, 0x40,
    0x41, 0x42, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x48, 0x49, 0x50,
    0x51, 0x52, 0x53, 0x54, 0x55,
    0x56, 0x57, 0x58, 0x59, 0x60
};
   unsigned char mode=0;
    int h=0,m=0,s=0;
    unsigned char str[9];
     unsigned int temp=0;
    while(1)
    {
        clcd_print("Set the RTC",LINE1(0));
        
        unsigned int key = read_switches(STATE_CHANGE);
        str[1]=(h%10)+'0';
        str[0]=((h/10)%10)+'0';
        str[2]=':';
        str[4]=(m%10)+'0';
        str[3]=((m/10)%10)+'0';
        str[5]=':';
        str[7]=(s%10)+'0';
        str[6]=((s/10)%10)+'0';
        str[8]='\0';
        //clcd_print(str,LINE2(0));
       
        if(SW11==key)
        {
            if(mode<3)
            {
                mode++;
                temp=0;
            }
        }
        if(mode==0)
        {
            
            if(SW1==key)
            {
                if(s<59)
                {
                    s++;
                   // sec++;
                }
            }
            else if(SW2==key)
            {
                if(s>0)
                {
                    s--;
                   // sec--;
                }
            }
            
        }
        else if(mode==1)
        {
            
            if(SW1==key)
            {
                if(m<59)
                {
                    m++;
                    //mi++;
                }
            }
            else if(SW2==key)
            {
                if(m>0)
                {
                    m--;
                    //mi--;
                }
            }

        }
        else if(mode==2)
        {
           
            if(SW1==key)
            {
                if(h<23)
                {
                    h++;
                    //hr++;
                }
            }
            else if(SW2==key)
            {
                if(h>0)
                {
                    h--;
                    //hr--;
                }
            }

        }
        if(mode==0)
        {
            if(temp++ <= 1000)
            {
                clcd_print(str,LINE2(0));
            }
            else if(temp <= 2000)
            {
                str[7]=' ';
                str[6]=' ';
                clcd_print(str,LINE2(0));
            }
            else
            {
                temp=0;
            }
        }
        else if(mode==1)
        {
            if(temp++ <= 1000)
            {
                clcd_print(str,LINE2(0));
            }
            else if(temp <= 2000)
            {
                str[4]=' ';
                str[3]=' ';
                clcd_print(str,LINE2(0));
            }
            else
            {
                temp=0;
            }
        }
        else if(mode==2)
        {
            if(temp++ <= 1000)
            {
                clcd_print(str,LINE2(0));
            }
            else if(temp <= 2000)
            {
                str[0]=' ';
                str[1]=' ';
                clcd_print(str,LINE2(0));
            }
            else
            {
                temp=0;
            }
        }
        else 
        {
            clcd_print(str,LINE2(0));
        }
        if(SW12==key)
        {
            edit_time(bcd[s],0x00);
            edit_time(bcd[m],0x01);
            edit_time(bcd[h],0x02);
            sub_mode--;
            return;
        }
    }
}