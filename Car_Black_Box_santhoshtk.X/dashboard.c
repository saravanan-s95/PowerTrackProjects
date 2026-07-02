/*
 * File:   dashboard.c
 * Author: 25014
 *
 * Created on 31 December, 2025
 */
#include <pic.h>
#include <string.h>
#include <xc.h>
#include "adc.h"
#include "auth.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "main.h"
#include "uart.h"


/* GLOBALS */
unsigned char clock_reg[3], speed[3] = {0};
const char  *event[] = { 
	"ON",  
	"GN", 
	"G1", 
	"G2", 
	"G3", 
	"G4", 
	"GR", 
	"C "
};
const unsigned char *menu_options[5] = {
		"View Log      ", 
		"Clear Log     ", 
		"Download Log  ", 
		"Set Time      ", 
		"Change Pass   "
};

int pos = 0;

unsigned int adc_val;
unsigned int event_count;
char logs[17];


/* EXTERNAL LINKAGES*/
extern char time[9];
extern unsigned char access;


// To read the parameter values
// NT: Authentication Required
char *read_event(unsigned int addr){
	static char event[11];

	for(unsigned int i = 0; i < 10; i++){
		event[i] = read_external_eeprom(addr + i);
	}

	return event;
}


// To write the parameter values into External EEPROM upon event
void write_event(void){

	static unsigned int address = 0x00;

	
	// FIFO Elimination of Existing log
	// Bytes Allocated for Logs: 100 (0-99)
	// Once filled it starts overwriting on First Entry
	if(address == 100) address = 0x00; 

	// Store time HHMMSS
	for(unsigned int i = 0; i < 8; i++){
		if(i == 2 || i == 5) continue;
		else write_external_eeprom(address++, time[i]);	
	}

	// Store Event
	write_external_eeprom(address++, event[pos][0]);
	write_external_eeprom(address++, event[pos][1]);

	// Store Speed
	write_external_eeprom(address++, speed[0]);
	write_external_eeprom(address++, speed[1]);

	event_count = (event_count + 1) % 10;
}


void dashboard(unsigned char key) {
    
	clcd_print(" TIME     EV  SP", LINE1(0));
	// clcd_print(read_event(0x00), LINE1(0)); // Test
	
	// display time
	get_time(clock_reg);
	display_time(clock_reg);

	// Check for key
	// Increase the gear
	if(key == SW1){

		// C -> GN
		if(pos == 7){
			pos = 1;
		}
		else if(pos < 6){
			pos++;
			write_event();
		}
	}

	else if(key == SW2){
		// C -> GN
		if(pos == 7){
			pos = 1;
		}
		else if(pos > 1){
			pos--;
			write_event();
		}
	}

	// Collision
	else if(key == SW3){
		pos = 7;
		write_event();
	}
	
	// event display ( Gear pos and speed )
	clcd_print(event[pos], LINE2(10));


	// ADC -> Speed
	adc_val = read_adc(CHANNEL0);
	
	// Speed
	adc_val = adc_val / 10.23; // (ADC Range: 1023 / 10.23 = 100)

	// Since, the range we will be displaying is (0-99)
	if(adc_val == 100) adc_val--;

	speed[0] = (adc_val / 10) + '0'; // 1st digit
	speed[1] = (adc_val % 10) + '0';
	speed[2] = '\0';

	clcd_print(speed, LINE2(14));
}


void print_star(unsigned int position){
	if(position == 0){
		clcd_print("* ", LINE1(0));
		clcd_print("  ", LINE2(0));
	}
	else{
		clcd_print("* ", LINE2(0));
		clcd_print("  ", LINE1(0));
	}
}


void main_menu(void){

	auth_user();
	if(access == DENIED) return;

	unsigned int menu_index = 0;
	unsigned char star = 0;
	unsigned char key;
	
	clear_screen();
	
	while(1){
		// Display Menu Options
		print_star(star);
		clcd_print(menu_options[menu_index], LINE1(2));
		clcd_print(menu_options[menu_index + 1], LINE2(2));

		key = read_digital_keypad(STATE);

		if(key == SW1){
			if(star == 1){
				star = 0;
			}
			else if(menu_index > 0) menu_index--;
		}

		else if(key == SW2){

			if(star == 0){
				star = 1;
			}
			else if(menu_index < MAX_MENU - 2) menu_index++;
		}


		else if(key == SW4){
			
			unsigned int selected_menu = menu_index + star;
			clear_screen();

			if(selected_menu == 0){
				view_log();
			}
			else if(selected_menu == 1){
				clear_log();
			}
			else if(selected_menu == 2){
				download_log();
			}
			else if(selected_menu == 3){
				set_time();
			}
			else if(selected_menu == 4){
				change_password();
			}

		}

		else if(key == SW5){
			clear_screen();
			access = DENIED;
			return;
		}
	}
}


void read_and_format_log(unsigned char index){

	char *log = read_event(index * 10);
	unsigned int col = 0;

	for(unsigned int i = 0; i < 16; i++){

		if(i == 0) logs[i] = index + '0';

		else if(i == 4 || i == 7) logs[i] = ':';

		else if(i == 1 || i == 10 || i == 13) logs[i] = ' ';

		else{
			logs[i] = log[col++];
		}
	}

	logs[16] = '\0';
}


