#include <xc.h>
#include "clcd.h"

/* Sends exactly one nibble - used only for the 4-bit-mode init handshake,
 * while the controller is still technically in 8-bit mode. */
static void clcd_write_nibble(unsigned char nibble)
{
    CLCD_DATA_PORT = (CLCD_DATA_PORT & 0x0F) | (nibble & 0xF0);
    CLCD_EN = HI;
    __delay_us(100);
    CLCD_EN = LOW;
    __delay_us(100);
}

void clcd_write(unsigned char byte, unsigned char mode)
{
    CLCD_RS = (__bit) mode;

    /* Upper nibble first, masked so we never disturb bits 0-3
     * (shared with the keypad row outputs on this board). */
    CLCD_DATA_PORT = (CLCD_DATA_PORT & 0x0F) | (byte & 0xF0);
    CLCD_EN = HI;
    __delay_us(100);
    CLCD_EN = LOW;

    /* Lower nibble, shifted up into bits 4-7 */
    CLCD_DATA_PORT = (CLCD_DATA_PORT & 0x0F) | ((unsigned char)(byte << 4) & 0xF0);
    CLCD_EN = HI;
    __delay_us(100);
    CLCD_EN = LOW;

    __delay_us(100); /* most instructions finish well within this */
}

static void init_disp_control(void)
{
    CLCD_RS = LOW;
    __delay_ms(30);

    /* Software-reset handshake, sent in 8-bit mode per datasheet */
    clcd_write_nibble(0x30);
    __delay_ms(5);
    clcd_write_nibble(0x30);
    __delay_us(150);
    clcd_write_nibble(0x30);
    __delay_us(150);

    /* Switch to 4-bit mode - MUST be a single nibble, not clcd_write() */
    clcd_write_nibble(0x20);
    __delay_us(150);

    /* From here on the controller is in 4-bit mode; normal two-nibble
     * writes via clcd_write() are correct. */
    clcd_write(TWO_LINES_5x8_4_BIT_MODE, INST_MODE);
    clcd_write(DISP_ON_CURSOR_OFF, INST_MODE);
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_ms(2);
}

void init_clcd(void)
{
    CLCD_DATA_PORT_DDR &= 0x0F; /* RD4-RD7 as outputs for the LCD bus,
                                   RD0-RD3 (keypad rows) left untouched */
    CLCD_RS_DDR = 0;            /* output */
    CLCD_EN_DDR = 0;            /* output */

    init_disp_control();
}

void clcd_putch(const char data, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    clcd_write(data, DATA_MODE);
}

void clcd_print(const char *str, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    while (*str != '\0')
    {
        clcd_write(*str, DATA_MODE);
        str++;
    }
}

void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

