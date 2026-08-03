#include "main.h"

void clcd_write(unsigned char byte,unsigned char mode)
{
    CLCD_RS = (mode != 0);
    CLCD_DATA_PORT = byte;
    
    CLCD_EN = hi;
    __delay_us(100);
    CLCD_EN = low;
  
    __delay_us(4100);
    
}
static void init_dis_cont(void)
{
    __delay_ms(30);
    
    clcd_write(eight_bit_mode, init_mode);
    __delay_us(4100);
    clcd_write(eight_bit_mode, init_mode);
    __delay_us(100);
    clcd_write(eight_bit_mode, init_mode);
    __delay_us(1);
    
    clcd_write(TWO_LINES_5x8_8_BIT_MODE,init_mode);
    __delay_us(100);
    clcd_write( CLEAR_DISP_SCREEN, init_mode);
    __delay_ms(500);
    clcd_write(DISP_ON_AND_CURSOR_OFF, init_mode);
    __delay_ms(100);
}

void init_clcd(void)
{
    CLCD_DATA_PORT_DDR = 0x00;
    CLCD_RS_DDR = low;
    CLCD_EN_DDR = low;
 
    init_dis_cont();
}

void clcd_clear(void)
{
    clcd_write(0x01, init_mode);   
    __delay_ms(2);               
}
void clcd_putch(const char data, unsigned char addr)
{
    clcd_write(addr, init_mode);
    clcd_write(data, data_mode);
}

void clcd_print(const char *str, unsigned char addr)
{
    clcd_write(addr, init_mode);
    
    while (*str != '\0')
    {
        clcd_write(*str, data_mode); 
        str++;
    }
}