#ifndef CLCD_H
#define CLCD_H
#include <stdint.h>
void clcd_init(void);
void clcd_clear(void);
void clcd_goto(uint8_t row, uint8_t col);
void clcd_putc(char data);
void clcd_print(const char *str);
void clcd_print2(uint8_t value);
#endif
