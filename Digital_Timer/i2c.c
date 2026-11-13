#include <xc.h>
#include "main.h"
#include "i2c.h"

static void i2c_wait(void)
{
    while ((SSPCON2 & 0x1F) || SSPSTATbits.R_W)
        ;
}

void i2c_init(void)
{
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    SSPSTAT = 0x80;
    SSPCON = 0x28;
    SSPCON2 = 0x00;
    SSPADD = 49; /* 100 kHz with 20 MHz oscillator */
}

void i2c_start(void)
{
    i2c_wait();
    SSPCON2bits.SEN = 1;
    while (SSPCON2bits.SEN)
        ;
}

void i2c_restart(void)
{
    i2c_wait();
    SSPCON2bits.RSEN = 1;
    while (SSPCON2bits.RSEN)
        ;
}

void i2c_stop(void)
{
    i2c_wait();
    SSPCON2bits.PEN = 1;
    while (SSPCON2bits.PEN)
        ;
}

void i2c_write(uint8_t data)
{
    i2c_wait();
    SSPBUF = data;
    while (SSPSTATbits.BF)
        ;
    i2c_wait();
}

uint8_t i2c_read(uint8_t ack)
{
    uint8_t data;
    i2c_wait();
    SSPCON2bits.RCEN = 1;
    while (!SSPSTATbits.BF)
        ;
    data = SSPBUF;
    i2c_wait();
    SSPCON2bits.ACKDT = ack ? 0 : 1;
    SSPCON2bits.ACKEN = 1;
    while (SSPCON2bits.ACKEN)
        ;
    return data;
}