void view_log(void){

	// read and reformat the events from EEPROM
	char *log;
	unsigned int curr_log = 0;
	unsigned int key;

	// Initial log read
	read_and_format_log(curr_log);

	while(1){

		// If no logs
		if(event_count == 0){
			clcd_print("No Logs", LINE1(0));
			clcd_print("Available", LINE2(0));
		}

		else{
			clcd_print("# TIME     EV SP", LINE1(0));
			clcd_print(logs, LINE2(0));
		}

		// Poll for key press
		key = read_digital_keypad(STATE);

		if(key == SW1){
			if(curr_log > 0) curr_log--;
			read_and_format_log(curr_log);
		}

		else if(key == SW2){
			if(curr_log < event_count - 1) curr_log++;
			read_and_format_log(curr_log);
		}
		else if(key == SW5){
			// back to main menu
			return;
		}

	}

}


void clear_log(void){
	event_count = 0;
	clcd_print("Clearing Logs...", LINE1(0));
	__delay_ms(500);
	return;
}


void download_log(void){

	clcd_print("Logs are", LINE1(0));
	clcd_print("Downloading...", LINE2(0));

	// Send Title to the Serial port
	put_s("# TIME     EV SP\n");

	// Send it to Serial Port using UART
	for(unsigned int i = 0; i < event_count; i++){
		read_and_format_log(i);
		put_s(logs);
		put_char('\n');

		__delay_ms(500);
	}

	return;
}

void display_indicator(unsigned int pos){
	if(pos == 0){
		// Hour
		clcd_print("  ", LINE2(4));
	}
	else if(pos == 1){
		// Min
		clcd_print("  ", LINE2(7));
	}
	else if(pos == 2){
		// Sec
		clcd_print("  ", LINE2(10));
	}
	
}

void set_time(void){

	// Get the current time and convert to INT
	unsigned int hour = TIME_TO_INT(time[0], time[1]);
	unsigned int min = TIME_TO_INT(time[3], time[4]);
	unsigned int sec = TIME_TO_INT(time[6], time[7]);
	unsigned int indicator = 0;
	unsigned char key;

	clcd_print("    HH:MM:SS    ", LINE1(0));

	while(1){

		// Display Hour
		clcd_putch((hour / 10) + '0', LINE2(4));
		clcd_putch((hour % 10) + '0', LINE2(5));
		clcd_putch(':', LINE2(6));

		// Display Minute
		clcd_putch((min / 10) + '0', LINE2(7));
		clcd_putch((min % 10) + '0', LINE2(8));
		clcd_putch(':', LINE2(9));
		
		// Display Seconds
		clcd_putch((sec / 10) + '0', LINE2(10));
		clcd_putch((sec % 10) + '0', LINE2(11));

		display_indicator(indicator);


		// Poll for key
		key = read_digital_keypad(STATE);

		if(key == SW1){
			if(indicator == 0) hour = (hour + 1) % 24;

			if(indicator == 1) min = (min + 1) % 60;

			if(indicator == 2) sec = (sec + 1) % 60;
		}

		else if(key == SW2){
			indicator  = (indicator + 1) % 3;
		}

		else if(key == SW4){
			write_ds1307(HOUR_ADDR, DIGIT_TO_BCD(hour));
			write_ds1307(MIN_ADDR, DIGIT_TO_BCD(min));
			write_ds1307(SEC_ADDR, DIGIT_TO_BCD(sec));

			return;
		}

		else if(key == SW5){
			// Don't save time just go back
			return;
		}
	}
}


char *collect_password(unsigned int re_enter){

	if(re_enter){
		clcd_print("RE-ENTER NEW PASS", LINE1(0));
	}
	else{
		clcd_print(" ENTER NEW PASS ", LINE1(0));
	}


	static char password[5];
	unsigned int curr_pass_index = 0;
	unsigned char key;

	while(1){

		key = read_digital_keypad(STATE);

		// Blink _
		clcd_putch('_', LINE2(curr_pass_index));
		clcd_putch(' ', LINE2(curr_pass_index));

		if(key == SW1){
			clcd_putch('*', LINE2(curr_pass_index));
			password[curr_pass_index++] = '0';
		}
		else if(key == SW2){
			clcd_putch('*', LINE2(curr_pass_index));
			password[curr_pass_index++] = '1';
		}

		// If 4 digits are entered
		if(curr_pass_index > 3){
			__delay_ms(1000);
			break;
		}
	}

	return password;

}


void change_password(void){

	char new_password[5];
	char new_password_re_enter[5];

	strcpy(new_password, collect_password(0));
	clear_screen();
	strcpy(new_password_re_enter, collect_password(1));
	clear_screen();

	// Compare both passwords
	if(strcmp(new_password, new_password_re_enter) == 0){
		change_original_password(new_password);
		clcd_print("PASSWORD CHANGED", LINE1(0));
		__delay_ms(1500);
		return;
	}
	else{
		clcd_print("PASSWORD MISMATCH", LINE1(0));
		__delay_ms(1500);
		return;
	}

}