#include<xc.h>
#include "newfile.h"

void init_digital_keypad(void)
{
    KEY_PORT_DDR = KEY_PORT_DDR | 0x3f;
    
}

unsigned char once = 1;
unsigned char read_digital_keypad(unsigned char mode){
    if(mode == LEVEL){
        //if(RB0 == 1) return 1;
        for(int i = 500;i--;);
        return (PORTB & 0x3f);
    }
    else if(mode == STATE){
        if((PORTB&0x3F)!= ALL_RELEASED && once){
            once =0;
            for(int i = 500;i--;);
            return (PORTB & 0x3F);
        }else if((PORTB&0x3F)== ALL_RELEASED){
            once =1;
        }
    }
    for(int i= 500;i--;);
    return ALL_RELEASED;
}