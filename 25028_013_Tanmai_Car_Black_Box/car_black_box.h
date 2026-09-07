/* 
 * File:   car_black_box.h
 * Author: deepa
 *
 * Created on 5 March, 2026, 8:04 PM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

/* modes */
#define OPERATION_MODE    0x11
#define LOGIN_MODE        0x12
#define MENU_MODE         0x13
#define VIEW_LOG_MODE     0x14
#define CLEAR_LOG_MODE    0x15
#define DOWNLOAD_LOG_MODE 0x16
#define SET_TIME_MODE     0x17
#define CHANGE_PWD_MODE   0x18

/* reset flag values */
#define RESET_NOTHING      0x21
#define RESET_LOGIN_SCREEN 0x22
#define RESET_MENU_SCREEN  0x23
#define RESET_VIEW_LOG     0x24
#define RESET_TIME         0x27
#define RESET_CHANGE_PWD   0x25
#define PASSWORD_WAIT      0x26

#define RETURN_BACK      0x31
#define LOGIN_SUCCESS    0x32
#define PASSWORD_CHANGED 0x33
#define PASSWORD_FAILED  0x44

/* sub modes in main menu  */
#define VIEW_LOG      0
#define CLEAR_LOG     1
#define DOWNLOAD_LOG  2
#define SET_TIME      3
#define CHANGE_PWD    4

extern volatile unsigned char inactive_time;

void display_dashboard(unsigned char event[], unsigned char speed);
void display_time(void);
void log_event(unsigned char event[], unsigned char speed);
void clear_screen(void);
void clcd_write(unsigned char add, unsigned char mode);
unsigned char log_in_password(unsigned char key, unsigned char reset_flag);
unsigned char menu_screen(unsigned char key, unsigned char reset_flag);
void event_scroll(unsigned char key, unsigned char reset_flag);
void clear_logs(void);
void download_logs(void);
unsigned char change_password(unsigned char key, unsigned char reset_flag);
void set_time(unsigned char key, unsigned char reset_flag);

#endif	/* CAR_BLACK_BOX_H */

