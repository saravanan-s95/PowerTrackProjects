/* 
 * File:   main.h
 * Author: Swaraj
 *
 * Created on 23 June, 2026, 3:53 PM
 */

#ifndef MAIN_H
#define MAIN_H

#include <xc.h>

#define _XTAL_FREQ 20000000

/* Configuration Bits */
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

/* States */
#define SPLASH_SCREEN   0
#define HOME_SCREEN     1

#define MICRO_MODE      2
#define GRILL_MODE      3
#define CONV_MODE       4
#define QUICK_START     5

#define SET_TEMP        6
#define SET_TIME        7

#define READY           8
#define COOKING         9
#define PAUSE           10
#define DONE            11

#define PREHEAT         12

#define BUZZER      RC1
#define BUZZER_DDR  TRISC1

#define FAN         RC2
#define FAN_DDR     TRISC2

/* Function Prototypes */
void init_config(void);
void splash_screen(void);
void display_home(void);

#endif