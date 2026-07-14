/* 
 * File:   mainn.h
 * Author: AKSHITHA REDDY G
 *
 */

#ifndef MAIN_H
#define	MAIN_H
 
#include <xc.h>
#include "timers.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "micro_oven.h"

#pragma config WDTE = OFF

#define MENU_SCREEN     0x00
#define MICRO           0x01
#define GRILL           0x02
#define CONVECTION      0x03
#define START           0x04
#define RETURN_BACK     0x05

#define RESET_NOTHING   0x20
#define RESET_MICRO     0x21
#define RESET_GRILL     0x22
#define RESET_CONVEC    0x23
#define RESET_START     0x24

#endif
