/*

    Name : Anjana V
    Description: Convert a source code into corresponding HTML format
                 keyword -> blue
                 string -> red
                 single line comments in green

*/

#include "main.h"

int main(int argc, char *argv[])
{
    // validate command-line arguments
    if(validate_args(argc, argv) == 0)
    {
        return 1;
    }
    // open source file in read mode
    FILE *src = fopen(argv[1], "r");
    if(src == NULL)
    {
        printf("ERROR: Unable to open %s\n", argv[1]);
        return 1;
    }
    // open HTML file in write mode
    FILE *html = fopen(argv[2], "w");
    if(html == NULL)
    {
        printf("ERROR: Unable to create %s\n", argv[2]);
        fclose(src);
        return 1;
    }
    // write HTML header
    fprintf(html,
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "<meta charset=\"UTF-8\">\n"
        "<title>Source Code</title>\n"
        "<link rel=\"stylesheet\" href=\"style.css\">\n"
        "</head>\n"
        "<body>\n"
        "<pre>\n");

    int ch;
    char word[100];
    int i;
    /* read source file character by character */
    while((ch = fgetc(src)) != EOF)
    {
        /* String Literal */
        if(ch == '"')
        {
            fprintf(html,
                    "<span class=\"string\">\"");

            int closed = 0;

            while((ch = fgetc(src)) != EOF)
            {
                print_html_char(html, ch);

                if(ch == '"')
                {
                    closed = 1;
                    break;
                }
            }

            fprintf(html, "</span>");
            // missing closing quote
            if(!closed)
            {
                fprintf(html,
                "<span class=\"error\">"
                " Unterminated String "
                "</span>");
            }
        }

        /* Identifier / Keyword */
        else if(isalpha(ch) || ch == '_')
        {
            i = 0;
            word[i++] = ch;

            while((ch = fgetc(src)) != EOF &&
                  (isalnum(ch) || ch == '_'))
            {
                word[i++] = ch;
            }

            word[i] = '\0';

            if(is_keyword(word))
            {
                fprintf(html,
                        "<span class=\"keyword\">%s</span>",
                        word);
            }
            else
            {
                fprintf(html, "%s", word);
            }

            if(ch != EOF)
            {
                print_html_char(html, ch);
            }
        }
        /* Numbers */
        else if(isdigit(ch))
        {
            fprintf(html,
                    "<span class=\"number\">");

            do
            {
                print_html_char(html, ch);
                ch = fgetc(src);

            } while(ch != EOF &&
                    isdigit(ch));

            fprintf(html,
                    "</span>");

            if(ch != EOF)
            {
                ungetc(ch, src);
            }
        }
        /* ---------- COMMENTS ---------- */
        else if(ch == '/')
        {
            int next = fgetc(src);
            // sinle-line comment
            if(next == '/')
            {
                fprintf(html,
                "<span class=\"comment\">//");

                while((ch = fgetc(src)) != EOF &&
                    ch != '\n')
                {
                    print_html_char(html, ch);
                }

                fprintf(html, "</span>\n");
            }
            // multi line comment
            else if(next == '*')
            {
                fprintf(html,
                        "<span style=\"color:green;\">/*");

                int prev = 0;
                int closed = 0;
                while((ch = fgetc(src)) != EOF)
                {
                    print_html_char(html, ch);

                    if(prev == '*' &&
                       ch == '/')
                    {
                        closed = 1;
                        break;
                    }

                    prev = ch;
                }

                fprintf(html,
                        "</span>");
                // comment not terminated
                if(!closed)
                {
                    fprintf(html,
                    "<span class=\"error\">"
                    " Unterminated Comment "
                    "</span>");
                }
            }
            else
            {
                fputc('/', html);
                ungetc(next, src);
            }
        }
        /* Normal characters */
        else
        {
            print_html_char(html, ch);
        }
    }
    // file close section
    fprintf(html,
            "</pre>\n"
            "</body>\n"
            "</html>\n");
    // close file
    fclose(src);
    fclose(html);

    printf("HTML file generated: %s\n", argv[2]);

    return 0;
}