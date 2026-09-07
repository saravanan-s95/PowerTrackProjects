/* 
 * File:   eeprom.h
 */

#ifndef EEPROM_H
#define	EEPROM_H

#define SLAVE_READ_E		0xA1
#define SLAVE_WRITE_E		0xA0

void write_external_eeprom(unsigned char address1,  unsigned char data);
void eeprom_write_str(unsigned char addr, char *data);
unsigned char read_external_eeprom(unsigned char address1);

#endif	/* EEPROM_H */

