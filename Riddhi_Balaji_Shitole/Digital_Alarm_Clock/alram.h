#ifndef ALARM_H
#define ALARM_H

#define MAX_ALARMS 5


#define MENU_WAIT              0
#define MENU_SUCCESS           1
#define MENU_BACK              2

#define SET_TIME_OPTION        1
#define SET_DATE_OPTION        2
#define ALARM_MENU_OPTION      3

#define SET_ALARM_OPTION       1
#define VIEW_ALARM_OPTION      2
#define DELETE_ALARM_OPTION    3
#define ENABLE_DISABLE_OPTION  4
#define BACK_OPTION            5

typedef struct
{
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned char enable;
} ALARM;

ALARM alarms[MAX_ALARMS];

void save_alarm(unsigned char index,unsigned char hour,unsigned char min,unsigned char sec);
void load_alarm(unsigned char index);
void load_all_alarms(void);

unsigned char set_alarm(unsigned char key);
unsigned char view_alarm(unsigned char key);
unsigned char delete_alarm(unsigned char key);
unsigned char enable_disable_alarm(unsigned char key);
unsigned char alarm_check(void);

void write_internal_eeprom(unsigned char addr, unsigned char data);
unsigned char read_internal_eeprom(unsigned char addr);

void save_alarm_eeprom(unsigned char index);

#endif

