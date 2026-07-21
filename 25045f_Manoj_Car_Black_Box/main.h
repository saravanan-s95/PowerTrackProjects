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

void dashboard(unsigned char key);
void store_event(void);
unsigned char login_screen();
unsigned char main_menu();
void view_log(void);
void clear_log(void);
void download_log(void);
unsigned char set_time();
void change_password(void);


void my_strcpy(char *main_password , char *new_password);
void read_logs(char *str , unsigned char address);
int my_strcmp(char *s1 , char *s2);

#endif	/* MAIN_H */

