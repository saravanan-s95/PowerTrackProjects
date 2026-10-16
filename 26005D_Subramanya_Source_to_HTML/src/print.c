#include <unistd.h>
#include "lexer.h"

#define RESET "\033[0m"

static int line_no = 1;
static int line_start = 1;

const char *type_name(token_type_e t)
{
    static const char *names[] = {
        "Type Keyword (Reserved Keywords)", "Control Keyword (Non-Reserved Keywords)", "Directive", "Identifier",
        "Array", "Int Constant", "Float Constant", "Char Literal",
        "String Literal", "Operator", "Symbol", "Invalid", "Comment", "Header", "Whitespace", "EOF"
    };
    return names[t];
}


/* ---------- HTML output ---------- */

/* Finds the CSS class for the token type (passed as index), same order as the enum token_type_e. "" = plain text */
static const char *html_class[] = {
    "key1",      /* Type Keyword    */
    "key2",      /* Control Keyword */
    "directive", /* Directive       */
    "",          /* Identifier      */
    "",          /* Array           */
    "num",       /* Int Constant    */
    "num",       /* Float Constant  */
    "chr",       /* Char Literal    */
    "str",       /* String Literal  */
    "",          /* Operator        */
    "",          /* Symbol          */
    "",          /* Invalid         */
    "comment",   /* Comment         */
    "header",    /* Header          */
    "",          /* Whitespace      */
    ""           /* EOF             */
};


/* Creates the HTMl page and CSS class styling/coloring */
void print_html_begin(FILE *out)
{
    fprintf(out, "<!DOCTYPE html>\n<html lang=\"en-US\">\n<head>\n");
    fprintf(out, "<meta charset=\"UTF-8\">\n<title>source2html</title>\n");
    fprintf(out, "<style>\n");
    fprintf(out, "body { background-color: lightgrey; }\n");
    fprintf(out, ".key1 { color: green; }\n");
    fprintf(out, ".key2 { color: goldenrod; }\n");
    fprintf(out, ".directive { color: purple; }\n");
    fprintf(out, ".header { color: red; }\n");
    fprintf(out, ".str { color: magenta; }\n");
    fprintf(out, ".num { color: brown; }\n");
    fprintf(out, ".chr { color: firebrick; }\n");
    fprintf(out, ".comment { color: blue; }\n");
    fprintf(out, "</style>\n</head>\n<body>\n<pre>\n");
}

/* Closes the HTML page */
void print_html_end(FILE *out)
{
    fprintf(out, "\n</pre>\n</body>\n</html>\n");
}


/* Prints each token with the appropriate color using html_class array to the .html file */
void print_html_token(FILE *out, const token_t *tok)
{
    const char *cls = html_class[tok->type];
    
    if(cls[0] != '\0')
         fprintf(out, "<span class=\"%s\">", cls);
    
    int i = 0;
    while(tok->lexeme[i] != '\0')
    {
        if(line_start && show_lines)
        {
            if(cls[0] != '\0')
                fprintf(out, "</span>");    /* Remove colring */

            fprintf(out, "%4d. ", line_no);
            line_start = 0;

            if(cls[0] != 0)
                fprintf(out, "<span class=\"%s\">", cls); /* Revert back the same CSS class for token */
        }
        print_html_char(out, tok->lexeme[i]);
        i++;
    }
    
    if(cls[0] != '\0')
        fprintf(out, "</span>");
        
}

void print_html_char(FILE *out, const char ch)
{
    switch(ch)
    {
        case '<':
            fputs("&lt;", out);
            break;

        case '>':
            fputs("&gt;", out);
            break;

        case '&':
            fputs("&amp;", out);
            break;
        
        default:
            fputc(ch, out);
            break;
    }

    if(ch == '\n')
    {
        line_no++;
        line_start = 1;
    }
}