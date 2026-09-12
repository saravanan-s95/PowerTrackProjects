

#include <xc.h>
#include "1_adc_adc.h"

void init_adc(void) 
{
    ADON = 0;       //Disable the ADC (During configuration, keep ADC in OFF mode)
    
    PCFG0 = 0;      //To select all the Channels as Analog
    PCFG1 = 0;
    PCFG2 = 0;
    PCFG3 = 0;
    
    VCFG0 = 0;      //Vmin as default (microcontroller GND)
    VCFG1 = 0;      //Vmax as default (microcontroller Vcc)
            
    ADCS0 = 0;      //To set the frequency of ADC (Freq = Fosc/32 = 20MHz/32 = 625KHz) (TAD = 1/625KHz = 1.6us)
    ADCS1 = 1;
    ADCS2 = 0;
    
    ACQT0 = 0;      //To select Acq_time as 4TAD (4TAD = 4*1.us = 6.4us)
    ACQT1 = 1;
    ACQT2 = 0;
    
    ADFM = 1;       //Right Justification
    
    ADON = 1;        //Enable ADC
    
    return;
}


unsigned short read_adc(unsigned char channel)
{
    ADCON0 = ((ADCON0 & 0xC3) | (channel << 2));        
    /*
     STEP1 - Clear the bit5 to 2 of ADCON1 (Result1)
     STEP2 - Get the 4-bits from channel (Result2)
     STEP3 = Result1 | Result2
     */
    
    
    GO = 1;     //Set the Status bit
    
    while(GO);  //Wait till the Conversion is complete
    
    return ((ADRESH << 8) | ADRESL);
}