/* 
 * File:   clcd.h
 * Author: Dell
 *
 * Created on 10 August, 2026, 9:43 PM
 */

#ifndef CLCD_H
#define CLCD_H

#define LINE1(x)   (0x80 + (x))
#define LINE2(x)   (0xC0 + (x))

void init_clcd(void);
void clcd_putch(unsigned char data, unsigned char addr);
void clcd_print(const char *data, unsigned char addr);
void clcd_clear(void);

#endif