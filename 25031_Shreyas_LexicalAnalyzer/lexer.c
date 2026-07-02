#include "lexer.h"

#include <stdio.h>

#include "error.h"
#include "types.h"

// Static variables for tracking delimiter
static int paren_count = 0;
static int brace_count = 0;
static int bracket_count = 0;

// Reserved keywords
const char* keywords[] = {"auto",     "break",   "case",   "char",     "const",
                          "continue", "default", "do",     "double",   "else",
                          "enum",     "extern",  "float",  "for",      "goto",
                          "if",       "int",     "long",   "register", "return",
                          "short",    "signed",  "sizeof", "static",   "struct",
                          "switch",   "typedef", "union",  "unsigned", "void",
                          "volatile", "while"};

const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

// Operators
const char* operators[] = {"+",  "-",  "*",  "/",  "%",  "++", "--", "==",
                           "!=", "<",  ">",  "<=", ">=", "&&", "||", "!",
                           "=",  "+=", "-=", "*=", "/=", "%=", "&",  "|",
                           "^",  "~",  "<<", ">>", "->", ".",  "&",  "*"};

const int num_operators = sizeof(operators) / sizeof(operators[0]);

// Function to check if string is a keyword
int is_keyword(const char* str) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return e_failure;
}

// Function to check if string is an operator
int is_operator(const char* str) {
    for (int i = 0; i < num_operators; i++) {
        if (strcmp(str, operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if string is numeric
int is_numeric(const char* str) {
    if (*str == '\0')
        return 0;

    // Check each character
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            // Allow decimal point for floating numbers
            if (str[i] == '.' && i > 0 && str[i + 1] != '\0') {
                // Check remaining characters after decimal
                for (int j = i + 1; str[j] != '\0'; j++) {
                    if (!isdigit(str[j]))
                        return 0;
                }
                return 1;
            }
            return 0;
        }
    }
    return 1;
}

// Function to check if valid identifier
int is_valid_identifier(const char* str) {
    if (!isalpha(str[0]) && str[0] != '_') {
        return 0;
    }

    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') {
            return 0;
        }
    }
    return 1;
}

// Convert token type to string for printing
const char* token_type_to_string(TokenType type) {
    switch (type) {
        case KEYWORD:
            return "Reserved Keyword";
        case IDENTIFIER:
            return "Identifier";
        case NUMERIC_CONSTANT:
            return "Numeric Constant";
        case STRING_CONSTANT:
            return "String Constant";
        case CHARACTER_CONSTANT:
            return "Character Constant";
        case OPERATOR:
            return "Operator";
        case PREPROCESSOR_DIRECTIVE:
            return "Preprocessor Directive";
        case OPEN_BRACE:
            return "Open Brace";
        case CLOSE_BRACE:
            return "Close Brace";
        case OPEN_PAREN:
            return "Open Parenthesis";
        case CLOSE_PAREN:
            return "Close Parenthesis";
        case OPEN_BRACKET:
            return "Open Bracket";
        case CLOSE_BRACKET:
            return "Close Bracket";
        case SEMICOLON:
            return "Semicolon";
        case COMMA:
            return "Comma";
        case DELIMITER:
            return "Delimiter";
        default:
            return "Unknown";
    }
}

// Print token information
void print_token(Token token) {
    printf("Line %d: %-20s -> %s\n", token.line_number, token.value,
           token_type_to_string(token.type));
}

