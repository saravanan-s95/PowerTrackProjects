#ifndef ALARM_H
#define ALARM_H
#include "main.h"
void alarm_init(void);
void alarm_check(const RTC_Time *now);
void alarm_view_menu(void);
uint8_t alarm_get_count(void);
#endif
