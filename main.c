/*
Name        : Sheeba Insily R Robert
Batch Number: 26013
Project     : Source to HTML

Description :
This project converts a C source file into an HTML file.
It identifies and highlights source code elements such as
keywords, comments, strings, and special characters.
The generated HTML file displays the source code in a
readable format using HTML and CSS.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "source_to_html.h"

int main(int argc, char *argv[])
{
    FILE *input_fp;
    FILE *output_fp;

    int ch;
    char output_filename[1024];

    event_t *event;

    /* Check command-line argument */
    if (argc != 2)
    {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }

    /* Open input C source file */
    input_fp = fopen(argv[1], "r");

    if (input_fp == NULL)
    {
        perror("Error opening input file");
        return 1;
    }

    /* Create output filename */
    snprintf(output_filename,
             sizeof(output_filename),
             "%s.html",
             argv[1]);

    /* Open output HTML file */
    output_fp = fopen(output_filename, "w");

    if (output_fp == NULL)
    {
        perror("Error creating output file");
        fclose(input_fp);
        return 1;
    }

    /* Write HTML header */
    write_html_header(output_fp);

    /*
     * Read the input C file character by character
     */
    while ((ch = fgetc(input_fp)) != EOF)
    {
        event = parse_source(input_fp, ch);

        if (event != NULL)
        {
            source_to_html(output_fp, event);
        }
    }

    /* Write HTML footer */
    write_html_footer(output_fp);

    /* Close files */
    fclose(input_fp);
    fclose(output_fp);

    printf("Output file %s generated successfully.\n", output_filename);

    return 0;
}