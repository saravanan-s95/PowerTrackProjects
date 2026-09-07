#include <xc.h>
#include "adc.h"

void init_adc(void)
{
    ENABLE_ADC = 1;         //enable ADON bit
    RIGHT_JUSTFICATION = 1; // Setting ADFM = 1 for right justifiation
}

unsigned int read_adc(void)
{
    __delay_us(20); // wait for aquisition time
    
    GO = 1;         // start the convesrion
    
    while(GO);      // Wait till conversion 
    
    return ((unsigned int)ADC_HIGH_REG << 8) | ADC_LOW_REG; // store the result 
}