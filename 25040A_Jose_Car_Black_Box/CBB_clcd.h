#ifndef CLCD_H
#define	CLCD_H

#include <xc.h>  

#define INSTRUCTION     0
#define DATA            1

#define LINE1(x)		(0x80 + (x))
#define LINE2(x)		(0xC0 + (x))

#define _XTAL_FREQ      20000000    //(Indicates 20MHz frequency) This macro is necessary to use __delay_ms() function.


void init_clcd(void);
void clcd_write(unsigned char to_write, unsigned char type);
void clcd_print_line(unsigned char* str, unsigned char address);


#endif	

