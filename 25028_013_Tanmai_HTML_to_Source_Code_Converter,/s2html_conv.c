#include <stdio.h>
#include "s2html_conv.h"

/*----------------------------------------------------------
 * Print HTML-safe text
 *---------------------------------------------------------*/
static void print_html(FILE *fp, const char *str)
{
    while (*str)
    {
        switch (*str)
        {
            case '<':
                fprintf(fp, "&lt;");
                break;

            case '>':
                fprintf(fp, "&gt;");
                break;

            case '&':
                fprintf(fp, "&amp;");
                break;

            default:
                fputc(*str, fp);
                break;
        }
        str++;
    }
}

/*----------------------------------------------------------
 * HTML Begin
 *---------------------------------------------------------*/
void html_begin(FILE *fp, int type)
{
    if (type == HTML_OPEN)
    {
        fprintf(fp, "<!DOCTYPE html>\n");
        fprintf(fp, "<html lang=\"en-US\">\n");
        fprintf(fp, "<head>\n");
        fprintf(fp, "\t<meta charset=\"UTF-8\">\n");
        fprintf(fp, "\t<title>Source2HTML</title>\n");
        fprintf(fp, "\t<link rel=\"stylesheet\" href=\"styles.css\">\n");
        fprintf(fp, "</head>\n");
        fprintf(fp, "<body style=\"background-color:lightgrey;\">\n");
        fprintf(fp, "<pre>\n");
    }
}

/*----------------------------------------------------------
 * HTML End
 *---------------------------------------------------------*/
void html_end(FILE *fp, int type)
{
    if (type == HTML_CLOSE)
    {
        fprintf(fp, "</pre>\n");
        fprintf(fp, "</body>\n");
        fprintf(fp, "</html>\n");
    }
}

/*----------------------------------------------------------
 * Convert Parser Event to HTML
 *---------------------------------------------------------*/
void source_to_html(FILE *fp, pevent_t *event)
{
    switch (event->type)
    {
        case PEVENT_REGULAR_EXP:
            print_html(fp, event->data);
            break;

        case PEVENT_RESERVED_KEY1:
            fprintf(fp, "<span class=\"reserved_key1\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        case PEVENT_RESERVED_KEY2:
            fprintf(fp, "<span class=\"reserved_key2\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        case PEVENT_SINGLE_LINE_COMMENT:
        case PEVENT_MULTI_LINE_COMMENT:
            fprintf(fp, "<span class=\"comment\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        case PEVENT_PREPROCESSOR:
            fprintf(fp, "<span class=\"preprocess_dir\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        case PEVENT_HEADER_FILE:
            fprintf(fp, "<span class=\"header_file\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        case PEVENT_STRING:
            fprintf(fp, "<span class=\"string\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        case PEVENT_ASCII_CHAR:
            fprintf(fp, "<span class=\"ascii_char\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        case PEVENT_NUMERIC_CONSTANT:
            fprintf(fp, "<span class=\"numeric_constant\">");
            print_html(fp, event->data);
            fprintf(fp, "</span>");
            break;

        default:
            print_html(fp, event->data);
            break;
    }
}