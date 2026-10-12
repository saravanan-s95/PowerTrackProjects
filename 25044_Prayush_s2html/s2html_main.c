
#include "s2html_conv.h"
#include "s2html_event.h"
#include <stdio.h>
#include <string.h>
// #include "s2html_conv.c"
// #include "s2html_event.c"

/********** main **********/

int main(int argc, char *argv[]) {
    FILE *sfp, *dfp; // source and destination file descriptors
    pevent_t *event;
    char dest_file[100];
    char *src_file = NULL;
    char *out_file = NULL;
    int line_num_enabled = 0;

    if (argc < 2) {
        printf("\nError ! please enter file name and mode\n");
        printf("Usage: %s <file name> [output file] [-n]\n", argv[0]);
        printf("Example : ./s2html abc.c abc.html -n\n\n");
        return 1;
    }

    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            line_num_enabled = 1;
        } else if (src_file == NULL) {
            src_file = argv[i];
        } else if (out_file == NULL) {
            out_file = argv[i];
        }
    }

    if (src_file == NULL) {
        printf("Error! No input file specified\n");
        return 1;
    }

#ifdef DEBUG
    printf("File to be opened : %s\n", src_file);
#endif

    /* open the file */
    if (NULL == (sfp = fopen(src_file, "r"))) {
        printf("Error! File %s could not be opened\n", src_file);
        return 2;
    }

    /* Check for output file */
    if (out_file != NULL) {
        if (strstr(out_file, ".html") || strstr(out_file, ".htm")) {
            snprintf(dest_file, sizeof(dest_file), "%s", out_file);
        } else {
            snprintf(dest_file, sizeof(dest_file), "%s.html", out_file);
        }
    } else {
        snprintf(dest_file, sizeof(dest_file), "%s.html", src_file);
    }

    /* open dest file */
    if (NULL == (dfp = fopen(dest_file, "w"))) {
        printf("Error! could not create %s output file\n", dest_file);
        fclose(sfp);
        return 3;
    }

    /* configure line numbers */
    set_line_number_flag(line_num_enabled);

    /* write HTML starting Tags */
    html_begin(dfp, HTML_OPEN);

    /* Read from src file convert into html and write to dest file */
    do {
        event = get_parser_event(sfp);
        /* call source_to_html */
        source_to_html(dfp, event);
    } while (event->type != PEVENT_EOF);

    /* Call start_or_end_conv function for ending the conversion */
    html_end(dfp, HTML_CLOSE);

    printf("\nOutput file %s generated\n", dest_file);
    /* close file */
    fclose(sfp);
    fclose(dfp);

    return 0;
}
