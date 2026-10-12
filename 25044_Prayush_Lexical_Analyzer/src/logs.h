#ifndef LOGS_H
#define LOGS_H
#include <stdio.h>
/*-------------------
    INFO LOGS
 -------------------*/
#define INFO_VALIDATE_ARGUMENTS "Validating Arguments"
#define INFO_OPEN_FILE "Opening File for analysis"
/*-------------------
    ERROR LOGS
 -------------------*/
#define ERROR_INVALID_ARGUMENTS "Invalid Arguments"
#define ERROR_OPEN_FILE "Opening File for analysis"
#define ERROR_BRACKET_MISMATCH "Bracket count mismatch"
#define ERROR_INVALID_IDENTIFIER "Invalid identifier"
#define ERROR_UNTERMINATED_STRING "Unterminated string literal"
/*-------------------
    LOGGERS
 -------------------*/
#define LOG_INFO(msg) printf("INFO: %s\n", msg);
#define LOG_ERROR(msg) printf("ERROR: %s\n", msg);
#endif // !LOGS_H
