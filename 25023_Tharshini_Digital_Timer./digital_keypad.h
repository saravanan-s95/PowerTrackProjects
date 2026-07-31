/* 
 * File:   digital_keypad.h
 * Description: Header file for keypad input functions
 */

#ifndef DIGITAL_KEYPAD_H
#define	DIGITAL_KEYPAD_H

#define LEVEL_DETECTION         0
#define LEVEL                   0

#define STATE_DETECTION         1
#define STATE                   1

#define KEYPAD_PORT             PORTB
#define KEYPAD_PORT_DDR         TRISB

#define INPUT_LINES             0x3F

//short press
#define SW_UP                    0x3E
#define SW_DOWN                  0x3D
#define SW_LEFT                  0x3B
#define SW_RIGHT                 0x37

//long press
#define SW_UP_L                 0xBE  //SW_UP | 0x80 
#define SW_DOWN_L               0xBD  //SW_DOWN | 0x80 
#define SW_LEFT_L               0xBB  //SW_LEFT | 0x80
#define SW_RIGHT_L              0xB7  //SW_RIGHT | 0x80

#define ALL_RELEASED            INPUT_LINES

unsigned char read_digital_keypad(unsigned char mode);
void init_digital_keypad(void);

#endif	/* DIGITAL_KEYPAD_H */

