# Lexical Analyser in C

## Project Overview

This project implements a **Lexical Analyser** for C source files. It scans one or more C files provided through the command line, breaks the source code into meaningful tokens, and classifies them into different lexical categories.

The project also validates matching parentheses, braces, and brackets using a stack-based approach and reports errors for mismatched delimiters.

---

## Features

- Accepts **multiple C source files** through Command Line Arguments (CLA).
- Stores input file names using a **Singly Linked List**.
- Validates input files before processing.
- Performs lexical analysis and classifies:
  - Keywords
  - Identifiers
  - Numbers
  - Operators
  - Symbols
  - Preprocessor Directives
  - String Literals

- Checks balancing of:
  - `()`
  - `{}`
  - `[]`

- Reports syntax errors for unmatched or unexpected parentheses.

---

## Project Structure

```
.
├── main.c
├── lexical_analyser.c
├── parantheses_handler.c
├── insert_last.c
├── keyword_checker.c
├── symbol_checker.c
├── validate_fname.c
├── word_checker.c
├── lexical.h
├── types.h
├── Makefile
└── README.md
```

---

## Modules

### main.c

- Accepts command line arguments.
- Creates the linked list of input files.
- Initializes the stack.
- Invokes the lexical analyser.

### lexical_analyser.c

Core module responsible for:

- Reading source files character by character.
- Token generation.
- Token classification.
- Invoking parenthesis validation.

### parantheses_handler.c

Implements stack-based matching of:

- Parentheses `()`
- Curly braces `{}`
- Square brackets `[]`

Reports descriptive errors whenever delimiters are mismatched.

### insert_last.c

Implements insertion at the end of the linked list used for storing file names.

### keyword_checker.c

Checks whether a scanned word belongs to the list of C language keywords.

### symbol_checker.c

Recognizes valid C symbols such as:

```
()
{}
[]
;
,
```

### validate_fname.c

Validates:

- File extension (`.c`)
- File existence

before lexical analysis begins.

### word_checker.c

Checks whether a token satisfies the rules of a valid identifier/word before further classification.

---

## Data Structures Used

### Singly Linked List

Used for storing all file names passed through the command line.

### Stack

Used for checking balanced:

- Parentheses
- Curly braces
- Square brackets

---

## Compilation

Run:

```bash
make
```

---

## Execution

```bash
./a.out file1.c
```

For multiple files:

```bash
./a.out file1.c file2.c file3.c
```

---

## Sample Output

```
#include "types.h" -> Preprocessor Directive
int -> Keyword
main -> Identifier
( -> Symbol
int -> Keyword
argc -> Identifier
== -> Operator
2 -> Number
"Hello World" -> String Literal
```

If mismatched parentheses are detected:

```
ERROR: Expected '}' but found ')'
In File: sample.c
```

---

## Concepts Used

- Lexical Analysis
- Tokenization
- Command Line Arguments
- File Handling
- Character Processing
- Strings
- Dynamic Memory Allocation
- Singly Linked List
- Stack
- Modular Programming
- Makefile

---

## Author

**Daaman Patel**
