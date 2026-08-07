/*
  File: external_eeprom.c
  Description: Handles read and write operations to external EEPROM
*/


#include "main.h"

void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
    for (unsigned int wait = 3000;wait--;);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_rep_start();
    i2c_write(SLAVE_READ_E);
    data = i2c_read(0);
    i2c_stop();

    return data;
}

//store string at particular address in EEPROM
void external_eeprom_str_write(unsigned char addr, char *str)
{
    while(*str!='\0')
    {
        write_external_eeprom(addr,*str);
        addr++;
        str++;
    }
}

void external_eeprom_str_read(unsigned char addr, char *str,unsigned char len)
{
    for(int i=0;i<len;i++)
    {
        str[i]=read_external_eeprom(addr+i);
    }
   // str[len]='\0'; 
}