#include <xc.h>

#include "CBB_adc.h"

extern unsigned long int speed;
extern unsigned char gear;


//Function to read speed from ADC
void read_speed(unsigned char* print_details)
{
    speed = read_adc(CHANNEL4);     //Read speed_data from 10-bit ADC
    speed = ((speed*100)/1023);     //Convert the speed to range of 0-100
    
    //Lock the speed based on the gear engaged
    if(gear == 0 && speed > 0)
    {
        speed = 0;          //If gear is Neutral, speed do not increase
    }
    else if(gear == 1 && speed > 15)
    {
        speed = 15;         //If gear is 1, maximum speed is set to 15
    }
    else if(gear == 2 && speed > 40)
    {
        speed = 40;         //If gear is 2, maximum speed is set to 40
    }
    else if(gear == 3 && speed > 65)
    {
        speed = 65;         //If gear is 3, maximum speed is set to 65
    }
    else if(gear == 4 && speed > 80)
    {
        speed = 80;         //If gear is 4, maximum speed is set to 80
    }
    else if(gear == 5 && speed > 100)
    {
        speed = 100;        //If gear is 5, maximum speed is set to 100
    }
       
    //Convert the speed data to ASCII to display on CLCD
    if(speed == 100)
    {
        print_details[0] = '1';
        print_details[1] = '0';
        print_details[2] = '0';
    }
    else
    {
        print_details[0] = ' ';
        print_details[1] = (((speed/10)%10) + '0');
        print_details[2] = ((speed%10) + '0');
    }
    print_details[3] = ' ';
    print_details[4] = ' ';
}


//Function to initialize ADC
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


//Function to read ADC value 
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