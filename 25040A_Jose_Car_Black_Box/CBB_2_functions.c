#include <xc.h>

#include "CBB_adc.h"
#include "CBB_clcd.h"
#include "CBB_main.h"
#include "CBB_matrix_keypad.h"
#include "CBB_ds1307.h"
#include "CBB_i2c.h"
#include "CBB_external_eeprom.h"
#include "CBB_uart.h"

extern unsigned long int speed;
extern unsigned char gear;
extern unsigned char switch_read;

extern unsigned char print_details[17];
extern unsigned char dashboard_title[];

extern SystemState_t current_state;

extern unsigned char scroll;
extern unsigned char eeprom_scroll;

extern unsigned char start_address;
extern unsigned char log_count;

unsigned char eeprom_content[17];


//Function for the system to work based on the selected mode
void mode_based_working(void)
{
    switch(current_state)
    {
        case DASHBOARD:                 //If inside view_dashboard
        {
            if(switch_read == 11)       //If Entry switch is pressed, move to display_menu mode
            {
                current_state = DISPLAY_MENU;
                scroll = 1;             //Reset scroll to 1, so that starts from first option
            }
            else                        //If inside the DASHBOARD, then print the live details
            {
                read_gear(print_details);       //Read the gear data
                read_speed(print_details);      //Read the speed data
                get_time();                     //Read the time from RTC

                clcd_print_line(dashboard_title, LINE1(0));     //Display the details
                clcd_print_line(print_details, LINE2(0));
            }
        }
        break;
            
        case DISPLAY_MENU:              //If inside display_menu
        {
            if(switch_read == 12)      
            {
                current_state = DASHBOARD;          //If Exit key pressed, move back to dashboard 
                clcd_write(0x01, INSTRUCTION);      //Clear display                                  
            }
            else if(switch_read == 11)              //If Entry switch is pressed, based on the 'scroll' value, move the corresponding state   
            {
                if(scroll == 1)
                {
                    current_state = VIEW_LOG;       //If scroll=1, then arrow pointing to view_log; So, enter to VIEW_LOG
                }
                else if(scroll == 2)
                {
                    current_state = DOWNLOAD_LOG;   //If scroll=2, then arrow pointing to DOWNLOAD_LOG; So, enter to DOWNLOAD_LOG
                }
                else if(scroll == 3)
                {
                    current_state = SET_LOG;        //If scroll=3, then arrow pointing to SET_LOG; So, enter to SET_LOG
                }
                else if(scroll == 4)
                {
                    current_state = CLEAR_LOG;     //If scroll=4, then arrow pointing to CLEAR_LOG; So, enter to CLEAR_LOG
                }
            }
            else            //If no switch pressed, dispaly the menu
            {
                display_menu();
            }
        }
        break;
            
        case VIEW_LOG:                  //If inside VIEW_LOG
        {
            if(switch_read == 12)      
            {
                current_state = DISPLAY_MENU;      //If Exit key pressed, move back to display menu            
            }
            else      
            {
                view_log_eeprom_scrolling();    //Function to decide which data to be displayed on clcd (based on eeprom_scroll value)
                 
                view_log_eeprom_contents();     //function to read the contents stored in EEPROM and display on clcd
            }
        }
        break;
            
        case DOWNLOAD_LOG:          //If inside DOWNLOAD_LOG
        {
            if(switch_read == 12)      
            {
                current_state = DISPLAY_MENU;      //If Exit key pressed, move back to display menu            
            }
            else      
            {  
                download_log_using_uart();         //function to transmit data to PC using UART (Download log)
                
                clcd_print_line("                ", LINE1(0));
                clcd_print_line("Download Success", LINE2(0));      //Print a success message on screen
                for(long int i=0; i<100000; i++);                   //A delay to keep the success message on clcd for some time
                
                current_state = DISPLAY_MENU;       //After sending the data once, move back to menu; otherwise, the teraterm will be loaded with infinite data
            }
        }
        break;
            
        case SET_LOG:           //If inside SET_LOG
        {
            if(switch_read == 12)      
            {
                current_state = DISPLAY_MENU;      //If Exit key pressed, move back to display menu            
            }
            else      
            {
                set_time();                        //function to set log 
                
                clcd_print_line("                ", LINE1(0));
                clcd_print_line("TIME SET SUCCESS", LINE2(0));      //Print a success message on screen
            }
        }
        break;
            
        case CLEAR_LOG:         //If inside CLEAR_LOG
        {
            if(switch_read == 12)      
            {
                current_state = DISPLAY_MENU;      //If Exit key pressed, move back to display menu            
            }
            else      
            {
                //clear the log (external eeprom)
                log_count = 0;              //Make log_count to 0
                start_address = 0x00;       //Make starting address to 0x00, so that next data will be written from 0x00
                eeprom_scroll = 0;
                
                write_external_eeprom(0x37, log_count);
                write_external_eeprom(0x38, start_address);
            
                clcd_print_line("                ", LINE1(0));
                clcd_print_line("  LOG  CLEARED  ", LINE2(0));      //Print a success message on screen
            }
        }
        break;    
    }    
}


