#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H


	
#define MAX_ROW				4
#define MAX_COL				3

#define STATE_CHANGE				1
#define LEVEL_CHANGE				0
#define MATRIX_KEYPAD_PORT			PORTB
#define ROW3					PORTBbits.RB7
#define ROW2					PORTBbits.RB6
#define ROW1					PORTBbits.RB5
#define COL4					PORTBbits.RB4
#define COL3					PORTBbits.RB3
#define COL2					PORTBbits.RB2
#define COL1					PORTBbits.RB1


#define SW1					1
#define SW2					2
#define SW3					3
#define SW4					4
#define SW5					5
#define SW6					6
#define SW7					7
#define SW8					8
#define SW9					9
#define SW10				10
#define SW11				11
#define SW12				12

#define ALL_RELEASED	0xFF

#define HI				1
#define LO				0

void init_matrix_keypad(void);
unsigned char scan_key(void);
unsigned char read_switches(unsigned char detection_type);

#endif
