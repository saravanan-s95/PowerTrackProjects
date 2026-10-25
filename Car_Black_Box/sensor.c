#include "adc.h"
#include "sensor.h"

uint8_t get_speed()
{
    // Implement the speed function
    
    /* Read adc channel value and return */
    /* Speed range is 0-100 */
    return (read_adc(SPEED_ADC_CHANNEL) / 10.23);
}

unsigned char get_gear_pos()
{
    // Implement the gear function
    
    /* Initialize gear position at neutral */
    static unsigned char gear_pos = 0;
    
    static unsigned char collide = 0;
    
    /* Read edge triggered matrix keypad for changing gear position */
    unsigned char key = read_matrix_keypad(EDGE);

    /* Car collided return neutral */
    if(collide == 1)
    {
        return 0;
    }
    
    /* Increment or decrement gears based on matrix keypad */
    if(key == GEAR_UP) 
    {
        if(gear_pos < MAX_GEAR)
        {
            gear_pos++;
        }
    } 
    else if(key == GEAR_DOWN)
    {
        if(gear_pos > 0)
        {
            gear_pos--;
        }
    }
    else if(key == COLLISION)
    {
        collide = 1;
    }
    return gear_pos;
}