//Function to display the menu_options on CLCD
void display_menu(void)
{
    if(switch_read == 1)            //if switch pressed in MK1, increment the scroll (ie., scroll down)
    {
        if(scroll <= 3)
        {
            scroll++;
        }
        else if(scroll >= 4)        //Do not increment scroll beyond 4 (because we only have four menu options)
        {
            scroll = 4;
        }
    }
    else if(switch_read == 2)       //if switch pressed in MK2, decrement the scroll (ie., scroll up)
    {
        if(scroll >= 2)
        {
            scroll--;
        }
        else if(scroll <= 1)
        {
            scroll = 1;             //Do not increment scroll beyond 1
        }
    }
    
    
    if(scroll == 1)             //Print the options on CLCD based on the 'scroll' value
    {
        clcd_print_line("--> View log    ", LINE1(0));      //if scroll=1, print menu with arrow pointing to View log
        clcd_print_line("    Download log", LINE2(0));
    }
    else if(scroll == 2)            //if scroll=2, print menu with arrow pointing to Download log
    {
        clcd_print_line("    View log    ", LINE1(0));
        clcd_print_line("--> Download log", LINE2(0));
    }
    if(scroll == 3)                 //if scroll=3, print menu with arrow pointing to Set log
    {
        clcd_print_line("    Download log", LINE1(0));
        clcd_print_line("--> Set log     ", LINE2(0));
    }
    if(scroll == 4)                 //if scroll=4, print menu with arrow pointing to Clear log
    {
        clcd_print_line("    Set log     ", LINE1(0));
        clcd_print_line("--> Clear log   ", LINE2(0));
    }
}


//Function to shift entries present in EEPROM when 11th entry comes
void shift_entries(void)
{
    unsigned char data = 0;
    unsigned char remove_data_base_address = 0x00;      //Initialize address to be overwritten with 0x00
    unsigned char new_data_base_address = (remove_data_base_address + 0x05);    //The data to be written into 0x00 is the data present in 0x05
    
    for(int i=1; i<10; i++)          //Do the shifting 9 times (so, all the data is shifted and the tenth location can be used for newly coming data)
    {
        for(int j=0; j<5; j++)
        {
            data = read_external_eeprom(new_data_base_address + j);         //Read data from the address location 
            write_external_eeprom((remove_data_base_address + j), data);    //Write the data to the destination location
        }
        remove_data_base_address = remove_data_base_address+0x05;           //Increment the address_to_be_overwritten by 5
        new_data_base_address = (remove_data_base_address + 0x05);          //Increment the destination address by 5
    }
}


