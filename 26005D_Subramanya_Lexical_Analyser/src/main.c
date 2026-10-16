#include "lexer.h"

int main(int argc, char *argv[])
{
    /* Validate CLA: input file, optional output file */
    if(argc != 2 && argc != 3)
    {
        fprintf(stderr, "%s", VALIDATION_ERROR);
        return 1;
    }

    char *ext = strrchr(argv[1], '.');
    if(ext == NULL || strcmp(ext, ".c") != 0)
    {
        fprintf(stderr, "Error: input must be a .c file\n");
        return 1;
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

    /* Output goes to the screen, or to a file if one is given */
    FILE *out = stdout;
    if(argc == 3)
    {
        char *oext = strrchr(argv[2], '.');
        if(oext != NULL && strcmp(oext, ".c") == 0)
        {
            fprintf(stderr, "Error: output file must not be a .c file\n");
            fclose(fp);
            return 1;
        }

        out = fopen(argv[2], "w");
        if(out == NULL)
        {
            perror("fopen");
            fclose(fp);
            return 1;
        }
    }

    int count[TOK_EOF + 1] = {0};
    int total = 0;

    print_banner(out, argv[1]);
    print_header(out);
    while(get_token(&tok) == SUCCESS)
    {
        print_token(out, &tok);
        count[tok.type]++;
        total++;
    }
    print_summary(out, count, total);

    fclose(fp);
    if(out != stdout)
    {
        fclose(out);
        fprintf(stderr, "Output saved to %s\n", argv[2]);
    }
    return 0;
}
