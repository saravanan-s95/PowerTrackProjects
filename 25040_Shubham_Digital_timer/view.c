#include "main.h"
#include<xc.h>
void view_entrys(unsigned char add);

extern unsigned char date[11];
event arr[10];
int count1 = 0;
int rear = -1;
unsigned char add[] = {0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xa3};

void set_event() {
    unsigned char str[10];
    int h = 0, m = 0;
    int temp = 0, temp1 = 0, long_p = 0, short_p = 0, mode = 0, run = 0;
    while (1) {
        clcd_print("Set the Alarm", LINE1(0));

        unsigned int key = read_switches(LEVEL_CHANGE);
        str[1] = (h % 10) + '0';
        str[0] = ((h / 10) % 10) + '0';
        str[2] = ':';
        str[4] = (m % 10) + '0';
        str[3] = ((m / 10) % 10) + '0';
        //str[4] = ' ';
        str[5] = ' ';
        str[6] = ' ';
        str[7] = ' ';

        str[9] = '\0';
        clcd_print(str, LINE2(0));
        if (run == 0) {
            
            str[8] = 'O';
        } else if (run == 1) {
           
            str[8] = 'D';
            ;
        } else if (run == 2) {
            
            str[8] = 'W';
        }
        if (key == SW1) {
            //for(int i = 50;i--;);
            if (temp++ == 200) {
                long_p = 1;
                short_p = 0;
                if (mode < 2)
                    mode++;

                CLEAR_DISP_SCREEN;
            }
        } else {
            if (temp < 200 && temp > 0) {
                short_p = 1;

                if (mode == 0) {
                    if (m < 59) {
                        m++;
                        //mi--;
                    }
                } else if (mode == 1) {
                    if (h < 23) {
                        h++;
                    }
                } else if (mode == 2) {
                    if (run < 2)
                        run++;
                }
            }
            temp = 0;
            long_p = 0;
        }
        if (key == SW2) {
            //for(int i = 50;i--;);
            if (temp1++ == 200) {
                long_p = 1;
                short_p = 0;
                int j = 0;
                write_external_eeprom(count1 + '0', 0x00);
                write_external_eeprom(rear + '0', 0x01);
                write_external_eeprom('1', 0x02);
                rear = (rear + 1) % 10;
                unsigned char ch = add[rear];
                while (str[j]) {
                    write_external_eeprom(ch + j, str[j]);
                    j++;
                }
                count1++;
                CLEAR_DISP_SCREEN;
                return;
            }
        } else {
            if (temp1 < 200 && temp1 > 0) {
                if (mode == 0) {
                    if (m > 0) {
                        m--;
                        //mi--;
                    }
                } else if (mode == 1) {
                    if (h > 0) {
                        h--;
                    }
                } else if (mode == 2) {
                    if (run > 0)
                        run--;
                }
            }
            temp1 = 0;
            long_p = 0;
        }
    }

}
unsigned char alarm[10];
void view() {
   
    int temp = 0, temp1 = 0, long_p = 0, short_p = 0, mode = 0, i = 0; //j = 0;
    unsigned char key;
    view_entrys(add[i++]);
    while (1) {
        
        key = read_switches(LEVEL_CHANGE);
        clcd_print(alarm,LINE2(0));
        if (key == SW1) {
            if (temp++ == 200) {
                long_p = 1;
                short_p = 0;
                mode++;
                CLEAR_DISP_SCREEN;
            }
        } else {
            if (temp < 200 && temp > 0) {
                short_p = 1;
                if (i < count1)
                    view_entrys(add[i++]);
                CLEAR_DISP_SCREEN;
            }
            temp = 0;
            long_p = 0;
        }
        if (key == SW2) {
            if (temp1++ == 200) {
                    return;
                
                CLEAR_DISP_SCREEN;

            }
        } else {
            if (temp1 < 200 && temp1 > 0) {
                short_p = 1;
                if (i > 0)
                    view_entrys(add[--i]);
                CLEAR_DISP_SCREEN;
            }
            temp1 = 0;
            long_p = 0;
        }
        clcd_print("ALARMS           ", LINE1(0));
        
        if (count1 == 0) {
            clcd_print("No ALARM         ", LINE2(0));
        } 
        
    }
}

void select_view_or_set_event() {
    int temp = 0, temp1 = 0, long_p = 0, short_p = 0, mode = 0, i = 0, j = 0;
    unsigned char key;
    while (1) {
        key = read_switches(LEVEL_CHANGE);
        if (key == SW1) {
            if (temp++ == 200) {
                long_p = 1;
                short_p = 0;
                mode++;
                CLEAR_DISP_SCREEN;
            }
            short_p=0;
        } else {
            if (temp < 200 && temp > 0) {
                short_p = 1;
                if(mode==0)
                {
                    if(i<1)
                        i++;
                }
                CLEAR_DISP_SCREEN;
            }
            temp = 0;
            long_p = 0;
        }
        if (key == SW2) {
            if (temp1++ == 200) {
                if (mode == 1) {
                    mode--;
                } else {
                    CLEAR_DISP_SCREEN;
                    return;
                }
                CLEAR_DISP_SCREEN;

            }
        } else {
            if (temp1 < 200 && temp1 > 0) {
                short_p = 1;
                if(mode==0)
                {
                    if(i>0)
                        i--;
                }
                CLEAR_DISP_SCREEN;
            }
            temp1 = 0;
            long_p = 0;
        }
        if (mode == 0) {
            clcd_print("set event        ", LINE1(3));
            clcd_print("view event        ", LINE2(3));
        }
        if (i == 0 && short_p == 1 && mode == 0) {
            clcd_print("=>", LINE1(0));
            clcd_print("  ", LINE2(0));
        } else if (i == 1 && short_p == 1 && mode == 0) {
            clcd_print("=>", LINE2(0));
            clcd_print("  ", LINE1(0));
        } else {
            clcd_print("=>", LINE1(0));
            clcd_print("  ", LINE2(0));
        }

        if (mode == 1) {
            switch (i) {
                case 0:set_event();
                    mode--;
                    break;
                case 1:view();
                mode--;


            }
        }
    }
}

void view_entrys(unsigned char add) 
{

    for (int j = 0; j < 10; j++) {
        alarm[j] = read_external_eeprom(add + j);
    }
    alarm[9] = '\0';
}