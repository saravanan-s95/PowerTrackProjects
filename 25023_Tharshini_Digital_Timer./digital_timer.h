/* 
 * File:   digital_timer.h
 * Author: Tharshini
 *
 * Created on 10 July, 2026, 3:24 PM
 */

#ifndef DIGITAL_TIMER_H
#define	DIGITAL_TIMER_H

//screens
#define DEFAULT_SCREEN       0x01
#define CONFIG_SCREEN        0x02

//menu options
#define SET_VIEW_ALARMS           0x04
#define EDIT_ALARM                0x05
#define EVENT_SUBMENU             0x06
#define TIME_DATE_SUBMENU         0x07
#define SET_TIME_SCREEN           0x08
#define SET_DATE_SCREEN           0x09

//reset flags
#define RESET_NOTHING             0x00
#define RESET_CONFIG              0x10
#define RESET_EVENT_SUBMENU       0x12
#define RESET_SET_VIEW_ALARMS     0x13
#define RESET_EDIT_ALARM          0x14
#define RESET_TIME_DATE_SUBMENU   0x16
#define RESET_SET_TIME_SCREEN     0x17
#define RESET_SET_DATE_SCREEN     0x18

//return status
#define RETURN_SUCCESS            0xA0
#define RETURN_BACK               0xA1

#define MAX_ALARM                  10//maximum number of alarms that can be stored in EEPROM.
#define ALARM_SIZE                  8 //bytes per alarm record in EEPROM
#define ALARM_BASE_ADDR             0 //EEPROM start address for alarm table

//alarm trigger 
#define TRIGGER_ONCE               'O'
#define TRIGGER_DAILY              'D'
#define TRIGGER_WEEKLY             'W'

//buzzer output
#define BUZZER_DDR                  TRISC1
#define BUZZER                      RC1

#define EEPROM_INIT_FLAG_ADDR       (ALARM_BASE_ADDR + (MAX_ALARM*ALARM_SIZE))   //EEPROM location used to detect first power-up initialization
#define EEPROM_INIT_MAGIC           0xAA   //magic value written after EEPROM initialization

//alarm struct
typedef struct
{
    unsigned char hour; //0-23, 24hr format 
    unsigned char min; //0-59
    //alarm duration
    unsigned char dur_hr;
    unsigned char dur_min;
    unsigned char dur_sec;
    unsigned char trigger_type; //TRIGGER_ONCE / TRIGGER_DAILY / TRIGGER_WEEKLY
    unsigned char day_of_week;//1-7, Valid only for weekly alarms
    unsigned char enable;//1=Alarm enabled, 0=Disabled
} alarm_t;

//====================Function Prototypes====================
void display_default_screen(void);
unsigned char config_screen(unsigned char reset_flag,unsigned char key);
unsigned char event_submenu_screen(unsigned char reset_flag, unsigned char key);
unsigned char set_view_alarms(unsigned char key,unsigned char reset_flag);
unsigned char edit_alarm(unsigned char key,unsigned char reset_flag);
unsigned char time_date_submenu_screen(unsigned char reset_flag, unsigned char key);
unsigned char set_time_screen(unsigned char key, unsigned char reset_flag);
unsigned char set_date_screen(unsigned char key, unsigned char reset_flag);

void init_alarms(void);
static void read_alarm(unsigned char index, alarm_t *alrm);
void save_alarm(unsigned char index,unsigned char hour,unsigned char min,unsigned char dur_hr,unsigned char dur_min,unsigned char dur_sec,unsigned char trigger_type,unsigned char day_of_week);
void delete_alarm(unsigned char index);
unsigned char get_next_alarm(unsigned char *hour,unsigned char *min,unsigned char *trigger_type);
void trigger_alarms(void);


#endif	/* DIGITAL_TIMER_H */

