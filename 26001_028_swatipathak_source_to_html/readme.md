# Source to HTML Converter

A C-based utility that converts C source code into a syntax-highlighted
HTML document. The project analyzes source code, identifies supported
language elements, and generates browser-readable HTML while preserving
source-code readability.

## Project Overview

The program reads a C source file, identifies different language
constructs, and generates an HTML file with syntax highlighting.

### Processing Flow

``` text
C Source File
     |
     v
Read Source Code
     |
     v
Analyze / Identify Tokens
     |
     +----------------------+
     |                      |
     v                      v
Keywords / Identifiers   Comments / Strings
     |                      |
     +----------+-----------+
                |
                v
        Generate HTML
                |
                v
       Syntax Highlighted
          HTML File
```

## Features

-   Converts C source code into HTML.
-   Syntax highlighting for supported C language elements.
-   Identifies keywords and identifiers.
-   Handles operators and symbols.
-   Handles comments.
-   Handles string literals.
-   Handles preprocessor directives.
-   Preserves source-code structure and readability.
-   Generates an HTML file that can be opened in a web browser.

## Technologies Used

-   **C Programming**
-   **File Handling**
-   **String Processing**
-   **Lexical Analysis / Tokenization**
-   **HTML Generation**
-   **Pointers and Character Arrays**

## Source Processing

The source file is processed by reading its contents and identifying
different types of tokens.

Typical constructs handled by the converter include:

``` text
Keywords
Identifiers
Operators
Symbols
Comments
String Literals
Preprocessor Directives
```

These elements are converted into appropriate HTML output so different
parts of the source code can be visually distinguished.

## Key Challenges

-   Identifying different C language constructs while processing the
    source file.
-   Handling comments and string literals without incorrectly
    interpreting their contents as C tokens.
-   Generating valid HTML while preserving the original source-code
    structure.
-   Handling special characters that have a different meaning in HTML.
-   Managing file operations and processing source-code text
    efficiently.

## Learning Outcomes

This project strengthened my understanding of:

-   File handling in C.
-   String manipulation and character processing.
-   Lexical analysis and tokenization.
-   Pointers and arrays.
-   Parsing source code.
-   HTML generation.
-   Handling edge cases while processing text.

## Build and Run

Compile using GCC:

``` bash
gcc *.c -o source_to_html
```

Run:

``` bash
./source_to_html abc.c output.html
```

## Project Objective

The objective of this project was to develop a C-based source-code
processing utility and gain practical experience with **file handling,
lexical analysis, tokenization, string processing, and HTML
generation**.

## Author

**Swati Pathak**

