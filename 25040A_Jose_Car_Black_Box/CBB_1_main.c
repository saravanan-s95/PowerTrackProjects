/*
 * Project:   Car Black Box
 * Author: Jose Thankachan
*/

#include <xc.h>

#include "CBB_adc.h"
#include "CBB_clcd.h"
#include "CBB_main.h"
#include "CBB_matrix_keypad.h"
#include "CBB_ds1307.h"
#include "CBB_i2c.h"
#include "CBB_external_eeprom.h"
#include "CBB_uart.h"

unsigned long int speed = 0;
unsigned char gear = 0;
unsigned char old_gear = 0;
unsigned char switch_read = 0;

unsigned char print_details[17];
unsigned char dashboard_title[] = {"SPD  G     RTC  "};

SystemState_t current_state = DASHBOARD;

unsigned char scroll = 1;
unsigned char eeprom_scroll = 0;

unsigned char start_address = 0x00;
unsigned char log_count = 0;

extern unsigned char clock_reg[3];

void main(void) 
{
    init_adc();                 //Initialize peripherals and protocols
    init_clcd();
    init_matrix_keypad();
    init_i2c();
	  init_ds1307();
    init_uart();
    
    print_details[16] = '\0';
    
    check_whether_first_poweron();      //check whether the board is powered ON for the very first time
    
    while(1)
    {
        switch_read = read_switches(STATE_CHANGE);
               
        if(switch_read == 3)        //If the MK3 switch is pressed, It indicates collission
        {
            gear = COLLISION;
        }
               
        if(gear != old_gear)        //Check whether there is a change in gear
        {                           //If the gear changed, then store the details to external EEPROM
            old_gear = gear;
      
            //Data stored to EEPROM from address 0x00 
            //(For each gear shift, 5 bytes of data stored - speed, gear, hour, minute, second)
            
            if(start_address <= 0x2D)       //Upto Tenth entry, direct storing (The tenth entry will be filled from address 0x2D)
            {
                write_external_eeprom(start_address, speed);
                write_external_eeprom((start_address + 0x01), gear);
                write_external_eeprom((start_address + 0x02), clock_reg[0]);
                write_external_eeprom((start_address + 0x03), clock_reg[1]);
                write_external_eeprom((start_address + 0x04), clock_reg[2]);  

                start_address = start_address + 0x05;       //After each entry, increment start_address by 5 for storing the next data
            }
            
            else if(start_address > 0x31)       //If already ten entries present in EEPROM
            {
                shift_entries();                //Shift the entries (So that oldest data is removed and tenth spot becomes free)
                
                start_address = 0x2D;           //Then, further datas to be stored at the tenth location
                
                write_external_eeprom(start_address, speed);
                write_external_eeprom((start_address + 0x01), gear);
                write_external_eeprom((start_address + 0x02), clock_reg[0]);
                write_external_eeprom((start_address + 0x03), clock_reg[1]);
                write_external_eeprom((start_address + 0x04), clock_reg[2]);
                
                start_address = start_address + 0x05;
            }
            
            log_count++;            //After storing each data to EEPROM, increment log_count
            if(log_count > 10)
            {
                log_count = 10;     //Maximum entries stored is 10
            }
            
            write_external_eeprom(0x37, log_count);
            write_external_eeprom(0x38, start_address);
        }
        
        mode_based_working();      //Function for the system to work based on the selected mode
    }
    
    return;
}

