/* 
 * File:   timer.h
 */

#ifndef TIMER_H
#define	TIMER_H

#define TIMER2_ENABLE            TMR2ON  
#define TIMER2_PRESCALE0         T2CKPS0
#define TIMER2_PRESCALE1         T2CKPS1
#define TIMER2_PERIOD            PR2
#define TIMER2_INTERRUPT_FLAG   TMR2IF
#define TIMER2_INTERRUPT_ENABLE  TMR2IE

void init_timer2(void);
extern volatile unsigned char sec;
#endif	/* TIMER_H */

