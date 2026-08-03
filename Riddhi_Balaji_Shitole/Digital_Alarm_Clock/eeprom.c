#include "main.h"

void save_alarm(unsigned char index,unsigned char hour,unsigned char min,unsigned char sec)
{
    alarms[index].hour = hour;
    alarms[index].min = min;
    alarms[index].sec = sec;
    alarms[index].enable = 1;
}

void save_alarm_eeprom(unsigned char index)
{
    unsigned char addr;

    addr = index * 4;

    write_internal_eeprom(addr,     alarms[index].hour);
    write_internal_eeprom(addr + 1, alarms[index].min);
    write_internal_eeprom(addr + 2, alarms[index].sec);
    write_internal_eeprom(addr + 3, alarms[index].enable);
}


void load_alarm(unsigned char index)
{
    unsigned char addr;

    addr = index * 4;

    alarms[index].hour =
            read_internal_eeprom(addr);

    alarms[index].min =
            read_internal_eeprom(addr + 1);

    alarms[index].sec =
            read_internal_eeprom(addr + 2);

    alarms[index].enable =
            read_internal_eeprom(addr + 3);
}

void load_all_alarms(void)
{
    unsigned char i;

    for(i = 0; i < MAX_ALARMS; i++)
    {
        load_alarm(i);
    }
}

unsigned char read_internal_eeprom(unsigned char addr)
{
    EEADR = addr;
    EEPGD = 0;
    RD = 1;

    return EEDATA;
}
void write_internal_eeprom(unsigned char addr, unsigned char data)
{
    EEADR = addr;
    EEDATA = data;

    EEPGD = 0;
    WREN = 1;

    GIE = 0;

    EECON2 = 0x55;
    EECON2 = 0xAA;
    WR = 1;

    while(WR);

    WREN = 0;
    GIE = 1;
}