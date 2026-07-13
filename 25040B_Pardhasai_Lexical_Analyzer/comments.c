#include <stdio.h>
int remove_comments(FILE *fp)
{
    int pres, next;      // Store current and next characters

    FILE *new = fopen("process.c", "w");      // Create processed output file

    if (new == NULL)
    {
        printf("Error occurred while creating process file\n");      // File creation failed
        return 0;
    }

    while ((pres = fgetc(fp)) != EOF)      // Read input file character by character
    {
        if (pres == '/')      // Possible start of a comment
        {
            next = fgetc(fp);      // Read next character

            if (next == '/')      // Single-line comment
            {
                while ((pres = fgetc(fp)) != '\n' && pres != EOF);      // Skip until end of line

                if (pres == '\n')
                    fputc('\n', new);      // Preserve newline
            }
            else if (next == '*')      // Multi-line comment
            {
                while ((pres = fgetc(fp)) != EOF)      // Skip comment contents
                {
                    if (pres == '*')      // Possible end of comment
                    {
                        next = fgetc(fp);

                        if (next == '/')
                            break;      // End of multi-line comment
                        else
                            ungetc(next, fp);      // Not the end, push back character
                    }
                }
            }
            else
            {
                fputc(pres, new);      // Write '/' to output file

                if (next != EOF)
                    fputc(next, new);      // Write next character if it is not part of a comment
            }
        }
        else
        {
            fputc(pres, new);      // Copy normal character to output file
        }
    }

    fclose(new);      // Close processed file

    return 1;      // Comment removal completed successfully
}