
#ifndef I2C_H
#define	I2C_H



#define FOSC            20000000

void init_i2c(unsigned long baud);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
unsigned char i2c_read(unsigned char ack);
int i2c_write(unsigned char data);


// ds1307

#define SLAVE_WRITE             0b11010000 // 0xD0
#define SLAVE_READ              0b11010001 // 0xD1

#define SEC_ADDR                0x00
#define MIN_ADDR                0x01
#define HOUR_ADDR               0x02

#define DATE_ADDR   0x04
#define MONTH_ADDR  0x05
#define YEAR_ADDR   0x06


void init_ds1307(void);
unsigned char read_ds1307(unsigned char addr);
void write_ds1307(unsigned char addr, unsigned char data);

void get_time(unsigned char *clock_reg);
void display_time(unsigned char *clock_reg);

void get_date(void);



#endif	/* I2C_H */

