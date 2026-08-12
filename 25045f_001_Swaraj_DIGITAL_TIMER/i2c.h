/* 
 * File:   i2c.h
 * Author: Dell
 *
 * Created on 11 August, 2026, 12:07 PM
 */

#ifndef I2C_H
#define I2C_H

void init_i2c(void);

void i2c_start(void);
void i2c_stop(void);

void i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char ack);

#endif