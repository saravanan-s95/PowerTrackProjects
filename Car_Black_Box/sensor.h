/* 
 * File:   sensor.h
 * Author: Sundar
 *
 * Created on 29 September, 2026, 9:01 AM
 */

#ifndef SENSOR_H
#define	SENSOR_H

#include <stdint.h>
#include "matrix_keypad.h"
#include <xc.h>

#define MAX_GEAR 6
#define SPEED_ADC_CHANNEL 0x04
#define GEAR_UP             SWITCH1
#define GEAR_DOWN           SWITCH2
#define COLLISION           SWITCH3

uint8_t get_speed();
unsigned char get_gear_pos();

#endif	/* SENSOR_H */