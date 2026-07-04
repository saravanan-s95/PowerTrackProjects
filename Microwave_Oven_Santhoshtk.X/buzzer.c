#include <xc.h>
#include "buzzer.h"

void init_buzz(void){
    BUZZ_DDR = 0;
    BUZZ = 0;
}