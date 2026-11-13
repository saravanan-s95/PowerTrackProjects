#include <xc.h>
#include "main.h"
#include "clcd.h"

#define LCD_RS PORTBbits.RB0
#define LCD_EN PORTBbits.RB1

static void lcd_pulse(void)
{
    LCD_EN = 1;
    __delay_us(2);
    LCD_EN = 0;
    __delay_us(50);
}

static void lcd_nibble(uint8_t value)
{
    PORTD = (PORTD & 0x0F) | (value & 0xF0);
    lcd_pulse();
}

static void lcd_command(uint8_t command)
{
    LCD_RS = 0;
    lcd_nibble(command);
    lcd_nibble((uint8_t)(command << 4));
    if (command == 0x01 || command == 0x02)
        __delay_ms(2);
}

void clcd_init(void)
{
    __delay_ms(20);
    LCD_RS = 0;
    LCD_EN = 0;
    lcd_nibble(0x30);
    __delay_ms(5);
    lcd_nibble(0x30);
    __delay_us(150);
    lcd_nibble(0x30);
    lcd_nibble(0x20);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
}

void clcd_clear(void)
{
    lcd_command(0x01);
}

void clcd_goto(uint8_t row, uint8_t col)
{
    lcd_command((uint8_t)(0x80 + (row ? 0x40 : 0x00) + col));
}

void clcd_putc(char data)
{
    LCD_RS = 1;
    lcd_nibble((uint8_t)data);
    lcd_nibble((uint8_t)(data << 4));
}

void clcd_print(const char *str)
{
    while (*str)
        clcd_putc(*str++);
}

void clcd_print2(uint8_t value)
{
    clcd_putc((char)('0' + value / 10));
    clcd_putc((char)('0' + value % 10));
}
