#include<stdio.h> 
#include<ctype.h> 
#include<string.h>
#include "lexer.h" 


int line_no=1; // Variable to store current line number
int round_bracket=0; // Variable to check parenthesis
int curly_bracket=0; // Variable to check curly brackets
int square_bracket=0; // Variable to check square brackets
char error[100][100]; // Array to store error messages
int error_line[100]; // Array to store error line numbers
int error_count=0; // Variable to count errors
int statement_flag=0; // Variable to track statements
int previous_line=1; // Store previous line number
int ignore_semicolon_check=0; // Variable to skip non statement lines
char current_line[200]; // Store current line


void lexicalError(int line,char *message) // Function to store lexical errors
{
    error_line[error_count]=line; // Store error line number

    strcpy(error[error_count],message); // Store error message

    error_count++; // Increase error count
}


void displayErrors() // Function to display lexical errors
{
    int i; // Loop variable


    if(error_count==0) // Check whether errors are present
    {
        return; // Return if no errors are found
    }


    printf("\n\n----- Lexical Errors -----\n");


    for(i=0;i<error_count;i++) // Display all errors
    {
        printf("Line %d : %s\n",
        error_line[i],
        error[i]);
    }
}


void tokenize(FILE *fp) // Function to read the source file and identify tokens
{
    char ch; // Variable to store each character
    char temp[2]; // Temporary character array to store current character
    strcpy(current_line,"");

    while((ch=getc(fp))!= EOF) // Read one character until end of file
    {
        temp[0]=ch; // Store current character
        temp[1]='\0'; // Add string termination
        strcat(current_line,temp); // Add character to current line

        if(ch=='\n')
        {

            if(strlen(current_line)>1)
            {
                if(isSpecialLine(current_line))
                {
                    ignore_semicolon_check=1;
                }
            }

            if(statement_flag==1 && ignore_semicolon_check==0)
            {
                lexicalError(line_no,"Missing ';' at end of statement");
            }

            statement_flag=0;

            ignore_semicolon_check=0;

            strcpy(current_line,"");


            line_no++;
        }
        if(isspace(ch)) // Check if the character is a space or new line
        {
            continue; // Skip white spaces
        }
        else if(isalpha(ch) || ch =='_') // Check if it is the start of an identifier
        {
            readIdentifier(fp, ch); // Read the complete identifier
        }
        else if(isdigit(ch)) // Check if it is a number
        {
            readNumber(fp,ch); // Read the complete number
        }
        else
        {
            switch(ch) // Check for special symbols
            {
                case '/': // Handle comments and division operator
                    skipComment(fp); // Check comment
                    break; // Exit the switch

                case '"': // Handle string literals
                    readString(fp); // Read the complete string
                    break; // Exit the switch

                default: // Handle operators and delimiters
                    readOperator(fp,ch); // Process the current symbol

            }
        }
    }

    checkBrackets(); // Check bracket matching after scanning complete file
}
void readIdentifier(FILE *fp, char first) // Function to read identifiers and keywords
{
    char str[100]; // Store the identifier

    statement_flag=1;
    int i=0; // Start index from 0

    str[i++]=first; // Store the first character

    char c; // Variable to read remaining characters

    while((c=getc(fp))!=EOF) // Read characters until end of file
    {
        if(isalnum(c) || c=='_') // Check if it is still part of an identifier
        {
            str[i++] = c; // Store the character
        }
        else
        {
            ungetc(c,fp); // Put back the extra character

            break; // Stop reading the identifier
        }
    }

    str[i]='\0'; // End the string


    if(isdigit(str[0])) // Check invalid identifier
    {
        lexicalError(line_no,"Invalid identifier");
    }


    int p = isKeyword(str); // Check whether it is a keyword

    if(p==1) // If it is a keyword
    {
        printf("%-20s %-40s\n",str,"Keyword"); // Print keyword
    }

    else
    {
        printf("%-20s %-40s\n",str,"Identifier");

        if(str[0]!='#')
        {
            statement_flag=1;
        }
    }
}


void readNumber(FILE *fp, char first) // Function to read integer and floating values
{
    char c; // Variable to read characters

    char num[100]; // Store the number

    int i = 0; // Start index from 0
    statement_flag=1;

    num[i++] = first; // Store the first digit


    while ((c = fgetc(fp)) != EOF) // Read remaining characters
    {
        if (isdigit(c) || c=='.') // Accept digits and decimal point
        {
            num[i++] = c; // Store the character
        }

        else if(isalpha(c)) // Check invalid number
        {
            num[i++] = c;

            while((c=fgetc(fp))!=EOF && isalnum(c))
            {
                num[i++]=c;
            }

            ungetc(c,fp);

            break;
        }

        else
        {
            ungetc(c, fp); // Put back the extra character

            break; // Stop reading the number
        }
    }


    int x=0; // Flag to check whether it is a float

    int dot=0; // Count decimal points

    num[i] = '\0'; // End the string


    for(int j=0;num[j]!=0;j++) // Check every character
    {
        if(num[j]=='.') // Check for decimal point
        {
            x=1; // Mark it as a floating value

            dot++; // Increase decimal count
        }

        if(isalpha(num[j])) // Check invalid number format
        {
            lexicalError(line_no,"Invalid numeric constant");
        }
    }


    if(dot>1) // Check multiple decimal points
    {
        lexicalError(line_no,"Invalid numeric constant");
    }


    if(x==0) // If decimal point is not found
    {
        printf("%-20s %-40s\n", num,"Integer"); // Print integer
    }

    else
    {
        printf("%-20s %-40s\n", num,"Float"); // Print float
    }
}


