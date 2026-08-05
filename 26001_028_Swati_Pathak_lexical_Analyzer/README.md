# Lexical Analyzer in C

A **Lexical Analyzer (Tokenizer)** built in C that reads a C source file and classifies its contents into meaningful tokens — a core component of compiler front-end design.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Token Types](#token-types)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Sample Output](#sample-output)
- [Error Handling](#error-handling)
- [Future Improvements](#future-improvements)
- [Author](#author)

---

## Overview

This project implements the first phase of a compiler — **lexical analysis**. The analyzer scans a C source file character-by-character and groups characters into tokens based on predefined lexical rules. It also performs basic syntax validation such as brace balancing and unterminated string detection.

---

## Features

- Detects all standard **C keywords** (`int`, `if`, `while`, `return`, etc.)
- Identifies **identifiers** (variable and function names)
- Recognizes **operators** (`+`, `-`, `*`, `/`, `=`, etc.)
- Detects **symbols and braces** (`;`, `{`, `}`, `(`, `)`)
- Parses **numeric constants**
- Handles **string literals**
- Detects **preprocessor directives** (`#include`, `#define`, etc.)
- Skips **single-line** (`//`) and **multi-line** (`/* */`) comments
- Reports **line numbers** for each token

---

## Token Types

| Token Type             | Example                  |
|------------------------|--------------------------|
| Keyword                | `int`, `return`, `while` |
| Identifier             | `count`, `main`, `total` |
| Operator               | `+`, `-`, `=`, `*`       |
| Symbol                 | `;`, `,`, `.`            |
| Curly Brace Opening    | `{`                      |
| Curly Brace Closing    | `}`                      |
| Round Brace Opening    | `(`                      |
| Round Brace Closing    | `)`                      |
| Numeric Constant       | `42`, `100`              |
| String Literal         | `"Hello, World!"`        |
| Preprocessor Directive | `#include <stdio.h>`     |

---

## Project Structure

```
Lexical_Analyzer/
├── main.c        # Core lexer logic
├── header.h      # Token definitions and function declarations
└── README.md
```

---

## Getting Started

### Prerequisites

- GCC compiler
- A Linux/macOS terminal or Windows with MinGW/WSL

### Compile

```bash
gcc main.c -o lexer
```

### Run

```bash
./lexer sample.c
```

Replace `sample.c` with the path to any C source file you want to analyze.

---

## Sample Output

Given the input:

```c
int main() {
    return 0;
}
```

The analyzer produces:

```
Line  1 : Keyword              : int
Line  1 : Identifier           : main
Line  1 : Round brace opening  : (
Line  1 : Round brace closing  : )
Line  1 : Curly brace opening  : {
Line  2 : Keyword              : return
Line  2 : Numeric constant     : 0
Line  2 : Symbol               : ;
Line  3 : Curly brace closing  : }
```

---

## Error Handling

The analyzer detects and reports the following errors:

| Error                    | Description                                      |
|--------------------------|--------------------------------------------------|
| Unbalanced `()`          | More opening or closing round braces than expected  |
| Unbalanced `{}`          | More opening or closing curly braces than expected  |
| Unterminated string      | A string literal that is not properly closed with `"` |

---

## Future Improvements

- [ ] Multi-character operator support (`==`, `!=`, `<=`, `>=`, `&&`, `||`)
- [ ] Floating-point constant parsing (`3.14`, `2.0e5`)
- [ ] Token count statistics summary at end of output
- [ ] Symbol table generation
- [ ] Transition into a syntax analysis (parsing) phase

---

## Concepts Practiced

- Lexical Analysis & Tokenization
- Character classification using `ctype.h`
- File handling in C
- Brace/bracket balancing
- Compiler Design fundamentals

---

## Author

**Swati Pathak**

> This project helped me understand the fundamentals of compiler design and lexical analysis by building the first phase of a compiler from scratch.
