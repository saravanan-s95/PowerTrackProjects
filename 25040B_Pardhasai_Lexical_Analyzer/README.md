# Lexical Analyzer in C

## Overview
This project implements a **Lexical Analyzer (Scanner)** for the C programming language. The analyzer reads a C source file, removes comments and unnecessary white spaces, extracts lexemes, classifies them into different token types, and reports lexical errors. It is the first phase of a compiler and demonstrates how source code is converted into meaningful tokens.

---

## Features

- Removes single-line (`//`) and multi-line (`/* */`) comments.
- Ignores unnecessary white spaces.
- Identifies C language keywords.
- Detects valid identifiers.
- Recognizes integer and floating-point constants.
- Identifies operators and special symbols.
- Detects preprocessor directives.
- Recognizes header files.
- Validates string and character literals.
- Reports lexical errors such as:
  - Invalid identifiers
  - Invalid operators
  - Illegal characters
  - Unterminated string literals
  - Unterminated character literals
  - Invalid numeric constants
  - Mismatched brackets

---

## Token Types

The lexical analyzer classifies the input into the following token categories:

- Preprocessor Directive
- Header File
- Keyword
- Identifier
- Number
- Operator
- Special Symbol
- String Literal
- Character Literal
- Invalid Token

---

## Project Structure

```
.
├── main.c
├── lexer.c
├── lexer.h
├── remove_comments.c
├── classify_token.c
├── check_error.c
├── process.txt
├── input.c
└── README.md
```

---

## Workflow

1. Read the input C source file.
2. Remove comments.
3. Skip unnecessary white spaces.
4. Extract one lexeme at a time.
5. Check for lexical errors.
6. Classify the lexeme into its corresponding token.
7. Display the line number, lexeme, and token type.

---

## Sample Output

```
Line  Lexeme          Token Type
-----------------------------------------
1     #include        PREPROCESSOR_DIRECTIVE
1     <stdio.h>       HEADER_FILE
2     int             KEYWORD
2     main            IDENTIFIER
2     (               SPECIAL_SYMBOL
2     )               SPECIAL_SYMBOL
3     {               SPECIAL_SYMBOL
4     int             KEYWORD
4     num             IDENTIFIER
4     =               OPERATOR
4     10              NUMBER
4     ;               SPECIAL_SYMBOL
```

---

## Error Detection

The lexical analyzer reports various lexical errors, including:

- Invalid Token
- Illegal Character
- Invalid Identifier
- Invalid Numeric Constant
- Invalid Operator
- Unterminated String Literal
- Unterminated Character Literal
- Bracket Mismatch

---

## Concepts Used

- File Handling
- Strings
- Character Processing
- Arrays
- Functions
- Modular Programming
- Compiler Design Fundamentals

---

## Applications

- Compiler Design
- Source Code Analysis
- Syntax Highlighting
- Static Code Analysis
- Programming Language Processing

---

## Future Enhancements

- Support all C99/C11 keywords.
- Generate a symbol table.
- Integrate with syntax analysis (Parser).
- Improve error recovery.
- Support hexadecimal, octal, and binary constants.
- Export token list to a file.

---

## Author

**Pardha Sai**
