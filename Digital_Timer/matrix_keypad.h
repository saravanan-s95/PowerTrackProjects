#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H
void keypad_init(void);
char keypad_scan(void);
char keypad_get_event(void);
void keypad_wait_release(void);
/* Key mapping: 2=UP, 8=DOWN, 4=LEFT, 6=RIGHT, 5=SELECT, D=BACK.
   Long press returns U=long UP, V=long DOWN, S=long SELECT, d=long BACK. */
#endif
