#include<xc.h>
#include"main.h"
unsigned char time[9];
unsigned char clock_reg[3];
unsigned char calender_reg[4];

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
unsigned char date[11];
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
void set_time()
{
    
   unsigned int mode=0;
    int h=0,m=0,s=0;
    unsigned char str[9];
     unsigned int temp=0,temp1=0,long_p=0,short_p=0,i=0,temp3=0;
    while(1)
    {
        clcd_print("Set the RTC",LINE1(0));
        
        unsigned int key = read_switches(LEVEL_CHANGE);
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
       
        if(key == SW1)
        {
            //for(int i = 50;i--;);
            if(temp++ == 200)
            {
                long_p=1;
                short_p=0;
                //if(mode<2)
                clcd_putch('s',LINE1(14));
                mode++;
                CLEAR_DISP_SCREEN;
            }
        }
        else
        {
            if(temp <200 && temp> 0)
            {
                short_p=1;
                if(mode==0)
                {
            
                    if(s<59)
                    {
                        s++;
                    }     
                }
                else if(mode==1)
                {
                    if(m<59)
                    {
                        m++;
                    //mi--;
                    }
                }
                else if(mode==2)
                {
                    if(h<23)
                    {
                        h++;
                    }
                }
            }
            temp=0;
            long_p=0;
        }
        if(key == SW2)
        {
            //for(int i = 50;i--;);
            if(temp1++ == 200)
            {
                long_p=1;
                short_p=0;
                
                edit_time(bcd[s],0x00);
                edit_time(bcd[m],0x01);
                edit_time(bcd[h],0x02);
                CLEAR_DISP_SCREEN;
                return;
            }
        }
        else 
        {
            if(temp1 <200 && temp1> 0)
            {
                short_p=1;
                if(mode==0)
                {
            
                    if(s>0)
                    {
                        s--;
                    }     
                }
                else if(mode==1)
                {
                    if(m>0)
                    {
                        m--;
                    //mi--;
                    }
                }
                else if(mode==2)
                {
                    if(h>0)
                    {
                        h--;
                    }
                }
            }
            temp1=0;
            long_p=0;
        }
        //clcd_putch(mode+'0',LINE2(13));
       if(mode==0)
        {
            if(temp3++ <= 1000)
            {
                clcd_print(str,LINE2(0));
            }
            else if(temp3 <= 2000)
            {
                str[7]=' ';
                str[6]=' ';
                clcd_print(str,LINE2(0));
            }
            else
            {
                temp3=0;
            }
        }
        else if(mode==1)
        {
            if(temp3++ <= 1000)
            {
                clcd_print(str,LINE2(0));
            }
            else if(temp3 <= 2000)
            {
                str[4]=' ';
                str[3]=' ';
                clcd_print(str,LINE2(0));
            }
            else
            {
                temp3=0;
            }
        }
        else if(mode==2)
        {
            if(temp3++ <= 1000)
            {
                clcd_print(str,LINE2(0));
            }
            else if(temp3 <= 2000)
            {
                str[0]=' ';
                str[1]=' ';
                clcd_print(str,LINE2(0));
            }
            else
            {
                temp3=0;
            }
        }
        else 
        {
            clcd_print(str,LINE2(0));
        }
        
    }
}

