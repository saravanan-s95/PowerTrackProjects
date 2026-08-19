/*
 * File:   i2c.c
 * Author: Dell
 *
 * Created on 11 August, 2026, 12:08 PM
 */


#include "main.h"
#include "i2c.h"

void init_i2c(void)
{
    // Configure I2C pins as input
    TRISC3 = 1;
    TRISC4 = 1;

    // Enable MSSP module in I2C Master mode
    SSPCON = 0x28;

    // I2C clock = 100 kHz for 20 MHz crystal
    SSPADD = 49;

    // Clear status register
    SSPSTAT = 0x00;
}

void i2c_start(void)
{
    // Send START condition
    SSPCON2bits.SEN = 1;

    while(SSPCON2bits.SEN);
}

void i2c_stop(void)
{
    // Send STOP condition
    SSPCON2bits.PEN = 1;

    while(SSPCON2bits.PEN);
}

void i2c_write(unsigned char data)
{
    // Load data into SSP buffer
    SSPBUF = data;

    // Wait until transmission is complete
    while(SSPSTATbits.BF);

    // Wait for ACK
    while(SSPCON2bits.ACKSTAT);
}

unsigned char i2c_read(unsigned char ack)
{
    unsigned char data;

    // Enable receive mode
    SSPCON2bits.RCEN = 1;

    // Wait for data
    while(!SSPSTATbits.BF);

    data = SSPBUF;

    // Send ACK/NACK
    SSPCON2bits.ACKDT = !ack;
    SSPCON2bits.ACKEN = 1;

    while(SSPCON2bits.ACKEN);

    return data;
}