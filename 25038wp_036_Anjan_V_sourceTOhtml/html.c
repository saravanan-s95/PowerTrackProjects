#include "main.h"


// array of keywords 
const char *keywords[] =
{
    "auto","break","case","char","const","continue",
    "default","do","double","else","enum","extern",
    "float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct",
    "switch","typedef","union","unsigned","void","volatile",
    "while"
};

// return 1 -> keyword found, 0 -> not a keyword
int is_keyword(const char *word)
{
    // calculate total number of keywords present in the keywords array
    int n = sizeof(keywords) / sizeof(keywords[0]);

    for(int i = 0; i < n; i++)
    {
        if(strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// converts special HTML characters into entities.
void print_html_char(FILE *html, char ch)
{
    switch(ch)
    {
        case '<':
            fprintf(html, "&lt;");
            break;

        case '>':
            fprintf(html, "&gt;");
            break;

        case '&':
            fprintf(html, "&amp;");
            break;

        default:
            fputc(ch, html);
    }
}