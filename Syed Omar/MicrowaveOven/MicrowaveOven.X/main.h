#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "timers.h"

/* ---------------- Screen / operation states ---------------- */
#define POWER_SCREEN        0x01
#define MENU_SCREEN         0x02
#define MICRO_SET_TIME      0x03
#define GRILL_SET_TIME      0x04
#define CONV_SET_TEMP       0x05
#define CONV_PREHEAT        0x06
#define CONV_SET_TIME       0x07   /* time entry AFTER preheat finishes   */
#define DISPLAY_TIME        0x08
#define PAUSE_SCREEN        0x09
#define STOP_SCREEN         0x0A

/* ---------------- Flags passed to screen functions ---------------- */
/* RESET_MODE    : just entered this screen -> clear its data AND redraw
 * REDRAW_ONLY   : data must NOT change (e.g. door closed mid-countdown),
 *                 just repaint the static labels because the LCD was wiped
 * RESET_NOTHING : normal pass, only refresh the changing digits
 */
#define RESET_MODE           0x10
#define REDRAW_ONLY          0x11
#define RESET_NOTHING        0x0F

/* ---------------- Digital logic ---------------- */
#define ON                   1
#define OFF                  0
#define OPEN                 0
#define CLOSED               1

/* ---------------- Peripherals ---------------- */
#define FAN_DDR              TRISC2
#define FAN                  RC2
#define BUZZER_DDR           TRISC1
#define BUZZER               RC1
#define DOOR_DDR             TRISB3
#define DOOR                 RB3

#define MAX_TEMP             180
#define PREHEAT_SECONDS      180   /* 3 minute preheat, non-blocking */

/* ---------------- Function prototypes ---------------- */
void power_on_screen(void);
void menu_display_screen(unsigned char reset_flag);
void set_time(unsigned char key, unsigned char reset_flag);
void set_temp(unsigned char key, unsigned char reset_flag);
void preheat_screen(unsigned char reset_flag);
void time_display_screen(unsigned char reset_flag);
void door_check(void);

#endif  /* MAIN_H */
