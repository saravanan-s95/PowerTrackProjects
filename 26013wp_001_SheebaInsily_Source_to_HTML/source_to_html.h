#ifndef SOURCE_TO_HTML_H
#define SOURCE_TO_HTML_H

#include <stdio.h>
#include "parser.h"

/* Generate HTML output from parser events */
void source_to_html(FILE *fp, event_t *event);

/* Write HTML header */
void write_html_header(FILE *fp);

/* Write HTML footer */
void write_html_footer(FILE *fp);

#endif