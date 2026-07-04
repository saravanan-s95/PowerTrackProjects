/* 
 * File:   auth.h
 * Author: santhoshtk
 *
 * Created on June 30, 2026, 12:44 AM
 */

#ifndef AUTH_H
#define	AUTH_H

#define MAX_ATTEMPTS    3
#define GRANTED         1
#define DENIED          0

#define DELAY           30



void run_delay(void);
void auth_user(void);
void change_original_password(unsigned char new_password[5]);

#endif	/* AUTH_H */

