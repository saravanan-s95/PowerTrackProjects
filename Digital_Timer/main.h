#ifndef MAIN_H
#define MAIN_H
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#define _XTAL_FREQ 20000000UL
#define MAX_ALARMS 5
typedef struct {
    uint8_t sec, min, hour, weekday, date, month, year;
} RTC_Time;
typedef struct {
    uint8_t hour, minute, weekday, type, enabled;
} Alarm;
#endif
