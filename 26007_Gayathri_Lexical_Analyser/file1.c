#include<stdio.h>
#include<string.h>
#include<ctype.h>

int isKeyword(char str[])
{
    //define keywords
    char keywords[10][10] = {"int", "char", "if", "else", "while", "return", "main", "printf", "void", "for"};
    for(int i = 0; i < 10; i++)
    {
        // compare with str and keyword
        if(strcmp(keywords[i], str) == 0)
        return 1;                            //keyword 
    }

    return 0;                                // not a keyword
}

int main()
{
    char ch, buffer[50], op;
    FILE *fptr;
    int i = 0;

    //open file
    fptr = fopen("input.txt", "r");
    if(fptr == NULL)
    {
        printf("file not found!");
        return 0;
    }

    
    while((ch = fgetc(fptr)) != EOF)
    {

        //check space
        if(isspace(ch))
            continue;

        //preprocessor
        if(ch == '#')
        {
            i = 0;
            buffer[i++] = ch;
            while((ch = fgetc(fptr)) != '\n' && ch != EOF)
                buffer[i++] = ch;
            buffer[i] = '\0';
            printf("%s: Preprocessor\n", buffer);    
        }

       
        //keyword / identifier
        else if(isalpha(ch))
        {
            i = 0;
            buffer[i++] = ch;
            while(1)
            {
                ch = fgetc(fptr); 
                if(ch == EOF)
                    break;
                if(isalnum(ch) || ch == '_')
                    buffer[i++] = ch;
                else
                {
                    ungetc(ch, fptr);       //put char back to input
                    break; 
                }   

            }

            buffer[i] = '\0';
            

            if(isKeyword(buffer))
                printf("%s : Keyword\n", buffer);
            else
                printf("%s : Identifier\n", buffer);    
        }

        //constant

        else if(isdigit(ch))
        {
            i = 0;
            buffer[i++] = ch;
            while(1)
            {
                ch = fgetc(fptr);
                if(ch == EOF)
                    break;
                if(isdigit(ch) || ch == '.')
                    buffer[i++] = ch;
                else
                {
                    ungetc(ch, fptr);         //put char back to input
                    break;    
                }
            }
            buffer[i] = '\0';
            printf("%s : Constant\n", buffer);
        }

        //operator
        else if(ch == '+' || ch == '*' || ch == '/' || ch == '-' || ch == '<' || ch == '>' || ch == '_' || ch == '=' || ch == '%')
        {
            op = ch;
            ch = fgetc(fptr);
            if(ch == '=')
                printf("%c%c : Operator\n", op, ch);
            else
            {
                ungetc(ch, fptr);                  //put char back to input
                printf("%c :Operator\n", op);
            }
        }

        //delimiter
        else if(ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' || ch == '}')
        {
            printf("%c : Delimiter\n", ch);
        }

        
    }

    fclose(fptr);
    return 0;
}


