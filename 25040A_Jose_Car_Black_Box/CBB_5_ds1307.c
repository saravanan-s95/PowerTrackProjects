#include <xc.h>

#include "CBB_main.h"
#include "CBB_ds1307.h"
#include "CBB_i2c.h"

/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */

unsigned char clock_reg[3];
extern unsigned char print_details[17];


//Function to set time to 15:30:00
void set_time(void)
{
    unsigned char dummy = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, dummy | 0x80);           //Stop the RTC before writing
    
    //To set time to 15:30:00 (24-Hour format)
    write_ds1307(HOUR_ADDR, 0x15);      //Write the hour 15 to hour address of RTC
    write_ds1307(MIN_ADDR, 0x30);       //Write the minute 30 to minute address of RTC
    write_ds1307(SEC_ADDR, 0x00);       //Write the second 00 to seconds address of RTC & start RTC
}


//Function to get time from RTC
void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);      //Read the hour value from hour address of RTC
	clock_reg[1] = read_ds1307(MIN_ADDR);       //Read the minute value from minute address of RTC
	clock_reg[2] = read_ds1307(SEC_ADDR);       //Read the seconds value from seconds address of RTC

	if (clock_reg[0] & 0x40)            //Check if 12-hour format
	{
		print_details[8] = '0' + ((clock_reg[0] >> 4) & 0x01);      //Convert the hour value to ASCII
		print_details[9] = '0' + (clock_reg[0] & 0x0F);
	}
	else            //Else, it is 24-hour format
	{
		print_details[8] = '0' + ((clock_reg[0] >> 4) & 0x03);      //Convert the hour value to ASCII
		print_details[9] = '0' + (clock_reg[0] & 0x0F);
	}
	print_details[10] = ':';
	print_details[11] = '0' + ((clock_reg[1] >> 4) & 0x0F);         //Convert the minute value to ASCII
	print_details[12] = '0' + (clock_reg[1] & 0x0F);
	print_details[13] = ':';
	print_details[14] = '0' + ((clock_reg[2] >> 4) & 0x0F);         //Convert the seconds value to ASCII
	print_details[15] = '0' + (clock_reg[2] & 0x0F);
	print_details[16] = '\0';
}


//Function to initialize DS1307 RTC
void init_ds1307(void)
{
	unsigned char dummy;	
	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy | 0x80);   //Setting the CH bit of the RTC to Stop the Clock

	//Seting 24 Hr Format
	dummy = read_ds1307(HOUR_ADDR);
    write_ds1307(HOUR_ADDR, dummy & 0x1F);      //To 24-hr format

	/* 
	 * Control Register of DS1307
	 * Bit 7 - OUT
	 * Bit 6 - 0
	 * Bit 5 - OSF
	 * Bit 4 - SQWE
	 * Bit 3 - 0
	 * Bit 2 - 0
	 * Bit 1 - RS1
	 * Bit 0 - RS0
	 * 
	 * Setting RS0 and RS1 as 11 to achieve SQW out at 32.768 KHz
	 */ 
	write_ds1307(CNTL_ADDR, 0x93); 

	dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy & 0x7F);       //Clearing the CH bit of the RTC to Start the Clock

}


//Function to write data to DS1307 RTC using I2C Protocol
void write_ds1307(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
}


//Function to read data from DS1307 RTC using I2C Protocol
unsigned char read_ds1307(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ);
	data = i2c_read();
	i2c_stop();

	return data;
}



