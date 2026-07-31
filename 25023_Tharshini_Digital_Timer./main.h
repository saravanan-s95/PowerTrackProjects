/* 
 * File: main.h
 * Description: Header file for the Digital Alarm Clock application
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "i2c.h"
#include "ds1307.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "digital_timer.h"
#include "external_eeprom.h"
#include "timers.h"

extern unsigned char buzzer_secs;

#endif	/* MAIN_H */

