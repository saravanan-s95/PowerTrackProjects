#ifndef MAIN_H
#define	MAIN_H

#define COLLISION      7

typedef enum {
    DASHBOARD,
    DISPLAY_MENU,
    VIEW_LOG,
    DOWNLOAD_LOG,
    SET_LOG,
    CLEAR_LOG
} SystemState_t;


void mode_based_working(void);

void display_menu(void);

void shift_entries(void);

void view_log_eeprom_contents(void);
void view_log_eeprom_scrolling(void);

void download_log_using_uart(void);

void check_whether_first_poweron(void);

#endif

