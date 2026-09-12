
#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

void init_adc(void);
unsigned short read_adc(unsigned char channel);


#define CHANNEL0    0
#define CHANNEL1    1
#define CHANNEL2    2
#define CHANNEL3    3
#define CHANNEL4    4
#define CHANNEL5    5
#define CHANNEL6    6
#define CHANNEL7    7
#define CHANNEL8    8
#define CHANNEL9    9
#define CHANNEL10   10



#endif	

