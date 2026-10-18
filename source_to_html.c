#include <stdio.h>
#include <string.h>
#include "source_to_html.h"

/*
 * Write the beginning of the HTML file
 */
void write_html_header(FILE *fp)
{
    fprintf(fp, "<!DOCTYPE html>\n");
    fprintf(fp, "<html>\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "<meta charset=\"UTF-8\">\n");
    fprintf(fp, "<title>Source Code</title>\n");
    fprintf(fp, "<link rel=\"stylesheet\" href=\"style.css\">\n");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
    fprintf(fp, "<pre>\n");
}


/*
 * Write the end of the HTML file
 */
void write_html_footer(FILE *fp)
{
    fprintf(fp, "</pre>\n");
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");
}


/*
 * Escape special HTML characters
 */
static void print_escaped(FILE *fp, const char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        switch (str[i])
        {
            case '&':
                fprintf(fp, "&amp;");
                break;

            case '<':
                fprintf(fp, "&lt;");
                break;

            case '>':
                fprintf(fp, "&gt;");
                break;

            case '"':
                fprintf(fp, "&quot;");
                break;

            default:
                fputc(str[i], fp);
                break;
        }

        i++;
    }
}


/*
 * Convert parser event into HTML
 */
void source_to_html(FILE *fp, event_t *event)
{
    if (fp == NULL || event == NULL)
    {
        return;
    }

    switch (event->type)
    {
        case PEVENT_PREPROCESSOR_DIRECTIVE:
            fprintf(fp, "<span class=\"preprocess_dir\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_MULTI_LINE_COMMENT:
            fprintf(fp, "<span class=\"comment\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_SINGLE_LINE_COMMENT:
            fprintf(fp, "<span class=\"comment\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_STRING:
            fprintf(fp, "<span class=\"string\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_HEADER_FILE:
            fprintf(fp, "<span class=\"header_file\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_KEYWORD:
            fprintf(fp, "<span class=\"reserved_key1\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_NUMERIC_CONSTANT:
            fprintf(fp, "<span class=\"numeric_constant\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_ASCII_CHAR:
            fprintf(fp, "<span class=\"ascii_char\">");
            print_escaped(fp, event->data);
            fprintf(fp, "</span>");
            break;


        case PEVENT_REGULAR_EXP:
            print_escaped(fp, event->data);
            break;


        case PEVENT_NONE:
        default:
            break;
    }
}