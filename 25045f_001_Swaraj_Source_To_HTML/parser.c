#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

// Read next token
Event get_event(FILE *fp)
{
    Event e;

    int ch;
    int i = 0;

    // Read one character
    ch = fgetc(fp);

    // End of file
    if (ch == EOF)
    {
        e.type = END_FILE;
        strcpy(e.word, "");
        return e;
    }

    // Check preprocessor
    if (ch == '#')
    {
        e.word[i++] = ch;

        // Read till end of line
        while ((ch = fgetc(fp)) != EOF && ch != '\n')
        {
            e.word[i++] = ch;
        }

        e.word[i] = '\0';
        e.type = PREPROCESSOR;

        return e;
    }

    // Check keyword / identifier
    if (isalpha(ch) || ch == '_')
    {
        // Store first character
        e.word[i++] = ch;

        // Read complete word
        while ((ch = fgetc(fp)) != EOF)
        {
            if (isalnum(ch) || ch == '_')
            {
                e.word[i++] = ch;
            }
            else
            {
                ungetc(ch, fp);
                break;
            }
        }

        e.word[i] = '\0';

        // Check keyword
        if (is_keyword(e.word))
        {
            e.type = KEYWORD;
        }
        else
        {
            e.type = NORMAL;
        }

        return e;
    }

    // Check number
    if (isdigit(ch) || ch == '.')
    {
        // Store first digit
        e.word[i++] = ch;

        // Read complete number
        while ((ch = fgetc(fp)) != EOF)
        {
            if (isdigit(ch))
            {
                e.word[i++] = ch;
            }
            else
            {
                ungetc(ch, fp);
                break;
            }
        }

        e.word[i] = '\0';
        e.type = NUMBER;

        return e;
    }

    // Check string
    if (ch == '"')
    {
        // Store first quote
        e.word[i++] = ch;

        // Read till closing quote
        while ((ch = fgetc(fp)) != EOF)
        {
            e.word[i++] = ch;

            if (ch == '"')
            {
                break;
            }
        }

        // End string
        e.word[i] = '\0';

        e.type = STRING;

        return e;
    }

    // Check character constant
    if (ch == '\'')
    {
        // Store first quote
        e.word[i++] = ch;

        // Read till closing quote
        while ((ch = fgetc(fp)) != EOF)
        {
            e.word[i++] = ch;

            if (ch == '\'')
            {
                break;
            }
        }

        // End character
        e.word[i] = '\0';

        e.type = CHARACTER;

        return e;
    }

    // Check comment
    if (ch == '/')
    {
        int next = fgetc(fp);

        // Single line comment
        if (next == '/')
        {
            e.word[i++] = '/';
            e.word[i++] = '/';

            while ((ch = fgetc(fp)) != EOF && ch != '\n')
            {
                e.word[i++] = ch;
            }

            e.word[i] = '\0';
            e.type = COMMENT;
            return e;
        }

        // Multi line comment
        else if (next == '*')
        {
            e.word[i++] = '/';
            e.word[i++] = '*';

            while ((ch = fgetc(fp)) != EOF)
            {
                e.word[i++] = ch;

                if (ch == '*')
                {
                    int temp = fgetc(fp);

                    if (temp == '/')
                    {
                        e.word[i++] = '/';
                        break;
                    }
                    else
                    {
                        ungetc(temp, fp);
                    }
                }
            }

            e.word[i] = '\0';
            e.type = MULTI_COMMENT;
            return e;
        }

        // Normal '/'
        else
        {
            ungetc(next, fp);
        }
    }
    
    // Store single character
    e.word[0] = ch;
    e.word[1] = '\0';

    e.type = NORMAL;

    return e;
}