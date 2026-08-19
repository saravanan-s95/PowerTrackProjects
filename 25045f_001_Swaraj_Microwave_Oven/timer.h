/* 
 * File:   timer.h
 * Author: Dell
 *
 * Created on 23 June, 2026, 5:52 PM
 */

#ifndef TIMER_H
#define TIMER_H

#include <xc.h>

extern volatile unsigned char one_sec_flag;

void init_timer2(void);    // here we are using timer 2

#endif