#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include <string.h>
#include "adc.h"
#include "digital_keypad.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "external_eeprom.h"
#include "uart.h"

// Global Variables 
extern unsigned char operation;
extern unsigned char event_count;
extern char data[5][17];
extern unsigned char clock_reg[3];
extern char password[5];
extern unsigned char time[9];
extern unsigned char old_menu_pos;

// Dashboard 
void dashboard(unsigned char key);

// Login 
unsigned char login(unsigned char key);

// Menu 
void menu(unsigned char key);

// View Log 
void view_log(unsigned char key);

// Clear Log 
void clear_log(void);

// Download Log 
void download_log(void);

// Set Time 
unsigned char set_time(unsigned char key);

// Change Password 
int change_password(unsigned char key);

// EEPROM Log Storage 
void store_event(void);


#endif	/* MAIN_H */

