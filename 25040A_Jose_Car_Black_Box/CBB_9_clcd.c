#include <xc.h>

#include "CBB_clcd.h"


//Function to initilaize CLCD
void init_clcd(void) 
{
    TRISD = 0x00;
    TRISC = TRISC & 0xF8;
    
    __delay_ms(30);
    
    RC0 = 0;
    
    clcd_write(0x33, INSTRUCTION);
    __delay_us(4100);
    clcd_write(0x33, INSTRUCTION);
    __delay_us(100);
    clcd_write(0x33, INSTRUCTION);
    __delay_us(10);                    
    
    clcd_write(0x38, INSTRUCTION);      //Function set (2 line, 5x8 display)
    __delay_us(100);
    clcd_write(0x0C, INSTRUCTION);      //Display ON, Cursor off
    __delay_us(100);
    clcd_write(0x01, INSTRUCTION);      //Clear display
    __delay_ms(2);                      //Clear display takes more time
    clcd_write(0x06, INSTRUCTION);      //Entry mode set (Increment, no shift)
    __delay_us(100);
    
    return;
}


//Function to write data to CLCD
void clcd_write(unsigned char to_write, unsigned char type)
{
    RC1 = type;
    RC0 = 0;
    PORTD = to_write;   //Data port
    RC2 = 1;            //Enable pin set to high
    RC2 = 0;            //Enable pin set to low
    
    
    TRISD = 0xFF;        //Make PORTD to input mode
    RC1 = INSTRUCTION;   //Make RS in instruction mode before checking whether write operation complete or not
    RC0 = 1;             //Make RW to read mode
    
    do
    {
        RC2 = 1;
        RC2 = 0;
    }while(RD7 == 1);
    
    RC0 = 0;            //Make RW to write mode
    TRISD = 0x00;       //Mkae PORTD in output mode
}


//Function to write a string (display a string) on CLCD 
void clcd_print_line(unsigned char* str, unsigned char address)
{
    clcd_write(address, INSTRUCTION);           //Gives the address where to print the data; address is an instruction
    
    for(unsigned char i=0; (str[i]!='\0'); i++)
    {
        clcd_write(str[i], DATA);
    }
}