# Lexical Analyzer

## Overview

This project implements a lexical analyzer for C source files. The program accepts a single `.c` file as a command-line argument and classifies the input into different token categories.

---

## Files

```text
.
├── main.c
├── lexer.c
├── lexer.h
├── test.c
└── README.md
```

---

## Features

- Accepts a single `.c` file as input.
- Identifies keywords.
- Identifies identifiers.
- Identifies operators.
- Identifies special characters.
- Identifies character literals.
- Identifies string literals.
- Identifies preprocessor directives.
- Skips single-line comments.
- Skips multi-line comments.
- Detects unterminated character literals.
- Detects unterminated string literals.
- Detects unterminated multi-line comments.

---

## Token Categories

- Keywords
- Identifiers
- Operators
- Special characters
- Character literals
- String literals

---

## Compilation

```bash
gcc main.c lexer.c
```

---

## Execution

```bash
./a.out test.c
```

---

## Error Handling

- Invalid file extension
- File opening errors
- Unterminated character literals
- Unterminated string literals
- Unterminated multi-line comments

---

## Author

Project: Lexical Analyzer
