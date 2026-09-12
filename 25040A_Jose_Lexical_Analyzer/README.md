# C Lexical Analyzer

A custom Lexical Analyzer built entirely in C. This program reads a C source code file, separates it into distinct tokens (keywords, operators, identifiers, and constants), and performs foundational syntax error checking. 

This project demonstrates file I/O operations, string manipulation, tokenization logic, and the use of a custom stack data structure for syntax validation.

## Features
* **Token Categorization:** Identifies and categorizes standard C keywords, special characters, operators, identifiers, and string/number literals.
* **Pre-processor Detection:** Identifies pre-processor directives (e.g., `#include`).
* **Syntax Error Checking:**
  * Validates matching quotes (`" "`).
  * Validates matching parentheses (`( )`).
  * Uses a custom stack implementation to ensure all flower brackets (`{ }`) are properly opened and closed.
* **Assignment Validation:** Detects invalid hexadecimal and octal value assignments.

## File Structure
The project consists of the core analyzer source code and a dedicated test file. 

**Source Code:**
* `1_lexical_main.c`
* `2_token_analysis.c`
* `3_error_analysis.c`
* `4_lexical.h`

**Test Input (Not part of the source code):**
* `input_c_file.c` - A dummy C file provided explicitly for testing the analyzer. It contains deliberate syntax and assignment errors to demonstrate the error-checking capabilities of the program.

## Compilation
Since `input_c_file.c` is the test file and not part of the analyzer's core logic, it must be excluded during compilation.

Compile the project using either of the following methods:

**Method 1:**
```bash
gcc 1_lexical_main.c 2_token_analysis.c 3_error_analysis.c
```
**Method 2:**
```bash
gcc $(ls *.c | grep -v "input_c_file.c")
```
