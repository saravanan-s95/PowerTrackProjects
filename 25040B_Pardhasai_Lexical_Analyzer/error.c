#include <stdio.h>
#include <ctype.h>
#include "main.h"
#include<string.h>
int check_error(char arr[])
{
    int index = 0;      // Index to traverse the lexeme

    // Check whether the lexeme is a numeric constant
    if (isdigit(arr[index]) || arr[index] == '.')
    {
        int dot_count = 0, e_count = 0;          // Count decimal points and exponents
        int digit_count = 0, exp_digit_count = 0; // Count digits before and after exponent

        while (arr[index])      // Traverse the complete lexeme
        {
            if (isdigit(arr[index]))      // Check for digit
            {
                digit_count++;

                if (e_count)      // Count digits after exponent
                    exp_digit_count++;
            }
            else if (arr[index] == '.')      // Check for decimal point
            {
                if (e_count)
                    return INVALID_IDENTIFIER;      // Decimal point after exponent is invalid

                if (++dot_count > 1)
                    return MULTIPLE_DOTS;      // More than one decimal point
            }
            else if (arr[index] == 'e' || arr[index] == 'E')      // Check for exponent
            {
                if (++e_count > 1)
                    return INVALID_IDENTIFIER;      // Multiple exponents are invalid

                if (digit_count == 0)
                    return INVALID_IDENTIFIER;      // Exponent without digits
            }
            else if ((arr[index] == '+' || arr[index] == '-') && index > 0 && (arr[index - 1] == 'e' || arr[index - 1] == 'E'))      // Valid sign after exponent
            {
                // Do nothing
            }
            else
            {
                return INVALID_IDENTIFIER;      // Invalid character in numeric constant
            }

            index++;      // Move to next character
        }

        if (digit_count == 0)
            return INVALID_IDENTIFIER;      // No digits found

        if (e_count && exp_digit_count == 0)
            return INVALID_IDENTIFIER;      // Missing exponent value

        return VALID_LEXEME;      // Valid numeric constant
    }

    // Check whether the lexeme is an identifier
    else if (isalpha(arr[index]) || arr[index] == '_')
    {
        index++;      // Skip first character

        while (arr[index])      // Validate remaining characters
        {
            if (!isalnum(arr[index]) && arr[index] != '_')
                return INVALID_IDENTIFIER;      // Invalid identifier character

            index++;
        }

        return VALID_LEXEME;      // Valid identifier
    }

    // Check whether the lexeme is a string literal
    else if (arr[index] == '"')
    {
        index++;      // Skip opening quote

        while (arr[index])      // Traverse string
        {
            if (arr[index] == '\\' && arr[index + 1] != '\0')
            {
                index += 2;      // Skip escaped character
                continue;
            }

            if (arr[index] == '"')
                return VALID_LEXEME;      // Properly terminated string

            index++;
        }

        return UNTERMINATED_STRING;      // Missing closing quote
    }

    // Check whether the lexeme is a character constant
    else if (arr[index] == '\'')
    {
        index++;      // Skip opening quote

        if (arr[index] == '\\')      // Escaped character constant
        {
            index++;      // Move to escape character

            if (arr[index] == 'n' || arr[index] == 't' || arr[index] == 'r' || arr[index] == '0' || arr[index] == '\'' || arr[index] == '"' || arr[index] == '\\')
            {
                index++;      // Valid escape sequence
            }
            else
            {
                return INVALID_CHARACTER_CONST;      // Invalid escape sequence
            }
        }
        else
        {
            if (arr[index] == '\0' || arr[index] == '\'')
                return INVALID_CHARACTER_CONST;      // Empty character constant

            index++;      // Skip normal character
        }

        if (arr[index] != '\'')
            return INVALID_CHARACTER_CONST;      // Missing closing quote

        index++;

        if (arr[index] != '\0')
            return INVALID_CHARACTER_CONST;      // Extra characters after character constant

        return VALID_LEXEME;      // Valid character constant
    }

    // Check for illegal characters
    else if (!(isalnum(arr[index]) || isspace(arr[index]) || strchr("+-*/%=!<>|&^~?:;,.(){}[]#_'\"", arr[index])))
    {
        return ILLEGAL_CHARACTER;
    }

    // Check for valid header file
    else if (arr[0] == '<' && arr[strlen(arr) - 1] == '>')
    {
        return VALID_LEXEME;
    }

    // Check whether the lexeme is an operator
    else if (strchr("+-*/=%<>!&|", arr[0]))
    {
        int i;      // Loop variable
        int flag = 0;      // Operator found flag

        for (i = 0; i < num_relational_operators; i++)      // Search operator table
        {
            if (strcmp(arr, relational_operators[i]) == 0)
            {
                flag = 1;
                break;
            }
        }

        if (!flag)
            return INVALID_OPERATOR;      // Operator not found

        return VALID_LEXEME;      // Valid operator
    }

    return VALID_LEXEME;      // All other valid lexemes
}

