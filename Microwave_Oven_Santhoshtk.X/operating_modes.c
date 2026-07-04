#include <xc.h>
#include "dashboard.h"
#include "main.h"

extern u_i min;
extern u_i sec;


// Mode of operations
void micro_mode(void){

    clear_screen();
    clcd_print("  Power = 900W  ", LINE2(0));
    __delay_ms(1000);

    set_time_screen();

    // Start coocking   
    coocking_screen();

    clear_screen();

}


void convection_mode(void){
    set_temp_screen();
    pre_heat_screen();
    set_time_screen();
    coocking_screen();
}


void grill_mode(void){
    set_time_screen();
    coocking_screen();
}


void start_mode(void){
    sec = 30;
    coocking_screen();
}