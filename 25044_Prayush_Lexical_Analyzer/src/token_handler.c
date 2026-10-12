#include "token_handler.h"
#include "logs.h"
#include "types.h"
#include "validations.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char *token_type_to_string(TokenType type) {
    switch (type) {
    case TOKEN_KEYWORD:
        return "KEYWORD";
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_OPERATOR:
        return "OPERATOR";
    case TOKEN_SYMBOL:
        return "SYMBOL";
    case TOKEN_STRING_LITERAL:
        return "STRING LITERAL";
    case TOKEN_CONSTANT_LITERAL:
        return "CONSTANT LITERAL";
    default:
        return "UNKNOWN";
    }
}

void print_token(TokenType type, const char *token) {
    printf("%s : %s\n", token_type_to_string(type), token);
}

void handle_bracket(int ch, BracketTracker *tracker) {
    if (!tracker)
        return;

    if (ch == '{')
        tracker->open_curly++;
    else if (ch == '}')
        tracker->close_curly++;
    else if (ch == '(')
        tracker->open_paren++;
    else if (ch == ')')
        tracker->close_paren++;
    else if (ch == '[')
        tracker->open_square++;
    else if (ch == ']')
        tracker->close_square++;
}

Status validate_brackets(const BracketTracker *tracker) {
    if (!tracker)
        return success;

    int total_open = tracker->open_curly + tracker->open_paren + tracker->open_square;
    int total_close = tracker->close_curly + tracker->close_paren + tracker->close_square;

    if (total_open != total_close ||
        tracker->open_curly != tracker->close_curly ||
        tracker->open_paren != tracker->close_paren ||
        tracker->open_square != tracker->close_square) {
        LOG_ERROR(ERROR_BRACKET_MISMATCH);
        return failure;
    }

    return success;
}

Status handle_string_token(FILE *fptr, int quote_char, int *line, int *col) {
    char word[100];
    int idx = 0;
    int ch;
    bool terminated = false;

    word[idx++] = (char)quote_char;
    if (col)
        (*col)++;

    while ((ch = fgetc(fptr)) != EOF) {
        if (idx < 99)
            word[idx++] = (char)ch;
        if (ch == '\\') {
            if (col)
                (*col)++;
            int escaped = fgetc(fptr);
            if (escaped != EOF && idx < 99) {
                word[idx++] = (char)escaped;
                if (col)
                    (*col)++;
            }
            continue;
        }
        if (ch == '\n') {
            if (line)
                (*line)++;
            if (col)
                *col = 1;
            break;
        } else {
            if (col)
                (*col)++;
        }
        if (ch == quote_char) {
            terminated = true;
            break;
        }
    }

    word[idx] = '\0';

    if (!terminated) {
        LOG_ERROR(ERROR_UNTERMINATED_STRING);
        return failure;
    }

    print_token(TOKEN_STRING_LITERAL, word);
    return success;
}

Status handle_alnum_token(FILE *fptr, int first_char, int *line, int *col) {
    (void)line;
    int ch = first_char;

    // Handle floating point or numeric constant starting with digit or '.'
    int peek_ch = EOF;
    if (ch == '.') {
        peek_ch = fgetc(fptr);
        if (peek_ch != EOF)
            ungetc(peek_ch, fptr);
    }

    if (isdigit(ch) || (ch == '.' && peek_ch != EOF && isdigit(peek_ch))) {
        char word[100];
        int idx = 0;
        bool has_decimal = (ch == '.');

        word[idx++] = (char)ch;
        if (col)
            (*col)++;

        while ((ch = fgetc(fptr)) != EOF) {
            if (isdigit(ch)) {
                if (idx < 99)
                    word[idx++] = (char)ch;
                if (col)
                    (*col)++;
            } else if (ch == '.' && !has_decimal) {
                has_decimal = true;
                if (idx < 99)
                    word[idx++] = (char)ch;
                if (col)
                    (*col)++;
            } else {
                break;
            }
        }

        word[idx] = '\0';

        // Check for invalid identifier starting with numbers (e.g. 123var)
        if (ch != EOF && (isalpha(ch) || ch == '_')) {
            LOG_ERROR(ERROR_INVALID_IDENTIFIER);
            return failure;
        }

        if (ch != EOF)
            ungetc(ch, fptr);

        print_token(TOKEN_CONSTANT_LITERAL, word);
        return success;
    }

    // Handle Keywords and Identifiers starting with alpha or '_'
    if (isalpha(ch) || ch == '_') {
        char word[100];
        int idx = 0;

        word[idx++] = (char)ch;
        if (col)
            (*col)++;

        while ((ch = fgetc(fptr)) != EOF && (isalnum(ch) || ch == '_')) {
            if (idx < 99)
                word[idx++] = (char)ch;
            if (col)
                (*col)++;
        }

        word[idx] = '\0';

        if (ch != EOF)
            ungetc(ch, fptr);

        print_token(is_keyword(word) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER, word);
        return success;
    }

    return success;
}

Status handle_operator_symbol_token(FILE *fptr, int first_char, int *line, int *col) {
    (void)line;
    int ch = first_char;
    int next_ch = fgetc(fptr);

    char op2[3] = {(char)ch, (char)(next_ch != EOF ? next_ch : 0), '\0'};
    char op1[2] = {(char)ch, '\0'};

    if (next_ch != EOF && is_operator(op2)) {
        print_token(TOKEN_OPERATOR, op2);
        if (col)
            *col += 2;
        return success;
    }

    if (is_operator(op1)) {
        if (next_ch != EOF)
            ungetc(next_ch, fptr);
        print_token(TOKEN_OPERATOR, op1);
        if (col)
            (*col)++;
        return success;
    }

    if (is_symbol(op1)) {
        if (next_ch != EOF)
            ungetc(next_ch, fptr);
        print_token(TOKEN_SYMBOL, op1);
        if (col)
            (*col)++;
        return success;
    }

    // Check if an invalid non-alphanumeric symbol (not an operator or symbol) is attached to an identifier start
    if (next_ch != EOF && (isalpha(next_ch) || next_ch == '_')) {
        char invalid_token[100];
        int idx = 0;
        invalid_token[idx++] = (char)ch;
        invalid_token[idx++] = (char)next_ch;

        while ((ch = fgetc(fptr)) != EOF && (isalnum(ch) || ch == '_')) {
            if (idx < 99)
                invalid_token[idx++] = (char)ch;
        }
        invalid_token[idx] = '\0';

        if (ch != EOF)
            ungetc(ch, fptr);

        LOG_ERROR(ERROR_INVALID_IDENTIFIER);
        printf("Debug: Found invalid symbol-prefixed identifier '%s'\n", invalid_token);
        return failure;
    }

    if (next_ch != EOF)
        ungetc(next_ch, fptr);

    if (col)
        (*col)++;
    return success;
}
