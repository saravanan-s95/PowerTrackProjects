#ifndef MAIN_H
#define MAIN_H
#include<xc.h>
#include "I2C.h"
#include "RTC.h"
#include "clcd.h"
#include "eeprom.h"
#include "matrix.h"
#include <string.h>
typedef struct event
{
    unsigned char time[10];
    unsigned char date[14];
    unsigned char ch;
    
}event;
void select_view_or_set_event();
void view_entrys(unsigned char add);
void timer2();
#endif