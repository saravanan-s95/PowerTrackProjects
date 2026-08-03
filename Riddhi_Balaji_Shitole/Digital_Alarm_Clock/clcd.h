#ifndef CLCD_H
#define	CLCD_H

#define CLCD_DATA_PORT_DDR TRISD
#define CLCD_RS_DDR TRISE2
#define CLCD_EN_DDR TRISE1

#define CLCD_DATA_PORT PORTD
#define CLCD_RS RE2
#define CLCD_EN RE1

#define hi 1
#define low 0

#define LINE1(x) (0x80 + x)
#define LINE2(x) (0xC0 + x)


#define init_mode 0
#define data_mode 1

#define eight_bit_mode 0x33
#define TWO_LINES_5x8_8_BIT_MODE    0x38
#define CLEAR_DISP_SCREEN           0x01
#define DISP_ON_AND_CURSOR_OFF      0x0C

void init_clcd(void);
static void init_dis_cont(void);
void clcd_write(unsigned char byte,unsigned char mode);
void clcd_putch(const char data, unsigned char addr);
void clcd_print(const char *str, unsigned char addr);
void clcd_clear(void);

#endif	/* CLCD_H */

