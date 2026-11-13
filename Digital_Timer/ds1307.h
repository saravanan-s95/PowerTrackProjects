#ifndef DS1307_H
#define DS1307_H
#include "main.h"
void ds1307_init(void);
bool ds1307_read(RTC_Time *time);
void ds1307_write(const RTC_Time *time);
#endif