//Function to view the log on the CLCD based on eeprom_scroll 
void view_log_eeprom_contents(void)
{
    unsigned char e_speed;
    unsigned char e_gear;
    unsigned char e_hour;
    unsigned char e_min;
    unsigned char e_sec;
    
    unsigned char base_address = 0x00;
    
    clcd_print_line("SPD  G    TIME  ", LINE1(0));      //Print title
    
    if(log_count == 0)      //If log_count = 0, means no logs present; So print "LOG IS EMPTY"
    {
        clcd_print_line(" -LOG IS EMPTY- ", LINE2(0));
    }
    else                    //If any data exist in EEPROM
    {
        unsigned char target_start_address = (base_address + (eeprom_scroll*5));        //Set target address based on the eeprom_scroll value 
                                                                                        //eg: of eeprom_scroll=1, print the second log

        e_speed = read_external_eeprom(target_start_address);           //Read the speed data from eeprom
        e_gear = read_external_eeprom(target_start_address + 1);        //Read the gaer data from eeprom
        e_hour = read_external_eeprom(target_start_address + 2);        //Read the hour data from eeprom
        e_min = read_external_eeprom(target_start_address + 3);         //Read the minute data from eeprom
        e_sec = read_external_eeprom(target_start_address + 4);         //Read the second data from eeprom

        //Convert speed to ASCII to display on CLCD
        if(e_speed == 100)
        {
            eeprom_content[0] = '1';
            eeprom_content[1] = '0';
            eeprom_content[2] = '0';
        }
        else
        {
            eeprom_content[0] = ' ';
            eeprom_content[1] = (((e_speed/10)%10) + '0');
            eeprom_content[2] = ((e_speed%10) + '0');
        }
        eeprom_content[3] = ' ';
        eeprom_content[4] = ' ';


        //Convert gear to ASCII to display on CLCD
        switch(e_gear)
            {
                case 0: eeprom_content[5] = 'N';
                        break;
                case 1: eeprom_content[5] = 1+'0';
                        break;
                case 2: eeprom_content[5] = 2+'0';
                        break;
                case 3: eeprom_content[5] = 3+'0';
                        break;
                case 4: eeprom_content[5] = 4+'0';
                        break;
                case 5: eeprom_content[5] = 5+'0';
                        break;
                case 6: eeprom_content[5] = 'R';
                        break;
                case 7: eeprom_content[5] = 'C';
                        break;
            }   

        eeprom_content[6] = ' ';
        eeprom_content[7] = ' ';


        //Convert time to ASCII to display on CLCD
        eeprom_content[8] = '0' + ((e_hour >> 4) & 0x03);
        eeprom_content[9] = '0' + (e_hour & 0x0F);
        eeprom_content[10] = ':';
        eeprom_content[11] = '0' + ((e_min >> 4) & 0x0F);
        eeprom_content[12] = '0' + (e_min & 0x0F);
        eeprom_content[13] = ':';
        eeprom_content[14] = '0' + ((e_sec >> 4) & 0x0F);
        eeprom_content[15] = '0' + (e_sec & 0x0F);
        eeprom_content[16] = '\0';

        clcd_print_line(eeprom_content, LINE2(0));      //Print the details on CLCD
    }
    
}


//Function to decide which log to be printed on CLCD based on eeprom_scroll
void view_log_eeprom_scrolling(void)
{
    if(switch_read == 1)            //If MKP1 Pressed, increment (ie., similar to down scrolling)
    {
        if(eeprom_scroll < (log_count-1))   //Do not increment beyond total_no_of_logs
        {
            eeprom_scroll++;
        }
        else if(eeprom_scroll >= (log_count-1))
        {
            eeprom_scroll = (log_count-1);
        }
    }
    else if(switch_read == 2)       //If MKP2 Pressed, decrement (ie., similar to up-scrolling)
    {
        if(eeprom_scroll > 0 )
        {
            eeprom_scroll--;
        }
        else if(eeprom_scroll <= 0)
        {
            eeprom_scroll = 0;
        }
    }
}


