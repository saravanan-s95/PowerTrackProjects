#include <xc.h>
#include "i2c.h"
#include "ds1307.h"

void init_ds1307(void)
{
    unsigned char dummy;
   
    dummy = read_ds1307(SEC_ADDR);
    dummy = dummy & 0x7F;
    write_ds1307(SEC_ADDR, dummy);// ch = 0
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

void write_ds1307(unsigned char addr, unsigned char data) // SEc_ADDR, data
{
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}
void save_time_ds1307(char *time)
{
    unsigned char hh, mm, ss;
    hh = ((time[0]-'0')<<4) | (time[1]-'0');
    mm = ((time[2]-'0')<<4) | (time[3]-'0');
    ss = ((time[4]-'0')<<4) | (time[5]-'0');

    write_ds1307(HOUR_ADDR, hh);
    write_ds1307(MIN_ADDR, mm);
    write_ds1307(SEC_ADDR, ss);
}