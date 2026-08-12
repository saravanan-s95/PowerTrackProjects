/*
 * File:   clcd.c
 * Author: Dell
 *
 * Created on 10 August, 2026, 9:46 PM
 */


#include "main.h"
#include "clcd.h"

static void clcd_write(unsigned char data, unsigned char control)
{
    // Send data to LCD
    PORTD = data;

    // RS = RE2
    RE2 = control;

    // R/W is connected to GND

    // Enable pulse
    RE1 = 1;
    __delay_us(100);
    RE1 = 0;

    __delay_us(100);
}

void init_clcd(void)
{
    // Make PORTD pins as output
    TRISD = 0x00;

    // Make PORTE pins as output
    TRISE = 0x00;

    // Make analog pins digital
    ADCON1 = 0x06;

    // Initial values
    PORTD = 0x00;
    RE1 = 0;
    RE2 = 0;

    __delay_ms(20);

    // 8-bit mode, 2 lines
    clcd_write(0x38, 0);

    // Display ON, cursor OFF
    clcd_write(0x0C, 0);

    // Clear display
    clcd_write(0x01, 0);

    __delay_ms(2);

    // Start from first position
    clcd_write(0x80, 0);
}

void clcd_putch(unsigned char data, unsigned char addr)
{
    // Set LCD address
    clcd_write(addr, 0);

    // Write character
    clcd_write(data, 1);
}

void clcd_print(const char *data, unsigned char addr)
{
    // Set LCD address
    clcd_write(addr, 0);

    while(*data != '\0')
    {
        // Write one character
        clcd_write(*data, 1);

        data++;
    }
}

void clcd_clear(void)
{
    // Clear the LCD
    clcd_write(0x01, 0);

    // Wait for LCD to clear
    __delay_ms(2);
}