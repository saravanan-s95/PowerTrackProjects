#include "main.h"
#include "i2c.h"
#include "ds1307.h"

#define DS1307_WRITE 0xD0
#define DS1307_READ  0xD1

static uint8_t bcd_to_dec(uint8_t value)
{
    return (uint8_t)(((value >> 4) * 10) + (value & 0x0F));
}

static uint8_t dec_to_bcd(uint8_t value)
{
    return (uint8_t)(((value / 10) << 4) | (value % 10));
}

void ds1307_init(void)
{
    i2c_init();
}

bool ds1307_read(RTC_Time *time)
{
    i2c_start();
    i2c_write(DS1307_WRITE);
    i2c_write(0x00);
    i2c_restart();
    i2c_write(DS1307_READ);

    time->sec = bcd_to_dec(i2c_read(1) & 0x7F);
    time->min = bcd_to_dec(i2c_read(1));
    time->hour = bcd_to_dec(i2c_read(1) & 0x3F);
    time->weekday = bcd_to_dec(i2c_read(1));
    time->date = bcd_to_dec(i2c_read(1));
    time->month = bcd_to_dec(i2c_read(1));
    time->year = bcd_to_dec(i2c_read(0));
    i2c_stop();

    return (time->sec < 60 && time->min < 60 && time->hour < 24 &&
            time->date >= 1 && time->date <= 31 &&
            time->month >= 1 && time->month <= 12);
}

void ds1307_write(const RTC_Time *time)
{
    i2c_start();
    i2c_write(DS1307_WRITE);
    i2c_write(0x00);
    i2c_write(dec_to_bcd(time->sec));
    i2c_write(dec_to_bcd(time->min));
    i2c_write(dec_to_bcd(time->hour));
    i2c_write(dec_to_bcd(time->weekday));
    i2c_write(dec_to_bcd(time->date));
    i2c_write(dec_to_bcd(time->month));
    i2c_write(dec_to_bcd(time->year));
    i2c_stop();
}
