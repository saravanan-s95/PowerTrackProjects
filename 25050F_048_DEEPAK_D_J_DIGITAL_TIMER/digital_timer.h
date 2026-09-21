/* 
 * File:   digital_timer.h
 * Author: GANES
 *
 * Created on 11 September, 2026, 3:23 PM
 */

#ifndef DIGITAL_TIMER_H
#define	DIGITAL_TIMER_H

static void get_date(unsigned char *);

static void get_time(unsigned char *);

void dashboard(unsigned char key);

void menu(unsigned char key);


void event(unsigned char key);


void date_time(unsigned char key);

void view_event(unsigned char key);

void set_event(unsigned char key);

void set_time(unsigned char key);

void set_date(unsigned char key);

#endif	/* DIGITAL_TIMER_H */

