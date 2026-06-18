/*  Name : Riddhi B Shitole 
    ID   : 25034f_036
    Project : Lexical Analyzer 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ---------- KEYWORD CHECK ----------
int isKeyword(char *word) {
    char *keywords[] = {
        "int","float","char","if","else","for","while","do","return",
        "void","double","long","short","switch","case","break","continue",
        "struct","typedef","union","static","const","unsigned","signed"
    };
    for (int i = 0; i < 24; i++)
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    return 0;
}

// ---------- VALIDATION FUNCTIONS ----------
int isValidIdentifier(char *word) {
    if (!isalpha(word[0]) && word[0] != '_')
    { 
        return 0;
    }
    for (int i = 1; i < strlen(word); i++)
    {
        if (!isalnum(word[i]) && word[i] != '_')
        {
         return 0;
        }
    }
    return 1;
}
int isHex(char *num) {
    for (int i = 2; i < strlen(num); i++)
    {
        if (!isxdigit(num[i]))
        { 
            return 0;
        }
    }
    return 1;
}

int isBinary(char *num) {
    for (int i = 2; i < strlen(num); i++)
    {
        if (num[i] != '0' && num[i] != '1')
        {
            return 0;
        }
    }
    return 1;
}

int isOctal(char *num) {
    for (int i = 1; i < strlen(num); i++)
    {
        if (num[i] < '0' || num[i] > '7')
        {
            return 0;
        }
    }
    return 1;
}

int isFloat(char *num) {
    int dotCount = 0;
    for (int i = 0; i < strlen(num); i++) 
    {
        if (num[i] == '.') 
        {   
            dotCount++;
        }
        else if (!isdigit(num[i]) && num[i] != 'f' && num[i] != 'F') 
        { 
            return 0;
        }
    }
    return dotCount == 1;
}

int isDelimiter(char ch) {
    return strchr(";,(){}[]", ch) != NULL;
}

int isOperator(char ch) {
    return strchr("+-*/%=!><&|", ch) != NULL;
}

