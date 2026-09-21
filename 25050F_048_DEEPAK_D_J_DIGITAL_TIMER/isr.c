#include<xc.h>

static unsigned int count;
static unsigned int count2;
static unsigned int count1;

unsigned char sec_tick = 0;
unsigned char show_event_flag = 0;


unsigned char ch;
__interrupt() void isr(void)
{ 
   // -------------------------
    // Check Timer 0 Interrupt
    // -------------------------
//    if(TMR0IF) 
//    {
//        TMR0 = TMR0 + 6 + 2;
//        if(++count == 10000) 
//        {
//            count = 0;
//            RD0 = !RD0; 
//        }
//        TMR0IF = 0; // Clear the flag
//    }
    
    // -------------------------
    // Check Timer 1 Interrupt
    // -------------------------
    if(TMR1IF) 
    {
        TMR1 = TMR1 + 3036 + 2;
       if(count1++ == 80)
        {
            count1 = 0;
            sec_tick++;
            if (show_event_flag == 0 && sec_tick >= 5) {
                show_event_flag = 1;
                sec_tick = 0;
            } 
            else if (show_event_flag == 1 && sec_tick >= 2) {
                show_event_flag = 0;
                sec_tick = 0;
            }
        }
        TMR1IF = 0;
    }

    // -------------------------
    // Check Timer 2 Interrupt
    // -------------------------
//    if(TMR2IF) 
//    {
//        if(++count2 == 20000) 
//        { 
//            count2 = 0;
//            RD2 = !RD2; 
//        }
//        TMR2IF = 0; // Clear the flag
//    }
}
//__interrupt() void isr2(void)
//{ 
//    if(TMR2IF)
//    {
//        //TMR0 = TMR0+6+2; 
//       if(++count2 == 20000) // scaling use 5000 for timer 2
//        {
//            count2 = 0;
//            RD2 = !RD2;
//        }
//        TMR2IF = 0;
//    }
//}
//__interrupt() void isr1(void){
//   // unsigned long int count =0;
//    if(TMR1IF){
//        TMR1 = TMR1 + 3036 +2;
//        if(count1++ == 80){
//            RD1 = !RD1;
//            count1 = 0;
//        }
//        TMR1IF = 0;
//    }
//}