/*
 * File:   rtc.c
 * Author: Dell
 *
 * Created on 11 August, 2026, 12:19 PM
 */


#include "main.h"
#include "i2c.h"
#include "rtc.h"

void rtc_init(void)
{
    // Initialize I2C communication
    init_i2c();
}

unsigned char rtc_read(unsigned char address)
{
    unsigned char data;

    // Start I2C communication
    i2c_start();

    // Send DS1307 write address
    i2c_write(DS1307_WRITE);

    // Send register address
    i2c_write(address);

    // Start again for read operation
    i2c_start();

    // Send DS1307 read address
    i2c_write(DS1307_READ);

    // Read data from RTC
    data = i2c_read(0);

    // Stop I2C communication
    i2c_stop();

    return data;
}

void rtc_write(unsigned char address, unsigned char data)
{
    // Start I2C communication
    i2c_start();

    // Send DS1307 write address
    i2c_write(DS1307_WRITE);

    // Send register address
    i2c_write(address);

    // Send data
    i2c_write(data);

    // Stop I2C communication
    i2c_stop();
}

unsigned char dec_to_bcd(unsigned char data)
{
    return ((data / 10) << 4) | (data % 10);
}

unsigned char bcd_to_dec(unsigned char data)
{
    return ((data >> 4) * 10) + (data & 0x0F);
}