void get_date(void)
{
	calender_reg[0] = read_ds1307(YEAR_ADDR);
	calender_reg[1] = read_ds1307(MONTH_ADDR);
	calender_reg[2] = read_ds1307(DATE_ADDR);
	calender_reg[3] = read_ds1307(DAY_ADDR);
    date[6]='2';
    date[7]='0';
	date[8] = '0' + ((calender_reg[0] >> 4) & 0x0F);
	date[9] = '0' + (calender_reg[0] & 0x0F);
	date[5] = '-';
	date[3] = '0' + ((calender_reg[1] >> 4) & 0x0F);
	date[4] = '0' + (calender_reg[1] & 0x0F);
	date[2] = '-';
	date[0] = '0' + ((calender_reg[2] >> 4) & 0x0F);
	date[1] = '0' + (calender_reg[2] & 0x0F);
	date[10] = '\0';
}
void set_date()
{
    unsigned char key;
    int temp=0,temp1=0,long_p=0,short_p=0,mode=0,i=0,year=0,month=0,d=0;
    unsigned char str[10];
    while(1)
    {
        str[9]=(year%10)+'0';
        str[8]=(year/10)+'0';
        
        str[7]='0';
        str[6]='2';
        str[5]='-';
        str[4]=(month%10)+'0';
        str[3]=(month/10)+'0';
        str[2]='-';
        str[1]=(d%10)+'0';
        str[0]=(d/10)+'0';
        str[10]='\0';
        clcd_print("set the date",LINE1(0));
        clcd_print(str,LINE2(0));
        
        key=read_switches(LEVEL_CHANGE);
        if(key == SW1)
        {
            if(temp++ == 300)
            {
                long_p=1;
                short_p=0;
                mode++;
                CLEAR_DISP_SCREEN;
            }
        }
        else
        {
            if(temp <200 && temp> 0)
            {
                short_p=1;
                if(mode==0)
                {
            
                    if(year<99)
                    {
                        year++;
                    }     
                }
                else if(mode==1)
                {
                    if(month<12)
                    {
                        month++;
                    }
                }
                else if(mode==2)
                {
                    if(d<31)
                    {
                        d++;
                    }
                }
            }
            temp=0;
            long_p=0;
        }
        if(key == SW2)
        {
            if(temp1++ == 300)
            { 
                CLEAR_DISP_SCREEN;
                edit_time(bcd[year],YEAR_ADDR);
                edit_time(bcd[month],MONTH_ADDR);
                edit_time(bcd[d],DATE_ADDR);
                return;
            }
        }
        else
        {
            if(temp1 <200 && temp1> 0)
            {
                short_p=1;
                CLEAR_DISP_SCREEN;
                if(mode==0)
                {
            
                    if(year>0)
                    {
                        year--;
                    }     
                }
                else if(mode==1)
                {
                    if(month>0)
                    {
                        month--;
                    }
                }
                else if(mode==2)
                {
                    if(d>0)
                    {
                        d--;
                    }
                }
            }
            temp1=0;
            long_p=0;
        }
        
        
        }
    
}
void set_time_and_date()
{
    int temp=0,temp1=0,long_p=0,short_p=0,mode=0,i=0;
    unsigned char key;
    while(1)
    {
        key=read_switches(LEVEL_CHANGE);
        if(key == SW1)
        {
            if(temp++ == 300)
            {
                long_p=1;
                short_p=0;
                mode++;
                CLEAR_DISP_SCREEN;
            }
        }
        else
        {
            if(temp <200 && temp> 0)
            {
                short_p=1;
                if(mode==0)
                {
                    if(i<1)
                        i++;
                }
                CLEAR_DISP_SCREEN;
            }
            temp=0;
            long_p=0;
        }
        if(key == SW2)
        {
            if(temp1++ == 300)
            { 
                CLEAR_DISP_SCREEN;
                return;
            }
        }
        else
        {
            if(temp1 <200 && temp1> 0)
            {
                short_p=1;
                if(mode==0)
                {
                    if(i>0)
                        i--;
                }
                CLEAR_DISP_SCREEN;
            }
            temp1=0;
            long_p=0;
        }
//        if(SW1==key &&short_p==1)
//        {
//            if(i<1)
//               i++;
//        }
//        else if((SW2==key && short_p==1))
//        {
//            if(i>0)
//              i--;
//        }
        clcd_print("set date        ",LINE2(3));
        clcd_print("set time        ",LINE1(3));
        if(i==0 && short_p==1)
        {
            clcd_print("=>",LINE1(0));
            clcd_print("  ",LINE2(0));
        }
        else if(i==1 && short_p==1)
        {
            clcd_print("=>",LINE2(0));
            clcd_print("  ",LINE1(0));
        }
        else
        {
            clcd_print("=>",LINE1(0));
            clcd_print("  ",LINE2(0));
        }
        clcd_putch(mode+'0',LINE2(15));
        if(mode==1)
        {
            switch(i)
            {
                case 0:set_time();
                mode--;
                break;
                case 1:set_date();
                mode--;
            }
        }
        
        
        
    }
}