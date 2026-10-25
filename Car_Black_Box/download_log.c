#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "ds1307.h"
#include "uart.h"

extern unsigned char log_count;
extern unsigned char start;

void download_log()
{   
    transmit_str_uart("\033[2J\033[H");
    
    CLEAR_DISP_SCREEN;
    clcd_print("LOG DOWNLOADED",LINE1(1));
    
    unsigned char view_index = 0;
    unsigned char log_position;
    unsigned char address;

    /* Variables for storing speed gear and time(hour, minute and second) */
    unsigned char speed;
    unsigned char gear;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    
    int delay = 0;
    
    /* Log empty/cleared */
    if(log_count == 0)
    {
        transmit_str_uart("LOG EMPTY");
        
        while(1)
        {
            unsigned char key = read_matrix_keypad(LEVEL);
            
            if(key == SWITCH11)
            {
                if(delay++ >= LONG_PRESS)
                {
                    CLEAR_DISP_SCREEN;
                    return;
                }
            }
        }
    }
   
    for(int i=view_index;i<log_count;i++)
    {
        /* Start from oldest available log */
        log_position = start + i;

        if(log_position >= 12)
        {
            log_position = log_position - 12;
        }

        /* Get the required address to be read */
        address = log_position * 5;

        /* Read and store in variables */
        speed = read_external_eeprom(address + 0);
        gear  = read_external_eeprom(address + 1);
        hour  = read_external_eeprom(address + 2);
        min   = read_external_eeprom(address + 3);
        sec   = read_external_eeprom(address + 4);

        /* Transmit log number */
        transmit_str_uart("LOG ");
        if(i >= 9)
        {
            transmit_char_uart((i + 1)/10 + '0');
        }
        transmit_char_uart((i + 1)%10 + '0');
        transmit_str_uart(": ");

        /* Transmit speed */
        transmit_char_uart((speed/100 % 10) + '0');
        transmit_char_uart((speed/10 % 10) + '0');
        transmit_char_uart((speed % 10) + '0');
        
        transmit_char_uart(' ');

        /* Transmit gear */
        if(gear == 0)
        {
            transmit_char_uart(' ');
            transmit_char_uart('N');
        }
        else if(gear == 6)
        {
            transmit_char_uart(' ');
            transmit_char_uart('R');
        }
        else
        {
            transmit_char_uart('G');
            transmit_char_uart(gear + '0');
        }
        
        transmit_char_uart(' ');

        /* Transmit time */
        transmit_char_uart(((hour >> 4) & 0x0F) + '0');
        transmit_char_uart((hour & 0x0F) + '0');

        transmit_char_uart(':');

        transmit_char_uart(((min >> 4) & 0x0F) + '0');
        transmit_char_uart((min & 0x0F) + '0');

        transmit_char_uart(':');

        transmit_char_uart(((sec >> 4) & 0x0F) + '0');
        transmit_char_uart((sec & 0x0F) + '0');
        
        transmit_str_uart("\r\n");
    }
    
    while(1)
    {
        unsigned char key = read_matrix_keypad(LEVEL);

        /* Switch 11 */
        if(key == SWITCH11)
        {
            /* Long Press */
            if(delay++ >= LONG_PRESS)
            {
                /* Clear screen and return to menu */
                CLEAR_DISP_SCREEN;
                return;
            }
        }
    }
}
