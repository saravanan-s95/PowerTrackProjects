/* 
 * File:   functions.h
 * Author: dhiva
 *
 * Created on 6 November, 2026, 6:16 PM
 */



#ifndef FUNCTIONS_H
#define	FUNCTIONS_H




// Structure: Time_t
// Purpose: Stores Current Time - Hour, Minute, Second and AM/PM Flag
typedef struct 
{
    unsigned char hour;   // Hour Value (1 - 12)
    unsigned char min;    // Minute Value (0 - 59)
    unsigned char sec;    // Second Value (0 - 59)
    unsigned char am_pm;  // AM/PM Flag (0 = AM, 1 = PM)
}Time_t;

// Structure: Date_t
// Purpose: Stores Current Date - Day, Month and Year
typedef struct 
{
    unsigned char day;    // Day Value (1 - 31)
    unsigned char month;  // Month Value (1 - 12)
    unsigned int year;    // Year Value (e.g. 2016, 2000 - 2099)
}Date_t;

// Structure: Event_t
// Purpose: Stores Event / Alarm Details
// Type: 0 = Once (O), 1 = Daily (D), 2 = Weekly (W)
typedef struct 
{
    unsigned char sec;            // Event Trigger Second (Usually 0)
    unsigned char min;            // Event Trigger Minute (0 - 59)
    unsigned char hour;           // Event Trigger Hour (1 - 12)
    unsigned char duration_min;   // Alarm Duration - Minutes Part
    unsigned char duration_hour;  // Alarm Duration - Hours Part
    unsigned int duration_sec;    // Alarm Duration - Seconds Part
    unsigned char type;           // Event Type (0 = Once, 1 = Daily, 2 = Weekly)
    unsigned char enabled;        // Event Status (1 = ON / Enabled, 0 = OFF / Disabled)
}Event_t;

// Global Variables - Defined in main.c, Shared with Other Files
// 'extern' Means Variable is Defined in Another File, But Can be Used Here
extern Time_t myTime;              // Current Time Variable
extern Date_t myDate;              // Current Date Variable
extern Event_t myEvent[2];         // Array of 2 Events (Event 1 and Event 2)

extern unsigned char config_select;         // Selection in CONFIG Menu (0 = SET/VIEW, 1 = TIME/DATE)
extern unsigned char blink_flag;             // Flag for Blinking Field in Edit Mode
extern unsigned char field;                  // Current Editable Field (Hour/Min/Sec etc.)
extern unsigned char key;                    // Current Key Status (SW1 / SW2 / Released)
extern unsigned int up_press_count;          // Counter for SW1 Press Duration
extern unsigned int down_press_count;        // Counter for SW2 Press Duration
extern unsigned int idle_count;              // Counter for Idle Timeout (Auto Return to Default)
extern unsigned char event_display_flag;     // Flag for Alternate Event Display on Default Screen
extern unsigned char set_time_date_select;   // Selection in TIME/DATE Menu (0 = TIME, 1 = DATE)
extern unsigned char set_view_select;        // Selection in SET/VIEW Menu (0 = SET, 1 = VIEW)
extern unsigned char event_num;              // Current Event Number (0 = Event 1, 1 = Event 2)

// Function Prototypes - Functions Defined in functions.c

// Show Default Clock Screen - Date and Time
void show_Default(void);

// Show CONFIG Menu - SET VIEW EVENT / SET TIME DATE
void show_config(void);

// Show SET / VIEW EVENT Menu
void show_set_view_event(void);

// Edit Event Details - Time, Type, Duration
void set_event(void);

// Display Event Details in Read-Only Mode
void view_event(void);

// Show SET TIME / SET DATE Menu
void show_set_time_date(void);

// Edit Current Time - HH:MM:SS
void set_time(void);

// Edit Current Date - DD-MM-YY (Named stop_date in Code)
void stop_date(void);

// Increment Clock by 1 Second
void Increment_Time(void);

// Check if Current Time Matches Event Time, Turn ON Buzzer
void Check_Alarm(void);

// Stop Buzzer Manually When Any Key is Pressed
void stop_alarm(void);


#endif	/* FUNCTIONS_H */

