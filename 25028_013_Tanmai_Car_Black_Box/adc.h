/* 
 * File:   adc.h
 
 */

#ifndef ADC_H
#define	ADC_H

#define _XTAL_FREQ         20000000

#define ENABLE_ADC         ADON
#define GO                 GO_nDONE
#define RIGHT_JUSTFICATION ADFM

#define ADC_HIGH_REG    ADRESH
#define ADC_LOW_REG     ADRESL

void init_adc(void);
unsigned int read_adc(void);

#endif	/* ADC_H */

