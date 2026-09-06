/* 
 * File:   newfile.h
 * Author: GANES
 *
 * Created on 26 July, 2026, 8:15 PM
 */

#ifndef NEWFILE_H
#define	NEWFILE_H

void init_digital_keypad(void);
unsigned char read_digital_keypad(unsigned char);
#define KEY_PORT PORTB
#define KEY_PORT_DDR TRISB

#define LEVEL 0
#define STATE 1

// Switch macros for active-low buttons on Pins 0 through 5
#define SW1 0x3E  // Pin 0 is 0 (Binary: 111110)
#define SW2 0x3D  // Pin 1 is 0 (Binary: 111101)
#define SW3 0x3B  // Pin 2 is 0 (Binary: 111011)
#define SW4 0x37  // Pin 3 is 0 (Binary: 110111)
#define SW5 0x2F  // Pin 4 is 0 (Binary: 101111)
#define SW6 0x1F  // Pin 5 is 0 (Binary: 011111)

// It is also helpful to have a macro for when NO buttons are pressed
#define ALL_RELEASED 0x3F // All pins are 1 (Binary: 111111)
#endif	/* NEWFILE_H */

