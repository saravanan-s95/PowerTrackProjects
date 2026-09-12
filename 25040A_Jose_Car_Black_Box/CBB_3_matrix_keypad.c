#include <xc.h>

#include "CBB_main.h"
#include "CBB_matrix_keypad.h"

extern unsigned char gear;
extern unsigned char switch_read;


//Function to read the gear from Matrix Keypad
void read_gear(unsigned char* print_details)
{
    if(switch_read == 1)        //If MKP1, increment gear 
    {
        if(gear < 6)
        {
            gear++;
        }
        else 
        {
            gear = 6;           
        }
    }
    else if(switch_read == 2)       //If MKP2, decrement gear 
    {
        if(gear > 0)
        {
            gear--;
        }
        else
        {
            gear = 0;
        }
    }
    else if(switch_read == 3)       //If MKP3, it represents collision 
    {
        gear = COLLISION;
    }
    
        //Convert the gear data to ASCII 
        switch(gear)
        {
            case 0: print_details[5] = 'N';
                    break;
            case 1: print_details[5] = 1+'0';
                    break;
            case 2: print_details[5] = 2+'0';
                    break;
            case 3: print_details[5] = 3+'0';
                    break;
            case 4: print_details[5] = 4+'0';
                    break;
            case 5: print_details[5] = 5+'0';
                    break;
            case 6: print_details[5] = 'R';
                    break;
            case 7: print_details[5] = 'C';
                    break;
        }   
    
    print_details[6] = ' ';
    print_details[7] = ' ';
}



//Function to initialize Matrix_Keypad
void init_matrix_keypad(void)
{
	ADCON1 = 0x0F;      //Configure PORTB as digital

	TRISB = 0x1E;       //Set Rows (RB7 - RB5) as Outputs and Columns (RB4 - RB1) as Inputs
	
	RBPU = 0;           //Set PORTB input as pull up for columns 

	MATRIX_KEYPAD_PORT = MATRIX_KEYPAD_PORT | 0xE0;
}


//Function to detect key press
unsigned char scan_key(void)
{
	ROW1 = LO;
	ROW2 = HI;
	ROW3 = HI;

	if (COL1 == LO)
	{
		return 1;
	}
	else if (COL2 == LO)
	{
		return 4;
	}
	else if (COL3 == LO)
	{
		return 7;
	}
	else if (COL4 == LO)
	{
		return 10;
	}

	ROW1 = HI;
	ROW2 = LO;
	ROW3 = HI;

	if (COL1 == LO)
	{
		return 2;
	}
	else if (COL2 == LO)
	{
		return 5;
	}
	else if (COL3 == LO)
	{
		return 8;
	}
	else if (COL4 == LO)
	{
		return 11;
	}

	ROW1 = HI;
	ROW2 = HI;
	ROW3 = LO;

	ROW3 = LO;

	if (COL1 == LO)
	{
		return 3;
	}
	else if (COL2 == LO)
	{
		return 6;
	}
	else if (COL3 == LO)
	{
		return 9;
	}
	else if (COL4 == LO)
	{
		return 12;
	}

	return 0xFF;
}


//Function to read switch based on edge/level triggering
unsigned char read_switches(unsigned char detection_type)
{
	static unsigned char once = 1, key;

	if (detection_type == STATE_CHANGE)
	{
		key = scan_key();
		if(key != 0xFF && once  )
		{
			once = 0;
			return key;
		}
		else if(key == 0xFF)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL_CHANGE)
	{
		return scan_key();
	}

	return 0xFF;
}


