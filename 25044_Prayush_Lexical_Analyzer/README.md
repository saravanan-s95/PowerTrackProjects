# Lexical Analyzer in C

A lightweight, stream-based Lexical Analyzer (Tokenizer) for C source code written in C11. It reads C source files, strips out preprocessor directives and comments, categorizes characters into structured lexical tokens, and performs lexical validations.

---

## Table of Contents

- [Features](#features)
- [Project Architecture](#project-architecture)
- [Supported Token Types](#supported-token-types)
- [Lexical Validations & Error Handling](#lexical-validations--error-handling)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building](#building)
  - [Usage](#usage)
- [Example](#example)
- [Directory Structure](#directory-structure)
- [License](#license)

---

## Features

- **Accurate Token Categorization**: Identifies Keywords, Identifiers, Operators, Symbols, String/Char Literals, and Numeric/Floating-point Constant Literals.
- **Fast Lookup**: Utilizes binary search ($O(\log N)$) across sorted tables of C keywords, operators, and symbols.
- **Preprocessor & Comment Stripping**:
  - Automatically skips preprocessor directives (`#include`, `#define`, etc.).
  - Handles single-line comments (`// ...`) and multi-line comments (`/* ... */`).
- **Bracket Balance Tracking**: Tracks and validates balance for parentheses `()`, curly braces `{}`, and square brackets `[]`.
- **Lexical Error Detection**:
  - Catches invalid identifiers (e.g., tokens starting with numeric digits like `123var` or invalid symbol prefixes).
  - Catches unterminated string and character literals.
  - Catches mismatched bracket counts.
- **Clean & Modular C11 Codebase**: Clear separation between tokenization, token handling, validations, logging, and data structures.

---

## Project Architecture

```
[ Input File (.c) ]
        │
        ▼
┌──────────────────┐
│   tokenizer.c    │ ◄── Handles file I/O, skips comments & preprocessor directives
└─────────┬────────┘
          │ (character streams)
          ▼
┌──────────────────┐
│ token_handler.c  │ ◄── Groups characters into token strings & tracks brackets
└─────────┬────────┘
          │ (token lookups)
          ▼
┌──────────────────┐
│ validations.c    │ ◄── Binary search for Keywords, Operators, and Symbols
└─────────┬────────┘
          │
          ▼
[ Classified Tokens / Error Messages ]
```

---

## Supported Token Types

| Token Type | Description | Examples |
| :--- | :--- | :--- |
| `KEYWORD` | Standard C language reserved keywords | `int`, `return`, `if`, `while`, `struct`, `sizeof`, `volatile`, etc. |
| `IDENTIFIER` | User-defined variable, function, or type names | `main`, `argc`, `argv`, `total_count`, `my_var` |
| `OPERATOR` | Arithmetic, relational, logical, bitwise, and member access operators | `+`, `-`, `*`, `/`, `%`, `++`, `--`, `==`, `!=`, `<=`, `>=`, `&&`, `\|\|`, `->`, `.`, etc. |
| `SYMBOL` | Delimiters and punctuators | `(`, `)`, `{`, `}`, `[`, `]`, `,`, `;` |
| `STRING LITERAL` | String and character literal expressions (with escape sequence support) | `"Hello, World!\n"`, `'a'`, `'\0'` |
| `CONSTANT LITERAL`| Integer and floating-point numeric constants | `42`, `3.14`, `.5` |

---

## Lexical Validations & Error Handling

The analyzer performs validation checks during and after tokenization:

1. **File Extension Check**: Ensures the input file is a valid `.c` source file.
2. **Unterminated Strings**: Emits `ERROR: Unterminated string literal` if a string or character literal spans past a newline without closing quotes.
3. **Invalid Identifiers**: Emits `ERROR: Invalid identifier` if an identifier begins with numeric digits (e.g., `123name`) or invalid symbol prefixes.
4. **Bracket Balance Validation**: Emits `ERROR: Bracket count mismatch` if the counts of open and closed braces `{}`, parentheses `()`, or brackets `[]` do not match.

---

## Getting Started

### Prerequisites

- GCC or Clang compiler with C11 support (`-std=c11`)
- GNU Make

### Building

To compile the project, run:

```bash
make
```

To clean build artifacts:

```bash
make clean
```

### Usage

Run the executable by passing a target `.c` file as an argument:

```bash
./Lexical_Analyzer <path-to-source-file.c>
```

---

## Example

Given a sample file `sample.c`:

```c
int main() {
    int count = 10;
    return 0;
}
```

Running the analyzer:

```bash
./Lexical_Analyzer sample.c
```

**Output:**

```text
INFO: Validating Arguments
INFO: Opening File for analysis
KEYWORD : int
IDENTIFIER : main
SYMBOL : (
SYMBOL : )
SYMBOL : {
KEYWORD : int
IDENTIFIER : count
OPERATOR : =
CONSTANT LITERAL : 10
SYMBOL : ;
KEYWORD : return
CONSTANT LITERAL : 0
SYMBOL : ;
SYMBOL : }
```

---

## Directory Structure

```text
.
├── Makefile              # Build configuration
├── README.md             # Project documentation
├── src/
│   ├── logs.h            # Logging macros and error string constants
│   ├── main.c            # CLI entry point and orchestration
│   ├── token_handler.c   # Token parsing and bracket tracker implementation
│   ├── token_handler.h   # Token handler interface and BracketTracker struct
│   ├── tokenizer.c       # File reader, comment stripper, and dispatcher
│   ├── tokenizer.h       # Tokenizer interface
│   ├── types.h           # Core data types (Status, TokenType, Token)
│   ├── validations.c     # Binary search lookup tables (Keywords, Operators, Symbols)
│   └── validations.h     # Validation prototypes
└── map.txt               # Character map data
```

---

## License

This project is open source and available under the [MIT License](LICENSE).