// ---------- MAIN ----------
int main(int argc, char *argv[]) {
    char filename[100];
    if (argc != 2) {
        printf("Usage: %s <c_source_file.c>\n", argv[0]);
        return 1;
    }
    strcpy(filename, argv[1]);

    // Validate file extension
    char *dot = strrchr(filename, '.');
    if (!dot || strcmp(dot, ".c") != 0) 
    {
        printf("Error: Invalid file extension. Only .c files allowed.\n");
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) 
    {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    char line[500], token[100];
    int lineNo = 0, i, j;
    int braceCount = 0, parenCount = 0, bracketCount = 0;
    int controlStatement = 0, doKeyword = 0;

    printf("Lexical analysis of: %s\n", filename);
    printf("\n%-6s %-30s %s\n", "Line", "Token", "Type");
    printf("------ ------------------------------ -------------------------\n");

    while (fgets(line, sizeof(line), fp)) 
    {
        lineNo++;
        i = 0;

        // Preprocessor directive
        if (line[0] == '#') {
            printf("%-6d %-30s %s\n", lineNo, strtok(line, "\n"), "Preprocessor Directive");
            continue;
        }

        while (line[i] != '\0') 
        {
            if (isspace(line[i])) { i++; continue; }

            // ---------- Misplaced semicolon after function declaration ----------
            if (line[i] == ';') {
                // Find '(' and ')' before this semicolon
                char *parenOpen = strchr(line, '(');
                char *parenClose = strrchr(line, ')');
                char *brace = strchr(line, '{');
                if (parenOpen && parenClose && parenClose < &line[i] && (!brace || brace > &line[i])) 
                {
                    char firstWord[50];
                    sscanf(line, "%49s", firstWord);
                    if (isKeyword(firstWord)) {
                        printf("Line %-3d | %-7s | %s\n", lineNo, "Warning", "Misplaced semicolon after function declaration.");
                        i++;  // skip semicolon token
                        continue;
                    }
                }
            }

            // Double semicolon
            if (line[i] == ';' && line[i + 1] == ';') 
            {
                printf("Line %-3d | %-7s | %s\n", lineNo, "Error", "';;' is invalid.");
                i += 2; 
                continue;
            }

            // String literal
            if (line[i] == '"') {
                j = i + 1;
                while (line[j] && line[j] != '"') 
                {
                    j++;
                }
                if (line[j] == '\0') 
                { 
                    printf("Line %-3d | %-7s | %s\n", lineNo, "Error", "Unterminated string literal.");
                    break; 
                }
                printf("%-6d %-30.*s %s\n", lineNo, j - i + 1, &line[i], "String Literal");
                i = j + 1; 
                continue;
            }

            // Char literal
            if (line[i] == '\'') {
                j = i + 1;
                while (line[j] && line[j] != '\'') 
                j++;
                if (line[j] == '\0') 
                { 
                    printf("Line %-3d | %-7s | %s\n", lineNo, "Error", "Unterminated character literal.");
                    break; 
                }
                printf("%-6d %-30.*s %s\n", lineNo, j - i + 1, &line[i], "Character Literal");
                i = j + 1; 
                continue;
            }

            // Delimiters
            if (isDelimiter(line[i])) 
            {
                if (line[i] == '{')
                { 
                    braceCount++;
                }
                else if (line[i] == '}') 
                { 
                    braceCount--; 
                    if (braceCount < 0) 
                    { 
                        printf("Line %-3d | %-7s | %s\n", lineNo, "Error", "Unmatched closing brace '}'.");
                        braceCount = 0; 
                    } 
                }
                else if (line[i] == '(') 
                {
                    parenCount++;
                }
                else if (line[i] == ')') 
                { 
                    parenCount--; 
                    if (parenCount < 0) 
                    { 
                        printf("Line %-3d | %-7s | %s\n", lineNo, "Error", "Unmatched closing parenthesis ')'.");
                        parenCount = 0; 
                    } 
                }
                else if (line[i] == '[') 
                {
                bracketCount++;
                }
                else if (line[i] == ']') 
                { 
                    bracketCount--; 
                    if (bracketCount < 0) 
                    { 
                        printf("Line %-3d | %-7s | %s\n", lineNo, "Error", "Unmatched closing bracket ']'.");
                        bracketCount = 0; 
                    } 
                }

                // Control statement misplaced semicolon
                if (line[i] == ')' && controlStatement) 
                {
                    printf("%-6d %-30c %s\n", lineNo, line[i], "Delimiter");
                    int k = i + 1; while (isspace(line[k])) k++;
                    if (line[k] == ';') 
                    {
                        printf("Line %-3d | %-7s | %s\n", lineNo, "Warning", "Misplaced semicolon after control statement.");
                        i = k + 1; controlStatement = 0; continue;
                    }
                    controlStatement = 0; 
                    i++; 
                    continue;
                }

                if (line[i] == ';' && doKeyword) 
                { 
                    printf("Line %-3d | %-7s | %s\n", lineNo, "Warning", "Misplaced semicolon after 'do'.");
                    doKeyword = 0; 
                    i++; 
                    continue; 
                }

                printf("%-6d %-30c %s\n", lineNo, line[i], "Delimiter");
                i++; 
                continue;
            }

            // Operators
            if (isOperator(line[i])) 
            { 
                printf("%-6d %-30c %s\n", lineNo, line[i], "Operator");
                i++; 
                continue; 
            }

            // Tokens
            j = 0;
            while (!isspace(line[i]) && !isDelimiter(line[i]) && !isOperator(line[i]) && line[i] != '\0')
            {    
                token[j++] = line[i++];
            }
            token[j] = '\0';
            if (strlen(token) == 0) 
            { 
                i++; continue; 
            }

            if (isKeyword(token)) 
            {
                printf("%-6d %-30s %s\n", lineNo, token, "Keyword");
                if (strcmp(token, "for") == 0 || strcmp(token, "if") == 0 || strcmp(token, "while") == 0)
                {
                    controlStatement = 1;
                }
                else if (strcmp(token, "do") == 0) 
                {
                    doKeyword = 1;
                }
            }
            else if (strncmp(token, "0x", 2) == 0 || strncmp(token, "0X", 2) == 0) 
            {
                if (isHex(token))
                { 
                    printf("%-6d %-30s %s\n", lineNo, token, "Hexadecimal Literal");
                }
                else 
                {
                    printf("Line %-3d | %-7s | %s '%s'.\n", lineNo, "Error", "Invalid hexadecimal literal", token);
                }
            }
            else if (strncmp(token, "0b", 2) == 0 || strncmp(token, "0B", 2) == 0) 
            {
                if (isBinary(token)) 
                {
                    printf("%-6d %-30s %s\n", lineNo, token, "Binary Literal");
                }
                else 
                {
                    printf("Line %-3d | %-7s | %s '%s'.\n", lineNo, "Error", "Invalid binary literal", token);
                }
            }

            else if (token[0] == '0' && strlen(token) > 1 && token[1]!='x' && token[1]!='X' && token[1]!='b' && token[1]!='B') 
            {
                if (isOctal(token)) 
                {
                    printf("%-6d %-30s %s\n", lineNo, token, "Octal Literal");
                }
                else 
                {
                    printf("Line %-3d | %-7s | %s '%s'.\n", lineNo, "Error", "Invalid octal literal", token);
                }
            }
            else if (strchr(token, '.')) 
            {
                if (isFloat(token)) 
                {
                    printf("%-6d %-30s %s\n", lineNo, token, "Float Literal");
                }
                else 
                {
                    printf("Line %-3d | %-7s | %s '%s'.\n", lineNo, "Error", "Invalid float literal", token);
                }   
            }
            else if (isdigit(token[0])) 
            {
                int validNum = 1;
                for (int k=0;k<strlen(token);k++) 
                {
                    if(!isdigit(token[k])) 
                    {
                        validNum=0;
                    }
                }
                if(validNum) 
                {
                    printf("%-6d %-30s %s\n", lineNo, token, "Integer Literal");
                }
                else 
                {
                    printf("Line %-3d | %-7s | %s '%s'.\n", lineNo, "Error", "Invalid number", token);
                }
            }
            else if (isValidIdentifier(token)) 
            {
                printf("%-6d %-30s %s\n", lineNo, token, "Identifier");
            }
            else 
            {
                printf("Line %-3d | %-7s | %s '%s'.\n", lineNo, "Error", "Invalid identifier", token);
            }
        }

        // Missing semicolon detection
        int lastCharIndex = strlen(line) - 1;
        while (lastCharIndex >= 0 && isspace(line[lastCharIndex])) 
        {
            lastCharIndex--;
        }
        if (lastCharIndex >= 0) 
        {
            char lastChar = line[lastCharIndex];
            if (braceCount > 0 && lastChar != ';' && lastChar != '{' && lastChar != '}' && lastChar != ':') 
             {
                int skip = 0;
                if (strstr(line,"if")==line || strstr(line,"for")==line || strstr(line,"while")==line || strstr(line,"do")==line || strstr(line,"switch")==line)
                {
                    skip = 1;
                }
                if (!skip) 
                {
                    printf("Line %-3d | %-7s | %s\n", lineNo, "Warning", "Missing semicolon at the end of the statement.");
                }
            }
        }
    }

    fclose(fp);
    return 0;
}
