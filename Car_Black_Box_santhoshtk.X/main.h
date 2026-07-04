/* 
 * File:   main.h
 * Author: 25014
 *
 * Created on 26 November, 2025, 5:53 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include "adc.h"
#include "digital_keypad.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "uart.h"
#include "external_eeprom.h"


#define TIME_TO_INT(x, y) ((x - '0') * 10) + (y - '0')
#define DIGIT_TO_BCD(x)    ((x / 10) << 4) | (x % 10)

#define MAX_MENU		5


void dashboard(unsigned char key);
void main_menu(void);


void view_log(void);
void clear_log(void);
void download_log(void);
void set_time(void);
void change_password(void);

#endif	/* MAIN_H */

