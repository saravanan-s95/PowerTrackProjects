/* 
 * File:   main.h
 * Author: swaraj
 *
 * Created on 10 june, 5:53 PM
 */

#ifndef MAIN_H
#define	MAIN_H
#include "adc.h"
#include "digital_keypad.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "external_eeprom.h"

void dashboard(unsigned char key);
void store_event(void);
char login(void);
void menu(unsigned char key);
void view_log(void);
void clear_log(void);
void download_log(void);
void set_time(void);
void change_password(void);

extern int event_count;
#endif	/* MAIN_H */

