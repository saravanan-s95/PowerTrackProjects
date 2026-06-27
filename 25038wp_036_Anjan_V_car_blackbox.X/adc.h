/* 
 * File:   adc.h
 * Author: HP
 *
 * Created on 22 June, 2026, 6:10 PM
 */

#ifndef ADC_H
#define	ADC_H

#define CHANNEL0        0x00  //0b000
#define CHANNEL1        0x01 //0b001
#define CHANNEL2        0x02 //0b010
#define CHANNEL3        0x03 //0b011
void init_adc(void);
unsigned short read_adc(unsigned char channel);

#endif	/* ADC_H */

