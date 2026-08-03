#ifndef MKP_H
#define MKP_H

#include <xc.h>

#define row1 RD3
#define row2 RD2
#define row3 RD1
#define row4 RD0

#define col1 RB2
#define col2 RB1
#define col3 RB0

#define LEVEL 0
#define EDGE  1

#define KEY_1   '1'
#define KEY_2   '2'
#define KEY_3   '3'

#define KEY_4   '4'
#define KEY_5   '5'
#define KEY_6   '6'

#define KEY_7   '7'
#define KEY_8   '8'
#define KEY_9   '9'

#define KEY_STAR '*'
#define KEY_0    '0'
#define KEY_HASH '#'


#define UP_KEY      KEY_2
#define DOWN_KEY    KEY_8
#define LEFT_KEY    KEY_4
#define RIGHT_KEY   KEY_6
#define ENTER_KEY   KEY_HASH
#define BACK_KEY    KEY_STAR

#define ALL_RELEASED 0xFF

void init_mkp(void);
unsigned char read_mkp(unsigned char mode);

#endif
