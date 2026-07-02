#include "error.h"

#include <ctype.h>
#include <string.h>

// Get error message for each error type
static const char* get_error_message(ErrorType type) {
    switch (type) {
        case ERR_INVALID_IDENTIFIER:
            return "Invalid identifier - cannot start with digit";
        case ERR_INVALID_HEX_NUMBER:
            return "Invalid hexadecimal number";
        case ERR_INVALID_OCTAL_NUMBER:
            return "Invalid octal number";
        case ERR_INVALID_FLOAT_NUMBER:
            return "Invalid floating point number";
        case ERR_INVALID_BINARY_NUMBER:
            return "Invalid binary number";
        case ERR_UNCLOSED_STRING:
            return "Unclosed string literal";
        case ERR_UNCLOSED_CHAR:
            return "Unclosed character literal";
        case ERR_UNCLOSED_PAREN:
            return "Unclosed parenthesis";
        case ERR_UNCLOSED_BRACE:
            return "Unclosed brace";
        case ERR_UNCLOSED_BRACKET:
            return "Unclosed bracket";
        case ERR_UNKNOWN_TOKEN:
            return "Unknown token";
        default:
            return "Unknown error";
    }
}

// Report an error and exit immediately
void report_error_and_exit(ErrorType type, int line_num,
                           const char* token_value) {
    printf("\nLEXICAL ERROR DETECTED!\n");
    printf("════════════════════════════════════════\n");
    printf("Line %d: %s\n", line_num, get_error_message(type));
    printf("Token: '%s'\n", token_value);
    printf("════════════════════════════════════════\n");
    printf("Lexical analysis stopped.\n");
    exit(1);
}

// Check if string is a valid binary number (0b...)
int is_valid_binary(const char* str) {
    if (strlen(str) < 3 || str[0] != '0' || (str[1] != 'b' && str[1] != 'B')) {
        return 0;
    }

    for (int i = 2; str[i] != '\0'; i++) {
        if (str[i] != '0' && str[i] != '1') {
            return 0;
        }
    }
    return 1;
}

// Check if string is a valid hexadecimal number (0x...)
int is_valid_hex(const char* str) {
    if (strlen(str) < 3 || str[0] != '0' || (str[1] != 'x' && str[1] != 'X')) {
        return 0;
    }

    for (int i = 2; str[i] != '\0'; i++) {
        if (!isxdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Check if string is a valid octal number
int is_valid_octal(const char* str) {
    if (str[0] != '0') {
        return 0;
    }

    // Special case: just "0" is valid
    if (str[1] == '\0') {
        return 1;
    }

    // Check remaining digits (must be 0-7)
    for (int i = 1; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '7') {
            return 0;
        }
    }
    return 1;
}

// Enhanced validation for floating point numbers
int is_valid_float(const char* str) {
    int dot_count = 0;
    int digit_before_dot = 0;
    int digit_after_dot = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dot_count++;
            if (dot_count > 1) {
                return 0;  // Multiple dots
            }
        } else if (isdigit(str[i])) {
            if (dot_count == 0) {
                digit_before_dot = 1;
            } else {
                digit_after_dot = 1;
            }
        } else if (i == 0 && (str[i] == '+' || str[i] == '-')) {
            // Allow sign at beginning
            continue;
        } else {
            // Check for scientific notation (e/E)
            if ((str[i] == 'e' || str[i] == 'E') && digit_before_dot) {
                // Check for optional sign after e/E
                if (str[i + 1] == '+' || str[i + 1] == '-') {
                    i++;
                }
                // Check that there are digits after e/E
                int has_digits = 0;
                for (int j = i + 1; str[j] != '\0'; j++) {
                    if (!isdigit(str[j])) {
                        return 0;
                    }
                    has_digits = 1;
                }
                return has_digits;
            }
            return 0;
        }
    }

    // Must have at least one digit before or after dot
    return digit_before_dot || digit_after_dot;
}

// Validate a token and exit on error
int validate_token_and_exit(Token token) {
    // Check for invalid identifiers (starting with digit)
    if (token.type == IDENTIFIER) {
        if (isdigit(token.value[0])) {
            report_error_and_exit(ERR_INVALID_IDENTIFIER, token.line_number,
                                  token.value);
            return 0;
        }
    }

    // Check numeric constants for various errors
    if (token.type == NUMERIC_CONSTANT) {
        // Check for hexadecimal numbers
        if (token.value[0] == '0' &&
            (token.value[1] == 'x' || token.value[1] == 'X')) {
            if (!is_valid_hex(token.value)) {
                report_error_and_exit(ERR_INVALID_HEX_NUMBER, token.line_number,
                                      token.value);
                return 0;
            }
        }
        // Check for binary numbers
        else if (token.value[0] == '0' &&
                 (token.value[1] == 'b' || token.value[1] == 'B')) {
            if (!is_valid_binary(token.value)) {
                report_error_and_exit(ERR_INVALID_BINARY_NUMBER,
                                      token.line_number, token.value);
                return 0;
            }
        }
        // Check for octal numbers
        else if (token.value[0] == '0' && token.value[1] != '.') {
            if (!is_valid_octal(token.value)) {
                report_error_and_exit(ERR_INVALID_OCTAL_NUMBER,
                                      token.line_number, token.value);
                return 0;
            }
        }
        // Check for floating point numbers
        else if (strchr(token.value, '.') != NULL) {
            if (!is_valid_float(token.value)) {
                report_error_and_exit(ERR_INVALID_FLOAT_NUMBER,
                                      token.line_number, token.value);
                return 0;
            }
        }
    }

    // Check for unknown tokens
    if (token.type == UNKNOWN) {
        report_error_and_exit(ERR_UNKNOWN_TOKEN, token.line_number,
                              token.value);
        return 0;
    }

    return 1;
}