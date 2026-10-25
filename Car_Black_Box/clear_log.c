#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"

extern unsigned char log_count;
extern unsigned char log_index;
extern unsigned char start;

void clear_log()
{
    /* Reset counters */
    log_count = 0;
    log_index = 0;
    start = 0;
    
    CLEAR_DISP_SCREEN;
    int clear_delay = 0;
    
    while(1)
    {
        clcd_print("LOG CLEARED",LINE1(3));
        
        /* Read level triggered matrix keypad */
        unsigned char key = read_matrix_keypad(LEVEL);

        /* Switch 11 */
        if(key == SWITCH11)
        {
            /* Long press */
            if(clear_delay++ >= LONG_PRESS)
            {
                /* lear screen and return to menu */
                CLEAR_DISP_SCREEN;
                return;
            }
        }
        else
        {
            clear_delay = 0;
        }
    }
}
