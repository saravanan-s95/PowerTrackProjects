/* 
 * File:   matrix_keypad.h
 * Author: Sundar
 *
 * Created on 29 September, 2026, 8:59 AM
 */

#ifndef MATRIX_KEYPAD_H
#define	MATRIX_KEYPAD_H

void init_matrix_keypad(void);
unsigned char read_matrix_keypad(unsigned char trigger);

#define ROW1 RB5
#define ROW2 RB6
#define ROW3 RB7

#define COL1 RB1
#define COL2 RB2
#define COL3 RB3
#define COL4 RB4

#define SWITCH1 1
#define SWITCH2 2
#define SWITCH3 3
#define SWITCH4 4
#define SWITCH5 5
#define SWITCH6 6
#define SWITCH7 7
#define SWITCH8 8
#define SWITCH9 9
#define SWITCH10 10
#define SWITCH11 11
#define SWITCH12 12

#define LEVEL 0
#define EDGE 1

#define ALL_RELEASED 0xFF

#define LONG_PRESS 500

#endif	/* MATRIX_KEYPAD_H */