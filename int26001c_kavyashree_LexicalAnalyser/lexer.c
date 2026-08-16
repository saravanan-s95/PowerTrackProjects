#include "lexer.h"

/*
 * Function : iskeyword
 * Checks whether the word is a data type keyword
 */

int iskeyword(const char *word)
{
    int size = sizeof(keywords_data) / sizeof(keywords_data[0]);

    for(int i = 0; i < size; i++)
    {
        if(strcmp(word, keywords_data[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}


/*
 * Function : is_nondatakeyword
 * Checks whether the word is a non-data keyword
 */

int is_nondatakeyword(const char *word)
{
    int size = sizeof(keywords_non_data) / sizeof(keywords_non_data[0]);

    for(int i = 0; i < size; i++)
    {
        if(strcmp(word, keywords_non_data[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}


/*
 * Function : isoperator
 * Checks whether a character is an operator
 */

int isoperator(char ch)
{
    int size = sizeof(operators) / sizeof(operators[0]);

    for(int i = 0; i < size; i++)
    {
        if(ch == operators[i])
        {
            return 1;
        }
    }

    return 0;
}


/*
 * Function : isdelimiter
 * Checks whether a character is a delimiter
 */

int isdelimiter(char ch)
{
    int size = sizeof(delimiters) / sizeof(delimiters[0]);

    for(int i = 0; i < size; i++)
    {
        if(ch == delimiters[i])
        {
            return 1;
        }
    }

    return 0;
}

void print_token(const char *type, const char *value)
{
    printf("%-15s : %s\n", type, value);
}

void lexical_analysis(FILE *fin)
{
    int ch;     //stores the current character read from the file
    int next;          //stores the look ahead character for decisions on token
    int i;        //variable to store token 

    char word[MAX_SIZE];      //temporary buffer to store token
    int line = 1;          //to have the line count for error messages

    int paren = 0;      //()
    int brace = 0;      //{}
    int bracket = 0;    //[]

    while((ch = fgetc(fin)) != EOF)
    {
        //line count  after reading \n
        if(ch == '\n')
        {
            line++;      
            continue;   //skip the token checks
        }

        //ignore space as it is not a seperate token
        if(isspace(ch))
        {
            continue;
        }

        //preproceesor directive

        if(ch == '#')   //starts from #
        {
            i = 0;
            word[i++] = ch;   //store #

            while((ch = fgetc(fin)) != EOF && ch != '\n')
            {
                word[i++] = ch;   //then remaining characters
            } 

            word[i] = '\0';    //valid string 

            print_token("Preprocessor", word);   //print 

            if(ch == '\n')      //preprocessor ends in one line 
            {
                line++;    //increment line number 
            }

            continue;         //skip further checks
        }

        //comments

        else if(ch == '/')        // '/' means divsion,single,multiline comment may start
        {
            next = fgetc(fin);         //lookahead another character to decide

            //Single line comment 
            if(next == '/')          //if next is / then // gives single line comment
            {
                while((ch = fgetc(fin)) != EOF && ch != '\n');   //no need to process the comment words so loop does'nt run

                if(ch == '\n')       //comment does not effects the program execution 
                {
                    line++;            //skip to next line after consuming the characters
                }

                continue;
            }

            //Multi line comment 
            else if(next == '*')      //if next is * character
            {
                int found = 0;         //to find whether */ closing of comment is done 

                while((ch = fgetc(fin)) != EOF)
                {
                    if(ch == '\n')              //line increment as multi line comment runs multiple lines
                    {
                        line++;
                    }

                    if(ch == '*')              //if * found
                    {
                        next = fgetc(fin);      //next is / then */ ends multi line 

                        if(next == '/')      
                        {
                            found = 1;
                            break;
                        }

                        ungetc(next, fin);      //to avoid skipping the lookahead character from reading eg. a / b; to put back into file
                    }
                }

                if(!found)             //error
                {
                    printf("\nError (Line %d): Multi-line comment not closed.\n", line);
                    return;
                }

                continue;
            }

            //Division Operator  
            else
            {
                ungetc(next, fin);    //put back character read during decision to file

                word[0] = '/';
                word[1] = '\0';

                print_token("Operator", word); //compare to the operators

                continue;
            }
        }

        //character literal
        else if(ch == '\'')     //starts the character with single quote
        {
            i = 0;
            word[i++] = ch;        //initialize the character '

            while((ch = fgetc(fin)) != EOF)     
            {
                word[i++] = ch;                                                              

                if(ch == '\'')      //ends with ' so character
                {
                    break;
                }

                if(ch == '\n')        //if new line found without single quote ' error
                {
                    printf("\nError (Line %d): Character literal not closed.\n", line);
                    return;
                }
            }

            if(ch == EOF)           
            {
                printf("\nError (Line %d): Character literal not closed.\n", line);
                return;
            }

            word[i] = '\0';     //valid string

            print_token("Character", word);  //compare characters
 
            continue;
        }

        //string literal 
        else if(ch == '"')          //string starts from double quote "
        {
            i = 0;
            word[i++] = ch; 

            while((ch = fgetc(fin)) != EOF)
            {
                if(ch == '\n')            //new line found without " double quote
                {
                    printf("\nError (Line %d): String literal not closed.\n", line);
                    return;
                }

                word[i++] = ch;          

                if(ch == '"')         //ends with double quote
                {
                    break;
                }
            }

            if(ch == EOF)            
            {
                printf("\nError (Line %d): String literal not closed.\n", line);
                return;
            }

            word[i] = '\0';       //valid string 

            print_token("String", word);

            continue;
        }
        
        //integer and float
        else if(isdigit(ch))   //is character number
        {
            i = 0;
            int dot = 0;     //dot variable to validate float/decimal number with 1 dot 

            word[i++] = ch;

            while((ch = fgetc(fin)) != EOF)
            {
                if(isdigit(ch))                 //store numbers
                {
                    word[i++] = ch;
                }
                else if(ch == '.' && dot == 0)      //if . is found in between and it is only one 
                {
                    dot = 1;
                    word[i++] = ch;     //then valid decimal point  
                }
                else if(isalpha(ch) || ch == '_')
                {
                    word[i++] = ch;

                    while((ch = fgetc(fin)) != EOF &&
                        (isalnum(ch) || ch == '_'))
                    {
                        word[i++] = ch;
                    }

                    word[i] = '\0';

                    printf("Error (Line %d): Invalid identifier '%s'\n",
                            line, word);

                    if(ch != EOF)
                        ungetc(ch, fin);

                    continue;
                }
                else
                {
                    break;      //if more then 1 dot found or no dots are found then break
                }
            }
 
            word[i] = '\0';       //valid string 

            print_token("Decimal", word);       

            if(ch != EOF)           //store the extra character we read back to file
            {
                ungetc(ch, fin);      //extra character used to validate the digits eg.1234;  ;->extra character
            }

            continue;      //skip further checks
        }

        //Keyword and Identifier 
        else if(isalpha(ch) || ch == '_')           //if character is alphabet validate for keywords or identifier
        {
            i = 0;
            word[i++] = ch;                       //identifier starts with _ underscore or alphabet not with digit

            while((ch = fgetc(fin)) != EOF && (isalnum(ch) || ch == '_'))            //in between numbers can be present in identifier
            {
                word[i++] = ch;                      //store
            } 
 
            word[i] = '\0';                          //valid string 

            if(iskeyword(word) || is_nondatakeyword(word))         //check whether word is keyword (datatype or nondatatype)
            {
                print_token("Keyword", word);      //if true prints the keyword 
            }
            else
            {
                print_token("Identifier", word);     
            }

            if(ch != EOF)
            {
                ungetc(ch, fin);          //put back the extra character to file eg: student;  ;->extra character
            }

            continue;
        }

        //operators
        else if(isoperator(ch))      
        {
            word[0] = ch;        //character is operator or not 
            word[1] = '\0';

            print_token("Operator", word);

            continue;
        }

        //delimiters
        else if(isdelimiter(ch))
        { 
            if(ch == '(')           //parenthesis ()
            {
                paren++;    //open paren (
            }
            else if(ch == ')')
            {
                paren--;           //if closed )

                if(paren < 0)
                {
                    printf("Error (Line %d): ')' without matching '('.\n", line);
                    return;
                }
            }
            else if(ch == '{')    //braces {}
            {
                brace++;
            }
            else if(ch == '}')
            {
                brace--;

                if(brace < 0)     //if not closed
                {
                    printf("Error (Line %d): '}' without matching '{'.\n", line);
                    return;
                }
            }
            else if(ch == '[')     //bracket []
            {
                bracket++;
            }
            else if(ch == ']')
            {
                bracket--;

                if(bracket < 0)
                {
                    printf("Error (Line %d): ']' without matching '['.\n", line);
                    return;
                }
            }

            word[0] = ch;
            word[1] = '\0';

            print_token("Delimiter", word);

            continue;
        }

    }
    int error = 0;

    if(paren != 0)
    {
        printf("Error: Missing ')'\n");
        error = 1;
    }

    if(brace != 0)
    {
        printf("Error: Missing '}'\n");
        error = 1;
    }

    if(bracket != 0)
    {
        printf("Error: Missing ']'\n");
        error = 1;
    }

    if(!error)
    {
        printf("\nLexical Analysis Completed Successfully.\n");
    } 
}