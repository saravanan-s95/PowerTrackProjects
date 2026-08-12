#include <stdio.h>
#include "html.h"
#include "parser.h"

// Write HTML start tags
void html_begin(FILE *fp_html)
{
    fprintf(fp_html, "<html>\n");
    fprintf(fp_html, "<head>\n");
    fprintf(fp_html, "<title>Source To HTML</title>\n");
    fprintf(fp_html, "</head>\n");
    fprintf(fp_html, "<body>\n");
    fprintf(fp_html, "<pre>\n");
}

// Write HTML ending tags
void html_end(FILE *fp_html)
{
    fprintf(fp_html, "</pre>\n");
    fprintf(fp_html, "</body>\n");
    fprintf(fp_html, "</html>\n");
}

// Write token into html file
void write_html(FILE *fp_html, Event e)
{
    if (e.type == KEYWORD)
    {
        fprintf(fp_html, "<span style=\"color:blue;\">%s</span>", e.word);
    }
    else if (e.type == NUMBER)
    {
        fprintf(fp_html, "<span style=\"color:purple;\">%s</span>", e.word);
    }
    else if (e.type == STRING)
    {
        fprintf(fp_html, "<span style=\"color:green;\">%s</span>", e.word);
    }
    else if (e.type == CHARACTER)
    {
        fprintf(fp_html, "<span style=\"color:brown;\">%s</span>", e.word);
    }
    else if (e.type == COMMENT)
    {
        fprintf(fp_html, "<span style=\"color:gray;\">%s</span>", e.word);
    }
    else if (e.type == MULTI_COMMENT)
    {
        fprintf(fp_html, "<span style=\"color:gray;\">%s</span>", e.word);
    }
    else if (e.type == PREPROCESSOR)
    {
        fprintf(fp_html, "<span style=\"color:red;\">");
        print_html(fp_html, e.word);         // because preprocessor also consist of <> html tags
        fprintf(fp_html, "</span>");
    }
    else
    {
        print_html(fp_html, e.word);
    }
}

// Print special HTML characters
void print_html(FILE *fp_html, char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '<')
        {
            fprintf(fp_html, "&lt;");
        }
        else if (str[i] == '>')
        {
            fprintf(fp_html, "&gt;");
        }
        else if (str[i] == '&')
        {
            fprintf(fp_html, "&amp;");
        }
        else
        {
            fputc(str[i], fp_html);
        }

        i++;
    }
}