/*
    Name		: DAAMAN PATEL
    Date		: 01-08-2026
    Project		: Lexical Analyser in C
    Description : Console-based Lexical Analyser for C source files.
                  Reads one or more C files provided through Command Line Arguments (CLA)
                  and performs lexical analysis by identifying different tokens such as
                  keywords, identifiers, numbers, operators, symbols, preprocessor
                  directives and string literals.
                  Also validates matching parentheses, braces and brackets using a stack
                  and reports errors for mismatched delimiters.
                  Developed using File I/O, Linked List, Stack and standard C libraries.

    Sample I/O  :
                  $ ./a.out main.c

                  #include "types.h"          -> Preprocessor Directive
                  #include "lexical.h"        -> Preprocessor Directive
                  int                         -> Keyword
                  main                        -> Identifier
                  (                           -> Symbol
                  int                         -> Keyword
                  argc                        -> Identifier
                  )                           -> Symbol
                  {                           -> Symbol
                  if                          -> Keyword
                  argc                        -> Identifier
                  <                           -> Operator
                  2                           -> Number
                  return                      -> Keyword
                  }                           -> Symbol
*/

#include "types.h"
#include "lexical.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Not sufficient arguments\n");
        return 0;
    }

    File_List *head = NULL;
    Status ret = storing_cla_fnames_to_LL(argv, &head);

    // Initialising Stack
    Stack stk;

    if (ret != e_success)
    {
        printf("ERROR: File Names could not be stored in list\n");
        return 0;
    }

    Status lex_ret = lexical_analyser(&head, &stk);
}