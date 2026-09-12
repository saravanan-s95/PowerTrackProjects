#include <xc.h>
#include "can_N3.h"
#include "main.h"


/* Global Variables */
//unsigned char can_payload[13];

typedef enum _CanOpMode
{
	e_can_op_mode_bits    = 0xE0,			/* Use this to access opmode bits */
	e_can_op_mode_normal  = 0x00,
	e_can_op_mode_sleep   = 0x20,
	e_can_op_mode_loop    = 0x40,
	e_can_op_mode_listen  = 0x60,
	e_can_op_mode_config  = 0x80
} CanOpMode;

/* Configure the CAN Module */
void init_can(void)
{
	/* CAN_TX = RB2, CAN_RX = RB3 */
	TRISB2 = 0;								/* CAN TX */
	TRISB3 = 1;								/* CAN RX */

	/* Enter CAN module into config mode */
							 /* clear previous mode */
	  CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_config);                     	/* set new mode */

	/* Wait untill desired mode is set */
	while (CANSTAT != 0x80);

	/* Enter CAN module into Mode 0 */
	ECANCON = 0x00;

    /*
	//Initialize CAN Timing 8MHz
	BRGCON1 = 0xE1;							//1110 0001, SJW=4, TQ, BRP 4 
	BRGCON2 = 0x1B;							//0001 1011, SEG2PHTS 1 sampled once PS1=4TQ PropagationT 4TQ
	BRGCON3 = 0x03;							//0000 0011, PS2, 4TQ
    */
    
    
    //Corrected CAN Timing for 20MHz Clock @ 250 Kbps
    BRGCON1 = 0x01;     // BRP = 1, SJW = 1TQ
    BRGCON2 = 0x73;     // Phase Segment 1 = 7TQ, Prop Seg = 4TQ
    BRGCON3 = 0x07;     // Phase Segment 2 = 8TQ
    
    
	/*
	 * Enable Filters
	 * Filter 0
	 */
	RXFCON0 = 0x01;     

	/*
	 * Initialize Receive Filters
	 * Filter 0 = 0xFFC
	 */
	RXF0EIDH = 0x00;
	RXF0EIDL = 0x00;
	RXF0SIDH = 0x6B;
	RXF0SIDL = 0xC0;
    
    
    RXM0SIDH = 0x00;    //To bypass filtering
    RXM0SIDL = 0x00;

	/* Enter CAN module into Loop back mode */
	CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_normal);

	/* Set Receive Mode for buffers */
	RXB0CON = 0x00;
}


unsigned char can_receive(unsigned char* data_length, char* received_data, unsigned int* msg_id)
{
	unsigned char rx_msg_flag = 0;
    
    unsigned char msg_id_H = 0;
    unsigned char msg_id_L = 0;

	if (RXB0FUL) //CheckRXB0 
	{
        //*data_length = RXB0DLC;
        *data_length = RXB0DLC & 0x0F;  // Safely extract lower 4 bits
           
        msg_id_H = RXB0SIDH;
		msg_id_L = RXB0SIDL;    
        *msg_id   = ((msg_id_H << 3) | (msg_id_L >> 5));        
        
		switch(*data_length)
        {
            case 8 : received_data[7] = RXB0D7;
            case 7 : received_data[6] = RXB0D6;
            case 6 : received_data[5] = RXB0D5;
            case 5 : received_data[4] = RXB0D4;
            case 4 : received_data[3] = RXB0D3;
            case 3 : received_data[2] = RXB0D2;
            case 2 : received_data[1] = RXB0D1;
            case 1 : received_data[0] = RXB0D0;
            case 0 : break;
        }
        received_data[*data_length] = '\0';

        RXB0FUL = 0;
		RXB0IF = 0;
        
		return TRUE;
	}
	else
	{
		return FALSE;
	}    
}
