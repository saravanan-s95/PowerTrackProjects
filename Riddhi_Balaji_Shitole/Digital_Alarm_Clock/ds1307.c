
#include "main.h"

void init_ds1307(void)
{
    unsigned char dummy;
   
    dummy = read_ds1307(SEC_ADDR);
    dummy = dummy & 0x7F;
    write_ds1307(SEC_ADDR, dummy);
}

unsigned char read_ds1307(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(SLAVE_READ);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_ds1307(unsigned char addr, unsigned char data) 
{
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}
unsigned char clock_reg[3];
char time[9];

void display_time(unsigned char *clock_reg)
{ 
    // HH ->
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    time[2] = ':';
    // MM 
    time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[4] = (clock_reg[1] & 0x0F) + '0';
    
    time[5] = ':';
    // SS
    time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[7] = (clock_reg[2] & 0x0F) + '0';
    time[8] = '\0';

}

void get_time(unsigned char *clock_reg)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR) & 0x3F;
    clock_reg[1] = read_ds1307(MIN_ADDR) & 0x7F;
    clock_reg[2] = read_ds1307(SEC_ADDR) & 0x7F;
}

char date[11];

void get_date(void)
{
    unsigned char day, month, year;

    day = read_ds1307(DATE_ADDR) & 0x3F;
    month = read_ds1307(MONTH_ADDR) & 0x1F;
    year = read_ds1307(YEAR_ADDR);

    date[0] = ((day >> 4) & 0x03) + '0';
    date[1] = (day & 0x0F) + '0';
    date[2] = '/';

    date[3] = ((month >> 4) & 0x01) + '0';
    date[4] = (month & 0x0F) + '0';
    date[5] = '/';

    date[6] = '2';
    date[7] = '0';
    date[8] = ((year >> 4) & 0x0F) + '0';
    date[9] = (year & 0x0F) + '0';
    date[10] = '\0';
}