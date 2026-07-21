/* 
 * File:   main.h
 * Author: manoj
 *
 * Created on 26 June, 2026, 7:17 AM
 */

#ifndef MAIN_H
#define	MAIN_H
#include "clcd.h"
#include "matrix_keypad.h"
#include "timers.h"

typedef enum
{
    POWER_ON,
    MENU,
    MICRO,
    GRILL,
    CONVECTION,
    START,
    SET_TIME,
    RUN_MODE,
    PREHEAT,
    TIME_UP
} State;

#define FAN_DDR    TRISC2
#define FAN        RC2

#define BUZZER_DDR  TRISC1
#define BUZZER      RC1

#define ON         1
#define OFF        0

State current_state = POWER_ON;

void power_on_screen(void);
void menu_screen(unsigned char key);
void micro_screen(void);
void grill_screen(void);
void convection_screen(unsigned char key);
void start_screen(void);
void set_time_screen(unsigned char key);
void run_mode_screen(unsigned char key);
void time_up_screen(void);
void preheat_screen(void);
unsigned char decrement_time(void);

#endif	/* MAIN_H */

