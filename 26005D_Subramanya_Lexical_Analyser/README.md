# Lexical Analyzer in C

A command-line tool that reads a C source file and breaks it into tokens
(lexemes), printing each token with its type. Built as an Advanced C
project in the Emertxe embedded systems course.

It only tokenizes. It does not parse, so it does not check things like
matching brackets or valid syntax.

## Features

- Recognizes keywords, identifiers, arrays, constants, literals, operators,
  symbols, preprocessor directives and comments
- Reports unknown characters, bad numbers and unterminated strings/comments
  as `Invalid`
- Colored output in the terminal (one color per token type)
- Optional output file, saved as plain text with no color codes
- Summary of token counts at the end

## Requirements

- Linux or WSL
- `gcc` and `make`

## Build

    make            # build ./build/lexer
    make test       # run on every tests/t*.c file
    make clean      # remove the build/ folder

## Usage

    ./build/lexer <input_file.c> [output_file]

Examples:

    ./build/lexer tests/input_file1.c               # print to screen
    ./build/lexer tests/input_file1.c result.txt    # save to a file

Notes:

- The input file name must end in `.c`
- The output file must not end in `.c` (protects your source files from
  being overwritten)
- Colors are only used when printing to a terminal. Saved files and pipes
  (`| grep`) get plain text.

## Token types

| Type            | Examples                              | Terminal color |
|-----------------|---------------------------------------|----------------|
| Type Keyword    | `int`, `char`, `struct`, `typedef`    | blue           |
| Control Keyword | `if`, `for`, `while`, `return`        | magenta        |
| Directive       | `#include`, `#define`, `#ifdef`       | yellow         |
| Identifier      | `main`, `total_count`                 | default        |
| Array           | `scores[10]`, `arr[i]`                | cyan           |
| Int Constant    | `10`, `0`                             | green          |
| Float Constant  | `3.14`                                | green          |
| Char Literal    | `'a'`, `'\n'`                         | green          |
| String Literal  | `"hello \"world\""`                   | green          |
| Operator        | `+`, `==`, `<<=`, `->`, `&&`          | default        |
| Symbol          | `( ) { } [ ] ; : . ?`                 | default        |
| Comment         | `// line`, `/* block */`              | dim            |
| Invalid         | `@`, `$`, `"unterminated`, `1.2.3`    | red            |

## Sample output

Input (`tests/t1.c`):

    a / b; a/b;
    // line comment
    /* block
       comment */
    /**/
    int x;

Output:

    ==============================================
          LEXICAL ANALYZER  -  C Tokenizer
    ==============================================
    Input file : tests/t1.c

    LEXEME               TYPE
    -------------------- ----------------
    a                    Identifier
    /                    Operator
    b                    Identifier
    ;                    Symbol
    ...
    // line comment      Comment
    /* block
       comment */ Comment
    /**/                 Comment
    int                  Type Keyword
    x                    Identifier
    ;                    Symbol
    -------------------- ----------------

    SUMMARY
      Total tokens : 14
      Keywords     : 1
      Identifiers  : 5
      Constants    : 0
      Comments     : 3
      Invalid      : 0

## Project structure

    Lexical_Analyser/
    ├── Makefile
    ├── README.md
    ├── inc/
    │   └── lexer.h        token types, struct, prototypes, extern tables
    ├── src/
    │   ├── main.c         argument checks, file handling, main loop
    │   ├── lexer.c        get_token(): decides the token kind
    │   ├── readers.c      one reader per token kind (word, number, ...)
    │   ├── tables.c       keyword, directive and operator tables
    │   └── print.c        colors, type names, banner, summary
    ├── tests/             sample inputs (input_file1.c, t1.c ... t8.c)
    └── build/             compiled files (created by make)

## How it works

1. `main` opens the input file and calls `get_token()` in a loop until the
   end of the file. Each call returns exactly one token.
2. `get_token()` skips whitespace and looks at the first character to
   decide what kind of token starts there.
3. A letter or `_` starts a word. The whole word is read and checked
   against the keyword tables, otherwise it is an identifier. An identifier
   directly followed by `[` is read as an array.
4. A digit starts a number: an int, or a float if it has one `.`.
5. A quote starts a char or string literal. A backslash escapes the next
   character, so `\"` does not end a string.
6. A `/` followed by `/` or `*` starts a comment, otherwise it is the
   division operator.
7. Operators use lookahead: the next character is read, and the longest
   match from the operator table wins (`<<=` before `<<` before `<`).
   Characters that are not part of the match are pushed back with `ungetc`.
8. A `#` starts a directive, checked against the directive table.
9. Anything else is reported as `Invalid`.
10. `main` counts tokens per type and prints a summary at the end.

## Testing

`tests/input_file1.c` is a valid C program that uses every supported token
type, and it should produce zero `Invalid` tokens. The other files check
edge cases:

| File     | What it checks                                  |
|----------|-------------------------------------------------|
| t1.c     | comments and `/` as division                    |
| t2.c     | a comment longer than 100 characters            |
| t3.c     | Windows (`\r\n`) line endings                   |
| t4.c     | unterminated block comment                      |
| t5.c     | empty file                                      |
| t6.c     | stray characters (`@`, `$`)                     |
| t7.c     | a string longer than 100 characters             |
| t8.c     | array with a missing `]`                        |

## Known limitations

- Strings and comments longer than about 100 characters are split into
  extra tokens (lexemes are stored in a fixed 100-byte buffer)
- An unterminated `/*` comment swallows the rest of the file
- `arr [0]` (space before `[`) is not detected as an array, and the whole
  `[...]` index is part of the array token, so `arr[i+1]` is one token
- In `m[2][3]`, only the first bracket pair is part of the array token
- Hex, exponent and suffixed numbers (`0x1F`, `1e5`, `10UL`) are not handled
- `<stdio.h>` is split into separate tokens (`<`, `stdio`, `.`, `h`, `>`)
- A space after `#` (`# include`) is reported as invalid
- Backslash-continued macro lines are not handled
