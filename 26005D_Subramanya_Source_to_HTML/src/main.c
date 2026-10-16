#include "lexer.h"

/* Global variable */
int show_lines = 0;

int main(int argc, char *argv[])
{
    /* Validate CLA */
   
    int have_out = 0;
    char out_name[256];

    /* input file + optional output.html + optional -n */
    if(argc < 2 || argc > 4)
    {
        fprintf(stderr, "Invalid CLA Input\nUSAGE: ./build/lexer input_file.c [output_file.html] [-n]\n");
        return 1;
    }

    char *ext = strrchr(argv[1], '.');
    if(ext == NULL || strcmp(ext, ".c") != 0)
    {
        fprintf(stderr, "Error: input must be a .c file\n");
        return 1;
    }

    /* Default output name: input name + ".html" */
    snprintf(out_name, sizeof(out_name), "%s.html", argv[1]);

    /* Each extra argument is either -n or the output name */
    for(int i = 2; i < argc; i++)
    {
        if(strcmp(argv[i], "-n") == 0)
        {
            show_lines = 1;
            continue;
        }

        char *oext = strrchr(argv[i], '.');
        if(have_out || oext == NULL || strcmp(oext, ".html") != 0)
        {
            fprintf(stderr, "Error: output must be a single .html file\n");
            return 1;
        }

        snprintf(out_name, sizeof(out_name), "%s", argv[i]);
        have_out = 1;
    }    

    /* Open the input file */
    token_t tok;
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        perror("fopen");
        return 1;
    }
    tok.fp = fp;


    FILE *out = fopen(out_name, "w");
    if(out == NULL)
    {
        perror("fopen");
        fclose(fp);
        return 1;
    }

    print_html_begin(out);
    while(get_token(&tok) == SUCCESS)
        print_html_token(out, &tok);
    print_html_end(out);

    fclose(fp);
    fclose(out);
    fprintf(stderr, "Output saved to %s\n", out_name);
    return 0;
}
