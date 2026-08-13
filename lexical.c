#include "main.h"

char errors[100][100];
int error_line[100];
int error_count = 0;

char *keyword[] = {"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if",
    "int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile"
    ,"while",NULL};

void lexical_analyzer(FILE *fptr)
{
    char ch;    // Variable to store each character read from file
    int hex_flag = 0;   //flag for hexadecimal value.
    char word[100];     //temporary buffer array for storing tokens.
    int i = 0;  //index for buffer array
    int line_number = 1;    //to print line numbers
    while((ch = fgetc(fptr)) != -1)     //loop upto End Of File.
    {
        if(isalnum(ch) || ch == '_')    // Build keywords or identifiers.
        {
            word[i++] = ch;     //store character into buffer
        }   
        else if(ch == '.')  //check for floating numbers
        {
            word[i] = '\0';     //add NULL and terminate the current token.
            if(is_number(word) || is_float(word))   //add '.' only if the current token is valid number or a float.
            {
                word[i++] = '.';
            }
        }
        else if(ch == '\'')     //check for character literals like 'A' or '\n'
        {
            char char_literal[10];  //buffer for charcter literal
            int j=0;    //index for buffer
            char_literal[j++] = ch;  //store the first ' in a buffer.
            char_literal[j++] = fgetc(fptr);    //read the next character also.
            if(char_literal[j-1] == '\\')   //if that character was \ then we either have '\n' or '\t'
            {
                char_literal[j++] = fgetc(fptr);    //so read the next 2 characters if condition is true. so now we have '\n' in buffer.
                char_literal[j++] = fgetc(fptr);    //we read n and ' and store in buffer.
            }
            else
            {
                char_literal[j++] = fgetc(fptr);    //if condition was false, just read 1 character because its not new line or tab space.
                                                    //now we have 'A' in buffer.
            }
            if(char_literal[j-1] == '\'')   //next check if closing ' is present.
            {
                char_literal[j] = '\0'; //if true, add NULL character and print it.
                printf("%d\t\t%s\t\tCharacter Literal\n", line_number, char_literal);
            }
            else
            {
                strcpy(errors[error_count], "Error : Invalid Character Literal.\n");   //else its an error, store in array.
                    error_line[error_count] = line_number;
                    error_count++;
            }
        }
        else if(ch == '"')  //check for string literals
        {
            char string_literal[100];   // Buffer for string literal.
            int j=0;    // Index for string buffer.
            string_literal[j++] = ch;   // Store opening quote '"'.
            int flag = 0;   // Flag to indicate if string is invalid.
            while(1)    //read characters until closing '"' .
            {
                ch = fgetc(fptr);
                if(ch == EOF)   //if EOF is reached before closing '"', then its error.
                {
                    strcpy(errors[error_count], "Error : Invalid String Literal.\n");
                    error_line[error_count] = line_number;
                    error_count++;
                    flag = 1;
                    break;
                }
                if(ch == '\n')  // Newline reached before closing quote then also error.
                {
                    strcpy(errors[error_count], "Error : Invalid String Literal,\n");
                    error_line[error_count] = line_number;
                    error_count++;
                    flag = 1;
                    break;
                }
                string_literal[j++] = ch;   // Store character.
                if(ch == '"')   // Stop when closing quote is found.
                    break;
            }
            if(!flag)   // Print string if it is valid based on the flag.
            {
                string_literal[j] = '\0';
                printf("%d\t\t%s\t\tString Literal\n", line_number, string_literal);
            }
        }
        else if(ch == '#')  // for handling preprocessor directives.
        {
            is_preprocessor(ch,fptr,line_number);   //call the function.
        }
        else
        {
            if(i != 0)  // Process the token if buffer is not empty.
            {
                word[i] = '\0'; //add NULL to terminate token.
                if(is_keyword(word))    //check if the token is a keyword.
                {
                    printf(GREEN "%d\t\t%s\t\tKeyword\n" RESET,line_number,word);
                }
                else if(is_octal(word,line_number)) // Check if token is an octal number.
                {
                    printf("%d\t\t%s\t\tOctal\n", line_number, word);
                }
                else if(is_number(word))    //check if the token is a number.
                {
                    printf(BCYAN "%d\t\t%s\t\tInteger\n" RESET,line_number, word);
                }
                else if(is_float(word)) // Check if token is a floating number.
                {
                    // printf("checking float : %s\n",word);
                    printf(BCYAN "%d\t\t%s\t\tFloat\n" RESET,line_number, word);
                }
                else if(is_hexadecimal(word,&hex_flag,line_number)) // Check if token is hexadecimal.
                {
                    printf("%d\t\t%s\t\tHexadecimal\n", line_number, word);
                }
                else if(is_identifier(word))    //check if the token is a identifier.
                {
                    printf(BMAGENTA "%d\t\t%s\t\tIdentifier\n" RESET,line_number, word);
                }
                else
                {
                    if(hex_flag == 0)   // Avoid duplicate hexadecimal errors.
                    {
                        if(strchr(word, '.') != NULL)   // Check for invalid float values
                        {
                            strcpy(errors[error_count], "Error : Invalid Float value.\n");
                            error_line[error_count] = line_number;
                            error_count++;
                        }
                        else if(!(is_identifier(word))) // Check for invalid variables.
                        {
                            strcpy(errors[error_count], "Error : Invalid variable declaration.\n");
                            error_line[error_count] = line_number;
                            error_count++;
                        }
                        else    // Print unknown token.
                        {
                            printf(BBLUE "%d\t\t%s\t\tUnknown\n" RESET, line_number, word);    //print unknown otherwise.
                        }
                    }
                    else
                    {
                        hex_flag = 0;
                    }
                }
                i = 0;  // Reset token buffer index.
            }
            check_brackets(ch, line_number);    // Check for unmatched brackets.
            if(is_operator(ch,fptr,line_number))     //check if the character read is an operator.
            {
                // printf(RED "%d\t\t%c\t\tOperator\n" RESET,line_number, ch);
            }
            else if(is_delimiter(ch))   //check if the character is a delimiter.
            {
                printf("%d\t\t%c\t\tDelimiter\n",line_number, ch);
            }

            if(ch == '\n')  // Update line number whenever newline is encountered.
            {
                line_number++;
                printf("\n");
            }
        }
    }
    check_remaining_brackets(line_number);  // Check for any remaining unmatched opening brackets
    if(error_count > 0)  // Print all collected lexical errors.
    {
        printf("\n\nErrors:\n");
        for(int i = 0; i < error_count; i++)
        {
            printf("Line %d : %s\n", error_line[i], errors[i]);
        }
    }
}

