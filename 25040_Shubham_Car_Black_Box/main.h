#ifndef MAIN_H
#define MAIN_H
#include <string.h>
#include <xc.h>
#include "I2C.h"
#include "adc.h"
#include "clcd.h"
#include "RTC.h"
#include "matrix.h"
#include "eeprom.h"
#include "uart.h"
unsigned short read_gear(unsigned short gear,unsigned char sw);
int sub_node(unsigned short mode);

#endif