// Get next token from file
Token get_next_token(FILE* file, int* line_num) {
    static char line[MAX_LINE_LEN] = "";
    static int pos = 0;
    static int current_line = 1;
    Token token;
    token.line_number = current_line;

    // If we need a new line
    if (line[pos] == '\0') {
        if (fgets(line, MAX_LINE_LEN, file) == NULL) {
            strcpy(token.value, "EOF");
            token.type = UNKNOWN;
            return token;
        }
        pos = 0;
        current_line++;
        token.line_number = current_line;

        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Skip preprocessor directives entirely
        if (line[0] == '#') {
            // Skip this line and get next token
            line[0] = '\0';
            return get_next_token(file, line_num);
        }

        // Skip single-line comments (//)
        if (strstr(line, "//") != NULL) {
            // Truncate at comment start
            char* comment_start = strstr(line, "//");
            *comment_start = '\0';
            // If line is now empty, get next token
            if (line[0] == '\0') {
                line[0] = '\0';
                return get_next_token(file, line_num);
            }
        }

        // // Check for preprocessor directive
        // if (line[0] == '#') {
        //     strcpy(token.value, line);
        //     token.type = PREPROCESSOR_DIRECTIVE;
        //     token.line_number = current_line - 1;
        //     return token;
        // }
    }

    // Skip whitespace
    while (isspace(line[pos])) {
        pos++;
        if (line[pos] == '\0') {
            line[0] = '\0';
            return get_next_token(file, line_num);
        }
    }

    // Handle special single-character tokens
    char ch = line[pos];

    switch (ch) {
        case '{':
            token.value[0] = '{';
            token.value[1] = '\0';
            token.type = OPEN_BRACE;
            brace_count++;
            pos++;
            return token;
        case '}':
            token.value[0] = '}';
            token.value[1] = '\0';
            token.type = CLOSE_BRACE;
            brace_count--;
            if (brace_count < 0) {
                report_error_and_exit(ERR_UNCLOSED_BRACE, current_line, "}");
            }
            pos++;
            return token;
        case '(':
            token.value[0] = '(';
            token.value[1] = '\0';
            token.type = OPEN_PAREN;
            paren_count++;
            pos++;
            return token;
        case ')':
            token.value[0] = ')';
            token.value[1] = '\0';
            token.type = CLOSE_PAREN;
            paren_count--;
            if (paren_count < 0) {
                report_error_and_exit(ERR_UNCLOSED_PAREN, current_line, ")");
            }
            pos++;
            return token;
        case '[':
            token.value[0] = '[';
            token.value[1] = '\0';
            token.type = OPEN_BRACKET;
            bracket_count++;
            pos++;
            return token;
        case ']':
            token.value[0] = ']';
            token.value[1] = '\0';
            token.type = CLOSE_BRACKET;
            bracket_count--;
            if (bracket_count < 0) {
                report_error_and_exit(ERR_UNCLOSED_BRACKET, current_line, "]");
            }
            pos++;
            return token;
        case ';':
            token.value[0] = ';';
            token.value[1] = '\0';
            token.type = SEMICOLON;
            pos++;
            return token;
        case ',':
            token.value[0] = ',';
            token.value[1] = '\0';
            token.type = COMMA;
            pos++;
            return token;
    }

    // Handle string literals
    if (ch == '"') {
        int i = 0;
        token.value[i++] = ch;
        pos++;

        int start_line = current_line;

        while (line[pos] != '"' && line[pos] != '\0' && i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = line[pos++];
        }

        if (line[pos] == '"') {
            token.value[i++] = line[pos++];
            token.value[i] = '\0';
            token.type = STRING_CONSTANT;
        } else {
            // Unclosed string - exit immediately
            token.value[i] = '\0';
            token.type = STRING_CONSTANT;
            report_error_and_exit(ERR_UNCLOSED_STRING, start_line, token.value);
        }
        return token;
    }

    // Handle character literals
    if (ch == '\'') {
        int i = 0;
        token.value[i++] = ch;
        pos++;

        int start_line = current_line;

        while (line[pos] != '\'' && line[pos] != '\0' &&
               i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = line[pos++];
        }

        if (line[pos] == '\'') {
            token.value[i++] = line[pos++];
            token.value[i] = '\0';
            token.type = CHARACTER_CONSTANT;
        } else {
            // Unclosed character - exit immediately
            token.value[i] = '\0';
            token.type = CHARACTER_CONSTANT;
            report_error_and_exit(ERR_UNCLOSED_CHAR, start_line, token.value);
        }
        return token;
    }

    int i = 0;

    // Handle operators
    for (int op_len = 3; op_len >= 1; op_len--) {
        if (pos + op_len <= strlen(line)) {
            char temp[4] = {0};
            strncpy(temp, line + pos, op_len);

            if (is_operator(temp)) {
                strcpy(token.value, temp);
                token.type = OPERATOR;
                pos += op_len;
                return token;
            }
        }
    }

    // Extract alphanumeric token
    while ((isalnum(line[pos]) || line[pos] == '_' || line[pos] == '.') &&
           line[pos] != '\0') {
        if (i < MAX_TOKEN_LEN - 1) {
            token.value[i++] = line[pos++];
        } else {
            break;
        }
    }

    token.value[i] = '\0';

    // Classify the token
    if (token.value[0] != '\0') {
        if (is_keyword(token.value)) {
            token.type = KEYWORD;
        } else if (is_numeric(token.value)) {
            token.type = NUMERIC_CONSTANT;
        } else if (is_valid_identifier(token.value)) {
            token.type = IDENTIFIER;
        } else {
            token.type = UNKNOWN;
        }
    } else {
        // Move to next character and try again
        pos++;
        return get_next_token(file, line_num);
    }

    return token;
}

// Analyze file
void analyze_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return;
    }

    // Reset delimiter counts
    paren_count = 0;
    brace_count = 0;
    bracket_count = 0;

    printf("\n========================================\n");
    printf("Lexical Analysis of: %s\n", filename);
    printf("========================================\n\n");

    int line_num = 1;
    Token token;

    do {
        token = get_next_token(file, &line_num);
        if (strcmp(token.value, "EOF") != 0 && token.value[0] != '\0') {
            // Validate token - will exit on error
            if (validate_token_and_exit(token)) {
                print_token(token);
            }
        }
    } while (strcmp(token.value, "EOF") != 0);

    // Check for unclosed delimiters
    if (paren_count > 0) {
        report_error_and_exit(ERR_UNCLOSED_PAREN, line_num, "(");
    }
    if (brace_count > 0) {
        report_error_and_exit(ERR_UNCLOSED_BRACE, line_num, "{");
    }
    if (bracket_count > 0) {
        report_error_and_exit(ERR_UNCLOSED_BRACKET, line_num, "[");
    }

    printf("\nLexical analysis completed successfully!\n");

    fclose(file);
}