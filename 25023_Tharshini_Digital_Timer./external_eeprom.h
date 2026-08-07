/*
  File: external_eeprom.h
  Description: Header file for EEPROM interface functions
*/

#ifndef external
#define external

#define SLAVE_READ_E		0xA1
#define SLAVE_WRITE_E		0xA0


void write_external_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_external_eeprom(unsigned char address1);
void external_eeprom_str_write(unsigned char addr, char *str);
void external_eeprom_str_read(unsigned char addr, char *str,unsigned char len);

#endif
