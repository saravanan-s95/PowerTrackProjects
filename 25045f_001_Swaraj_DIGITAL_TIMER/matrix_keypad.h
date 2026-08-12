/* 
 * File:   matrix_keypad.h
 * Author: Dell
 *
 * Created on 10 August, 2026, 9:08 PM
 */

#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H

// Key values
#define UP      1
#define DOWN    2
#define RIGHT   3
#define LEFT    4

void init_matrix_keypad(void);
unsigned char read_switches(void);

#endif