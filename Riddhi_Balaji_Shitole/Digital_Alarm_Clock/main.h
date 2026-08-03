#ifndef MAIN_H
#define	MAIN_H

#include<xc.h>
#include "clcd.h"
#include "mkp.h"
#include "i2c.h"
#include "alram.h"
#define _XTAL_FREQ 20000000
\
#pragma config WDTE = OFF

#define MODE_DASHBOARD      0
#define MODE_MENU           1
#define MODE_SET_TIME       2
#define MODE_SET_DATE       3
#define MODE_ALARM_MENU     4
#define MODE_SET_ALARM      5
#define MODE_VIEW_ALARM     6
#define MODE_DELETE_ALARM   7
#define MODE_ENABLE_DISABLE   8


#define BUZZER RC1

extern unsigned char clock_reg[3];
extern char time[9];
extern char date[11];

void buzzer_on(void);
void buzzer_off(void);

void load_dummy_time(void);
void increment_time(unsigned char field);
void decrement_time(unsigned char field);
void save_time(void);
unsigned char set_time(unsigned char key); 

void get_date(void);
unsigned char set_date(unsigned char key);
void load_dummy_date(void);
void save_date(void);
void increment_date(unsigned char field);
void decrement_date(unsigned char field);


void dashboard(unsigned char key);
unsigned char menu(unsigned char key);
unsigned char alarm_menu(unsigned char key);

void init_timer();
void __interrupt() isr(void);


#endif	/* MAIN_H */

