/* 
 * File:   main.h
 * Author: Gaytahri
 *
 * Created on 11 NOV, 2026, 
 */

#ifndef MAIN_H
#define	MAIN_H

#include "clcd.h"
#include "digital_keypad.h"

extern unsigned char key;

//macros

#define        UP              RB0
#define        DOWN            RB1
#define        BUZZER          RC0

#define        DEFAULT          0
#define        CONFIG           1
#define        SET_VIEW_EVENT   2
#define        SET_EVENT        3
#define        SET_TIME_DATE    4
#define        SET_TIME         5
#define        SET_DATE         6
#define        VIEW_EVENT       7




#endif	/* MAIN_H */

