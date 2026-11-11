/* 
 * File:   digitl_keypad.h
 * Author: dhiva
 *
 * Created on 6 November, 2026, 1:34 PM
 */

#ifndef DIGITL_KEYPAD_H
#define	DIGITL_KEYPAD_H

#define LEVEL_DETECTION         0
#define LEVEL                   0

#define STATE_DETECTION         1
#define STATE                   1

#define KEYPAD_PORT             PORTB
#define KEYPAD_PORT_DDR         TRISB

#define INPUT_LINES             0x0F       //all are released 00001111

#define SW1                     0x0E        //00001110
#define SW2                     0x0D        //00001101
#define SW3                     0x0B        //00001011
#define SW4                     0x07        //00000111

#define ALL_RELEASED            INPUT_LINES

unsigned char read_digital_keypad(unsigned char mode);
void init_digital_keypad(void);

#endif	/* DIGITL_KEYPAD_H */

