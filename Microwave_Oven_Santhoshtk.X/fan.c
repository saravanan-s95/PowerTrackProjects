#include <xc.h>
#include "fan.h"

void init_fan(void){
    FAN_DDR = 0;
    FAN = 0;
}