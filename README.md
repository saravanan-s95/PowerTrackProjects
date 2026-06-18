# Lexical Analysis Project

## What Is This Project?

This project is a simple lexical analyzer written in C. It reads a C source file and breaks the program into tokens such as keywords, identifiers, operators, delimiters, string literals, character literals, and numeric literals.

The analyzer also reports some common syntax-related warnings and errors, including:

- Invalid file extension
- Invalid identifiers
- Invalid numeric literals
- Unterminated string literals
- Unterminated character literals
- Unmatched braces, parentheses, and brackets
- Double semicolons
- Missing semicolons
- Misplaced semicolons after function declarations or control statements

## Uses Of The Project

This project is useful for learning the first phase of compiler design: lexical analysis. It can help students understand how source code is scanned and divided into meaningful tokens before parsing.

Main uses include:

- Demonstrating lexical analysis in compiler design
- Identifying tokens in a C program
- Checking simple lexical and syntax-style errors
- Practicing C file handling and string processing
- Understanding keywords, identifiers, literals, operators, and delimiters

## Project Files

- `main.c`: Main lexical analyzer program.
- `test.c`: Sample C input file used for testing.
- `a.out`: Existing compiled output file.

## Requirements

You need a C compiler installed, such as GCC.

Check GCC installation:

```bash
gcc --version
```

## How To Run The Project

1. Open the project folder:

```bash
cd /home/riddhi/lexical_analysis
```

2. Compile the lexical analyzer:

```bash
gcc main.c -o lexical_analyzer
```

3. Run the analyzer with a C source file:

```bash
./lexical_analyzer test.c
```

You can also analyze another C file:

```bash
./lexical_analyzer your_file.c
```

## Example Output

The program prints a table with the line number, token, and token type.

Example:

```text
Lexical analysis of: test.c

Line   Token                          Type
------ ------------------------------ -------------------------
1      #include<stdio.h>              Preprocessor Directive
2      int                            Keyword
2      main                           Identifier
```

## Notes

- The input file must have a `.c` extension.
- The analyzer is designed for basic C programs and educational use.
- It performs lexical token classification and also includes simple error and warning checks.
