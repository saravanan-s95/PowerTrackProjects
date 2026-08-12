/* 
 * File:   rtc.h
 * Author: Dell
 *
 * Created on 11 August, 2026, 12:17 PM
 */

#ifndef RTC_H
#define RTC_H

#define DS1307_WRITE    0xD0
#define DS1307_READ     0xD1

void rtc_init(void);
unsigned char rtc_read(unsigned char address);
void rtc_write(unsigned char address, unsigned char data);

unsigned char dec_to_bcd(unsigned char data);
unsigned char bcd_to_dec(unsigned char data);

#endif