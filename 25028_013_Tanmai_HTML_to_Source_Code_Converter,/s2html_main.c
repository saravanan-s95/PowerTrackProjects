#include <stdio.h>
#include <stdlib.h>

#include "s2html_event.h"
#include "s2html_conv.h"

int main(int argc, char *argv[])
{
    FILE *sfp;
    FILE *dfp;

    char dest_file[100];

    if(argc < 2)
    {
        printf("Usage : %s <source_file> [output_file]\n", argv[0]);
        return 1;
    }

    sfp = fopen(argv[1], "r");

    if(sfp == NULL)
    {
        printf("Unable to open %s\n", argv[1]);
        return 2;
    }

    if(argc > 2)
        sprintf(dest_file, "%s.html", argv[2]);
    else
        sprintf(dest_file, "%s.html", argv[1]);

    dfp = fopen(dest_file, "w");

    if(dfp == NULL)
    {
        printf("Unable to create output file\n");
        fclose(sfp);
        return 3;
    }

    printf("Source File : %s\n", argv[1]);
    printf("Output File : %s\n", dest_file);


    pevent_t *event;

while (1)
{
    event = get_parser_event(sfp);

    if (event->type == PEVENT_EOF)
        break;

    source_to_html(dfp, event);
}

html_end(dfp, HTML_CLOSE);


    fclose(sfp);
    fclose(dfp);

    return 0;
}