//Function to transmit data to laptop using UART Protocol (via tera term)
void download_log_using_uart(void)
{
    unsigned char e_speed;
    unsigned char e_gear;
    unsigned char e_hour;
    unsigned char e_min;
    unsigned char e_sec;
    
    unsigned char base_address = 0x00;
    
    if(log_count == 0)          //If no logs present, print "LOG EMPTY"
    {
        string_transmit_uart(" -LOG IS EMPTY- \n\r");
    }
    else                        //If any data present in the EEPROM
    {
        string_transmit_uart("SPD  G    TIME  \n\r");
        
        for(int i=0; i<log_count; i++)              //Run the lopp log_count no.of times
        {
            unsigned char target_start_address = (base_address + (5*i));        //Move the target_address to the starting byte of each log (ie., the location where speed is stored)

            e_speed = read_external_eeprom(target_start_address);           //Read the speed data from the EEPROM
            e_gear = read_external_eeprom(target_start_address + 1);        //Read the gear data from the EEPROM
            e_hour = read_external_eeprom(target_start_address + 2);        //Read the hour data from the EEPROM
            e_min = read_external_eeprom(target_start_address + 3);         //Read the minute data from the EEPROM
            e_sec = read_external_eeprom(target_start_address + 4);         //Read the second data from the EEPROM

            //Convert speed to ASCII to transmit
            if(e_speed == 100)
            {
                eeprom_content[0] = '1';
                eeprom_content[1] = '0';
                eeprom_content[2] = '0';
            }
            else
            {
                eeprom_content[0] = ' ';
                eeprom_content[1] = (((e_speed/10)%10) + '0');
                eeprom_content[2] = ((e_speed%10) + '0');
            }
            eeprom_content[3] = ' ';
            eeprom_content[4] = ' ';


            //Convert gear to ASCII to transmit
            switch(e_gear)
                {
                    case 0: eeprom_content[5] = 'N';
                            break;
                    case 1: eeprom_content[5] = 1+'0';
                            break;
                    case 2: eeprom_content[5] = 2+'0';
                            break;
                    case 3: eeprom_content[5] = 3+'0';
                            break;
                    case 4: eeprom_content[5] = 4+'0';
                            break;
                    case 5: eeprom_content[5] = 5+'0';
                            break;
                    case 6: eeprom_content[5] = 'R';
                            break;
                    case 7: eeprom_content[5] = 'C';
                            break;
                }   

            eeprom_content[6] = ' ';
            eeprom_content[7] = ' ';


            //Convert time to ASCII to transmit
            eeprom_content[8] = '0' + ((e_hour >> 4) & 0x03);
            eeprom_content[9] = '0' + (e_hour & 0x0F);
            eeprom_content[10] = ':';
            eeprom_content[11] = '0' + ((e_min >> 4) & 0x0F);
            eeprom_content[12] = '0' + (e_min & 0x0F);
            eeprom_content[13] = ':';
            eeprom_content[14] = '0' + ((e_sec >> 4) & 0x0F);
            eeprom_content[15] = '0' + (e_sec & 0x0F);
            eeprom_content[16] = '\0';

            string_transmit_uart(eeprom_content);
            string_transmit_uart("\n\r");
        }
    }   
}


//Function to check whether the board is powered ON for the very first time
void check_whether_first_poweron(void)
{
    unsigned char test1, test2, test3;
    test1 = read_external_eeprom(0x34);     //Read the data from three address locations in EEPROM: 0x34, 0x35, 0x36
    test2 = read_external_eeprom(0x35);
    test3 = read_external_eeprom(0x36);
    
    if(test1=='C' && test2=='B' && test3=='B')      //Check whether the verification_string is present in the above mentioned locations
    {
        log_count = read_external_eeprom(0x37);         //If yes, the board has already powered ON, and hence contains data 
        start_address = read_external_eeprom(0x38);     //So, read the log_count and start_address data from 0x37 and 0x38 locations respectively
    }
    else                                    //If the verification_string is not present, the board is powered ON for the very first time
    {
        log_count = 0;                      //Initialize log_count and start_address
        start_address = 0x00;
        
        write_external_eeprom(0x34, 'C');   //Write the verification_string to these locations so that the upcoming reboots/powerON can be recognized
        write_external_eeprom(0x35, 'B');
        write_external_eeprom(0x36, 'B');
    }
}