int is_keyword(char *word)  // Function to check whether the given token is a keyword.
{
    for(int i = 0; keyword[i] != NULL ; i++)    //run loop through the array of strings.
    {
        if(strcmp(word, keyword[i]) == 0)   //if keywords match with the word, then return success.
        {
            return SUCCESS;
        }
    }
    return FAILURE;   //return failure otherwise.
}

int is_delimiter(char ch)   // Function to check whether the given character is a delimiter..
{
    if(ch=='(' || ch==')' || ch=='{' || ch=='}' || ch==';' || ch == '[' || ch == ']' || ch == ',')
    {
        return SUCCESS;  // Character is a delimiter.
    }
    return FAILURE;      // Character is not a delimiter.
}

int is_number(char *word)   // Function to check whether the given token is an integer.
{
    if(word[0] == '\0')     // Empty string is not a valid number.
    {
        return FAILURE;
    }
    if(word[0] == '0' && word[1] != '\0')   // Numbers starting with 0 are  octal.
    {
        return FAILURE;
    }
    for(int i = 0; word[i]; i++)    //check every character by travsersing.
    {
        if(!isdigit(word[i]))   //if it is not a digit, then return failure.
        {
            return FAILURE;
        }
    }
    return SUCCESS;   //return success otherwise.
}

int is_invalid_identifier(char *word)   // Function to check whether an identifier is invalid.
{
    if(!(isalpha(word[0]) || word[0] == '_'))   //check if 1st character is an alphabet or underscore(_).
    {
        return SUCCESS;
    }
    for(int i = 1; word[i]; i++)    //loop through the word and check that the word has only alphabets, numbers or underscore(_).
    {
        if(!(isalnum(word[i]) || word[i] == '_'))
        {
            return SUCCESS;     // Invalid identifier.
        }
    }
    return FAILURE;     // Identifier is valid.
}

int is_hexadecimal(char *word,int *hex_flag,int line_number)    // Function to check whether the token is a hexadecimal number.
{
    int i=0,count=0;     // Variables for counting token length
    for(i=0;word[i] != '\0';i++)    //find token length.
    {
        count++;
    }
    i=0;
    if(count>=3)    // Minimum hexadecimal length should be 3 (e.g., 0x1).
    {
        if(word[i] == '0' && (word[i+1] == 'x' || word[i+1] == 'X'))    // Check for 0x or 0X prefix.
        {
            // int flag=0;
            for(int j=2;word[j] != '\0';j++)    // Validate hexadecimal digits.
            {
                /// Allow digits and A-F/a-f.
                if((word[j] >= 48 && word[j] <=57) || (word[j] >=65 && word[j] <= 70) || (word[j] >=97 && word[j] <= 102))
                {
                    continue;    // Valid hexadecimal digit.
                }
                else
                {   // Store hexadecimal error.
                    strcpy(errors[error_count], "Error : Invalid Hexadecimal value.");
                    error_line[error_count] = line_number;
                    error_count++;
                    *hex_flag = 1;  // Set flag to avoid duplicate error.
                    return FAILURE;
                }
            }
        }
        else
        {
            return FAILURE; // Missing hexadecimal prefix.
        }
    }
    else
    {
        return FAILURE; // Token too short to be hexadecimal.
    }
    *hex_flag = 1;  // Mark hexadecimal token as processed.
    return SUCCESS;
}

int is_octal(char *word,int line_number)    // Function to check whether the token is an octal number.
{
    if(word[0] != '0')  // Octal numbers must begin with 0.
        return FAILURE;

    if(word[1] == 'x' || word[1] == 'X')    // Ignore hexadecimal numbers beginning with 0x or 0X.
        return FAILURE;
        
    if(word[1] == '\0') // Single 0 is not treated as an octal here.
        return FAILURE;

    for(int i = 1; word[i] != '\0'; i++)    // Validate remaining digits.
    {
        if(word[i] < '0' || word[i] > '7')  // Octal digits must be between 0 and 7.
        {
            strcpy(errors[error_count], "Error : Invalid Octal value.");     // Store octal error.
            error_line[error_count] = line_number;
            error_count++;
            return FAILURE;
        }
    }

    return SUCCESS; // Valid octal number.
}