void readOperator(FILE *fp, char first) // Function to identify operators and delimiters
{
    if(isOperator(first)) // Check whether it is an operator
    {
        printf("%-20c %-40s\n",first,"Operator"); // Print operator
    }


    if(isDelimiter(first)) // Check whether it is a delimiter
    {
        if(first=='(') // Check opening parenthesis
            round_bracket++;

        else if(first==')') // Check closing parenthesis
            round_bracket--;


        else if(first=='{') // Check opening curly bracket
            curly_bracket++;

        else if(first=='}') // Check closing curly bracket
            curly_bracket--;


        else if(first=='[') // Check opening square bracket
            square_bracket++;

        else if(first==']') // Check closing square bracket
            square_bracket--;


        if(round_bracket<0)
        {
            lexicalError(line_no,"Extra ')'");

            round_bracket=0;
        }


        if(curly_bracket<0)
        {
            lexicalError(line_no,"Extra '}'");

            curly_bracket=0;
        }


        if(square_bracket<0)
        {
            lexicalError(line_no,"Extra ']'");
            
            square_bracket=0;
        }

        if(first==';')
        {
            statement_flag=0;
        }

        printf("%-20c %-40s\n",first,"Delimiter");
    }

}
int isOperator(char ch) // Function to check for operators
{
        char op[]="+-,*/%=<>!&|#@^?~`"; // List of supported operators

        int i; // Loop variable


        for(i=0;op[i]!='\0';i++) // Check each operator
        {
            if(op[i]==ch) // Compare current character
                return 1; // Operator found
        }


        return 0; // Not an operator
}


int isDelimiter(char ch) // Function to check for delimiters
{
        char d[]="(){}[];:"; // List of delimiters

        int i; // Loop variable


        for(i=0;d[i]!='\0';i++) // Check each delimiter
        {
            if(d[i]==ch) // Compare current character
                return 1; // Delimiter found
        }


        return 0; // Not a delimiter
}


void readString(FILE *fp) // Function to read string literals
{
    char ch; // Variable to read characters

    char str[100]; // Store the string

    int i=0; // Start index from 0
    statement_flag=1;

    while((ch=getc(fp))!=EOF) // Read characters until end of string
    { 
        if(ch=='"') // Check for closing double quote
        {
            str[i]='\0'; // End the string

            printf("%-20s %-40s\n",str,"String"); // Print string literal

            return; // Exit the function
        }


        if(ch=='\n') // Check new line inside string
        {
            line_no++; // Increase line number

            lexicalError(line_no,"Unterminated string literal");

            return; // Exit the function
        }


        str[i++] = ch; // Store each character
    }


    lexicalError(line_no,"Unterminated string literal");

    return; // Return if end of file is reached
}


void skipComment(FILE *fp) // Function to skip comments
{
    char ch; // Variable to read characters

    char next; // Variable to check next character


    next=getc(fp); // Read next character


    if(next=='/') // Check single line comment
    {
        while((ch=getc(fp))!=EOF) // Read characters until end of line
        {
            if(ch=='\n') // Check for new line
            {
                line_no++; // Increase line number

                break; // Stop skipping
            }
        }
    }


    else if(next=='*') // Check multi line comment
    {
        while((ch=getc(fp))!=EOF) // Read characters until comment ends
        {
            if(ch=='\n') // Check new line
            {
                line_no++; // Increase line number
            }


            if(ch=='*') // Check closing comment symbol
            {
                ch=getc(fp);


                if(ch=='/') // Check comment ending
                {
                    return; // Exit the function
                }
            }
        }


        lexicalError(line_no,"Unterminated comment");
    }


    else
    {
        ungetc(next,fp); // Put back the character

        printf("%-20c %-40s\n",'/',"Operator"); // Print division operator
    }

}

void checkBrackets() // Function to check brackets and parenthesis
{

    if(round_bracket>0)
    {
        lexicalError(line_no,"Missing ')'");
    }

    if(curly_bracket>0)
    {
        lexicalError(line_no,"Missing '}'");
    }

    if(square_bracket>0)
    {
        lexicalError(line_no,"Missing ']'");
    }
}

int isSpecialLine(char *line) // Function to check lines where semicolon is not required
{

    if(strstr(line,"#include")!=NULL)
        return 1;

    if(strstr(line,"#define")!=NULL)
        return 1;

    if(strstr(line,"main(")!=NULL)
        return 1;

    if(strstr(line,")")!=NULL && strstr(line,"{")==NULL)
        return 1;

    if(strstr(line,"if(")!=NULL)
        return 1;

    if(strstr(line,"for(")!=NULL)
        return 1;

    if(strstr(line,"while(")!=NULL)
        return 1;

    if(strstr(line,"switch(")!=NULL)
        return 1;

    if(strstr(line,"else")!=NULL)
        return 1;

    if(strstr(line,"{")!=NULL)
        return 1;

    if(strstr(line,"#")!=NULL)
    return 1;


    return 0;
}
