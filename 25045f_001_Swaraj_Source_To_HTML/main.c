#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "html.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    FILE *fp_html;

    Event e;
    char output_file[100];

    // Check command line argument
    if (argc != 2)
    {
        printf("Usage : ./a.out input.c\n");
        return 1;
    }

    // Open source file
    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Error : Unable to open file\n");
        return 1;
    }

    // Create output html file name
    sprintf(output_file, "%s.html", argv[1]);

    // Open html file
    fp_html = fopen(output_file, "w");

    if (fp_html == NULL)
    {
        printf("Unable to create HTML file\n");
        fclose(fp);
        return 1;
    }

    // Write starting html tags
    html_begin(fp_html);

    // Read file till end
    while (1)
    {
        e = get_event(fp);

        // Stop at end of file
        if (e.type == END_FILE)
        {
            break;
        }

        write_html(fp_html, e);
        
    }

    // Write ending html tags
    html_end(fp_html);

    // Close files
    fclose(fp);
    fclose(fp_html);

    return 0;
}