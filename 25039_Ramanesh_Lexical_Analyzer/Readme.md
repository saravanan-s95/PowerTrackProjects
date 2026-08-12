
# Lexical Analyzer

## Overview

The Lexical Analyzer is the first phase of a compiler. It reads a C source file character by character and converts the input into a sequence of tokens. This project identifies different types of tokens such as keywords, identifiers, numbers, operators, delimiters, strings, and comments.

---

## Features

- Identifies C keywords
- Identifies identifiers
- Recognizes integer constants
- Recognizes floating-point constants
- Identifies operators
- Identifies delimiters
- Reads string literals
- Skips single-line comments
- Displays the lexeme along with its corresponding token

---

## Project Structure

```
Lexical_Analyzer/
│── main.c
│── lexer.c
│── keywords.c
│── lexer.h
│── Makefile
│── README.md
```

---

## Working

1. Opens the input C source file.
2. Reads the file character by character.
3. Ignores white spaces.
4. Identifies the type of token based on the current character.
5. Prints the lexeme and its corresponding token.
6. Continues until the end of the file.

---

## Tokens Recognized

| Token | Description |
|--------|-------------|
| Keyword | Reserved words in C |
| Identifier | User-defined variable/function names |
| Integer | Integer constants |
| Float | Floating-point constants |
| Operator | Arithmetic, Relational and Logical operators |
| Delimiter | Symbols such as `()`, `{}`, `;`, `:` |
| String | String literals enclosed in double quotes |

---

## Compilation

Compile the project using GCC.

```bash
gcc *.c -o lexer
```

or

```bash
make
```

---

## Execution

Run the executable by passing a C source file as an argument.

```bash
./lexer sample.c
```

---

## Sample Input

```c
#include<stdio.h>

int main()
{
    int x = 10;
    float p = 153.35;
    printf("Hello World");
    return 0;
}
```

---

## Sample Output

```
LEXEME              TOKEN

int                 Keyword
main                Identifier
(                   Delimiter
)                   Delimiter
{                   Delimiter
int                 Keyword
x                   Identifier
=                   Operator
10                  Integer
;                   Delimiter
float               Keyword
p                   Identifier
=                   Operator
153.35              Float
;                   Delimiter
printf              Identifier
(                   Delimiter
Hello World         String
)                   Delimiter
;                   Delimiter
return              Keyword
0                   Integer
;                   Delimiter
}                   Delimiter
```

---

## Functions

### tokenize()

Reads the source file and identifies the type of token.

### readIdentifier()

Reads identifiers and checks whether they are keywords.

### readNumber()

Reads integer and floating-point constants.

### readOperator()

Identifies operators and delimiters.

### readString()

Reads string literals enclosed within double quotes.

### skipComment()

Skips single-line comments.

### isKeyword()

Checks whether a given word is a C keyword.

### isOperator()

Checks whether a character is an operator.

### isDelimiter()

Checks whether a character is a delimiter.

---

## Technologies Used

- C Programming
- Standard C Library
- GCC Compiler
- Linux / Ubuntu

---

## Learning Outcomes

- Understanding of compiler design basics
- File handling in C
- Character-by-character parsing
- String manipulation
- Token recognition
- Modular programming
- Header file usage
- Working with standard library functions

---

## Future Enhancements

- Support for multi-line comments
- Support for character literals
- Support for hexadecimal and octal numbers
- Recognition of preprocessor directives
- Recognition of header files
- Recognition of multi-character operators (`==`, `!=`, `<=`, `>=`, `&&`, `||`)
- Symbol table generation
- Lexical error detection

---

## Author

**Ramanesh M**
