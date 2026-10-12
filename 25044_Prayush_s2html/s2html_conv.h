#ifndef S2HTML_CONV_H
#define S2HTML_CONV_H

#include "s2html_event.h"
/* constants */

#define HTML_OPEN 1
#define HTML_CLOSE 0

/********** function prototypes **********/

void html_begin(FILE *dest_fp, int type); /* type => not used, but can be used
                                             to add differnet HTML tags */
void html_end(FILE *dest_fp, int type); /* type => not used, but can be used to
                                           add differnet HTML tags */
void set_line_number_flag(int flag);
void source_to_html(FILE *fp, pevent_t *event);

#endif
