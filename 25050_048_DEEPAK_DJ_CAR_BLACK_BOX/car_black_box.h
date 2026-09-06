/* 
 * File:   car_black_box.h
 * Author: GANES
 *
 * Created on 3 September, 2026, 11:52 PM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H


static void get_time(unsigned char *clock_reg);


void dash_board(unsigned char key);


static void display_time(unsigned char *clock_reg);

void store(char *clock_reg, char *gear_str, unsigned int speed);

void login_mode(unsigned char key);

int my_strcmp(char *,char *);

void menu(unsigned char key);

void change_passwd(unsigned char key);

void view_log(unsigned char key);

void clear_log(unsigned char key);

void download_log(unsigned char key);

void set_time(unsigned char key);

#endif	/* CAR_BLACK_BOX_H */

