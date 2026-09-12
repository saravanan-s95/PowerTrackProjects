#include <xc.h>
#include "can_N.h"
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


void can_transmit(unsigned char data_length, char* data, unsigned int msg_id)
{
	while(TXB0REQ == 1);   //Wait until the previous transmission is 100% complete
    
    TXB0EIDH = 0x00;		/* Extended Identifier */
	TXB0EIDL = 0x00;		/* Extended Identifier */
    
    TXB0SIDH = msg_id >> 3;		/* Standard Identifier */
	TXB0SIDL = msg_id << 5;		/* Standard Identifier */
    
	TXB0DLC = data_length;			/* Data Length Count */
    
    switch(data_length)
    {
        case 8 : TXB0D7 = data[7];
        case 7 : TXB0D6 = data[6];
        case 6 : TXB0D5 = data[5];
        case 5 : TXB0D4 = data[4];
        case 4 : TXB0D3 = data[3];
        case 3 : TXB0D2 = data[2];
        case 2 : TXB0D1 = data[1];
        case 1 : TXB0D0 = data[0];
        
        case 0 : break;
    }

	TXB0REQ = 1;			/* Set the buffer to transmit */
}
