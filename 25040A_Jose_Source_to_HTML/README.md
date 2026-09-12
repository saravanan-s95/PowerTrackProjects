# Source-to-HTML Converter

A modular command-line tool written in C that reads C source code and generates a syntax-highlighted HTML file. 
It includes a custom lexical analyzer to tokenize keywords, constants, strings, comments, and preprocessor directives, applying distinct HTML styling to each.

## Features
* **Syntax Highlighting:** Distinct colors for data keywords, non-data keywords, constants, string literals, and comments.
* **Line Numbering Mode:** Use the `-n` flag to automatically generate sequential line numbers alongside the code.
* **Smart File Validation:** Verifies `.c` input and `.html` output extensions, gracefully falling back to a default output file if none is provided.

## Compilation
To compile the converter, you must compile the core project files while intentionally excluding the target file you intend to parse (e.g., `input.c`). 

Run the following command in your terminal to link the specific source files:
```bash
gcc 1_main.c 2_token_functions.c 3_line_number_fn.c 4_html_functions.c 5_validations.c -o c2html
