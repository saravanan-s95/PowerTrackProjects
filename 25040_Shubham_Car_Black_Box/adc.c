#include <xc.h>
#include "adc.h"

void init_adc()
{
    ADON = 0;//disable adc for confi
    
    PCFG0=0;
    PCFG1=0;
    PCFG2=0;
    PCFG3=0; //to select all channels as analog pins
    
    VCFG0 = 0;
    VCFG1=0; //select Vref as default
    
    ADCS0 = 0;
    ADCS1 = 1;
    ADCS2 = 0; //to select the frequency(Freq = 625Khz) -> 1TAD = 1.6 ms
    
    
    ACQT0=0;
    ACQT0 = 1;
    ACQT2=0; //to select the acquisition time as 4TAD(6.4ms)
    
    
    ADFM=1;//right justification
    
    ADON = 1; //Enable the ADC
}
unsigned short read_adc(unsigned char channel)
{
    ADCON0 = (ADCON0 & 0xc3) | (channel<<2);
    GO =1;
    while(GO); 
    
    return (ADRESH << 8) | ADRESL;
}