void check_brackets(FILE *fp)
{
    int ch;      // Stores each character read from the file
    int round = 0, curly = 0, square = 0;      // Counters for (), {} and []

    while ((ch = fgetc(fp)) != EOF)      // Read file character by character
    {
        switch (ch)      // Check each bracket type
        {
            case '(':
                round++;      // Count opening round bracket
                break;

            case ')':
                if (round > 0)
                    round--;      // Matching opening bracket found
                else
                    printf(BRIGHT_RED "Extra ')'\n" RESET);      // Extra closing bracket
                break;

            case '{':
                curly++;      // Count opening curly bracket
                break;

            case '}':
                if (curly > 0)
                    curly--;      // Matching opening bracket found
                else
                    printf(BRIGHT_RED "Extra '}'\n" RESET);      // Extra closing bracket
                break;

            case '[':
                square++;      // Count opening square bracket
                break;

            case ']':
                if (square > 0)
                    square--;      // Matching opening bracket found
                else
                    printf(BRIGHT_RED "Extra ']'\n" RESET);      // Extra closing bracket
                break;
        }
    }

    while (round--)      // Print remaining unmatched '('
        printf(RED "Missing ')'\n" RESET);

    while (curly--)      // Print remaining unmatched '{'
        printf(RED "Missing '}'\n" RESET);

    while (square--)      // Print remaining unmatched '['
        printf(RED "Missing ']'\n" RESET);
}void print_error(int ret, char arr[])
{
    switch (ret)      // Select the corresponding error type
    {
        case MULTIPLE_DOTS:
            printf(CYAN "%-5d " YELLOW "%-20s " RED "Invalid numeric constant (multiple decimal points)\n" RESET, line_no, arr);      // Print multiple decimal point error
            break;

        case INVALID_NUMERIC_CONSTANT:
            printf(CYAN "%-5d " YELLOW "%-20s " RED "Invalid numeric constant\n" RESET, line_no, arr);      // Print invalid numeric constant error
            break;

        case INVALID_IDENTIFIER:
            printf(CYAN "%-5d " YELLOW "%-20s " RED "Invalid identifier\n" RESET, line_no, arr);      // Print invalid identifier error
            break;

        case UNTERMINATED_STRING:
            printf(CYAN "%-5d " YELLOW "%-20s " RED "Unterminated string literal\n" RESET, line_no, arr);      // Print unterminated string error
            break;

        case INVALID_CHARACTER_CONST:
            printf(CYAN "%-5d " YELLOW "%-20s " RED "Invalid character constant\n" RESET, line_no, arr);      // Print invalid character constant error
            break;

        case ILLEGAL_CHARACTER:
            printf(CYAN "%-5d " YELLOW "%-20s " RED "Illegal character\n" RESET, line_no, arr);      // Print illegal character error
            break;

        case INVALID_OPERATOR:
            printf(CYAN "%-5d " YELLOW "%-20s " RED "Invalid operator\n" RESET, line_no, arr);      // Print invalid operator error
            break;

        default:
            break;      // No matching error code
    }
}