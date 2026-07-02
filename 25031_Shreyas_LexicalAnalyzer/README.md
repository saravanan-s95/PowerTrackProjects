# Lexical Analyzer in C

A simple lexical analyzer for C source files written in C. The program scans a C source file, identifies different types of tokens, and reports lexical errors if any invalid tokens are found.

## Features

* Identifies C reserved keywords
* Detects identifiers
* Recognizes numeric constants
* Recognizes string and character literals
* Identifies operators
* Detects braces, parentheses, brackets, commas, and semicolons
* Reports lexical errors such as:

  - Invalid identifiers
  - Invalid hexadecimal, binary, octal, and floating-point numbers
  - Unclosed string literals
  - Unclosed character literals
  - Unmatched parentheses, braces, and brackets
  - Unknown tokens

## Build

```bash
make
```

## Run

```bash
./lexer test.c
```

or simply:

```bash
./lexer
```

and enter the C source file name when prompted.

## Sample Output

```
Line 2: int                  -> Reserved Keyword
Line 2: main                 -> Identifier
Line 2: (                    -> Open Parenthesis
Line 2: )                    -> Close Parenthesis
Line 2: {                    -> Open Brace
...
Lexical analysis completed successfully!
```

If an invalid token is encountered:

```
LEXICAL ERROR DETECTED!

Line 8: Invalid identifier - cannot start with digit
Token: '1arr'

Lexical analysis stopped.
```

