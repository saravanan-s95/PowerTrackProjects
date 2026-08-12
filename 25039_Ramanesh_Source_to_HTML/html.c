#include<stdio.h>
#include"source2html.h"

void html_begin(FILE *fp)
{
    fprintf(fp,
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "<title>Source To HTML</title>\n"
    "<style>\n"
    "body{background:white;font-family:Consolas,monospace;white-space:pre;font-size:15px;}\n"
    ".keyword{color:blue;font-weight:bold;}\n"
    ".identifier{color:black;}\n"
    ".number{color:red;}\n"
    ".string{color:green;}\n"
    ".comment{color:gray;font-style:italic;}\n"
    ".operator{color:purple;}\n"
    ".preprocessor{color:brown;}\n"
    "</style>\n"
    "</head>\n"
    "<body>\n");
}

void html_end(FILE *fp)
{
    fprintf(fp,"\n</body>\n</html>");
}

void html_escape(FILE *fp,char ch)
{
    switch(ch)
    {
        case '<':
            fprintf(fp,"&lt;");
            break;

        case '>':
            fprintf(fp,"&gt;");
            break;

        case '&':
            fprintf(fp,"&amp;");
            break;

        case '"':
            fprintf(fp,"&quot;");
            break;

        default:
            fputc(ch,fp);
    }
}

void print_keyword(FILE *fp,char *str)
{
    fprintf(fp,"<span class=\"keyword\">%s</span>",str);
}

void print_identifier(FILE *fp,char *str)
{
    fprintf(fp,"<span class=\"identifier\">%s</span>",str);
}

void print_number(FILE *fp,char *str)
{
    fprintf(fp,"<span class=\"number\">%s</span>",str);
}

void print_string(FILE *fp,char *str)
{
    fprintf(fp,"<span class=\"string\">%s</span>",str);
}

void print_comment(FILE *fp,char *str)
{
    fprintf(fp,"<span class=\"comment\">%s</span>",str);
}

void print_operator(FILE *fp,char *str)
{
    fprintf(fp,"<span class=\"operator\">%s</span>",str);
}

void print_preprocessor(FILE *fp,char *str)
{
    fprintf(fp,"<span class=\"preprocessor\">%s</span>",str);
}