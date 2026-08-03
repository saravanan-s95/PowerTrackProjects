#include "main.h"

void init_mkp(void)
{
    TRISB |=0x07;
    TRISD &=0xF0;
    
    row1 = hi;
    row2 = hi;
    row3 = hi;
    row4 = hi;
}

static unsigned char scan_key(void)
{
    unsigned int i;
    
    row1 = low;
    row2 = hi;
    row3 = hi;
    row4 = hi;
    for(i = 100;i--;);
    if(col3 == low)
        return 1;
    else if(col2 == low)
        return 2;
    else if(col1 == low)
        return 3;
    
    row1 = hi;
    row2 = low;
    row3 = hi;
    row4 = hi;
    for(i = 100;i--;);
    if(col3 == low)
        return 4;
    else if(col2 == low)
        return 5;
    else if(col1 == low)
        return 6;
    
    
    row1 = hi;
    row2 = hi;
    row3 = low;
    row4 = hi;
    for(i = 100;i--;);
    if(col3 == low)
        return 7;
    else if(col2 == low)
        return 8;
    else if(col1 == low)
        return 9;
    
    
    row1 = hi;
    row2 = hi;
    row3 = hi;
    row4 = low;
    for(i = 100;i--;);
    if(col3 == low)
        return '*';
    else if(col2 == low)
        return 0;
    else if(col1 == low)
        return '#';
    
    return ALL_RELEASED;
}

unsigned char read_mkp(unsigned char mode)
{
    static unsigned char once = 1;
    unsigned char key;
    key = scan_key();
    
    if(mode == LEVEL)
    {
        return key;
    }
    else
    {
        if((key != ALL_RELEASED)&& once)
        {
            once = 0;
            return key;
        }
        else if(key == ALL_RELEASED)
        {
            once = 1;
        }
    }
    return ALL_RELEASED;
}
