# Source to HTML Converter

## Project Overview

The Source to HTML Converter is a C-based application that converts C source code files into HTML format with syntax highlighting. The generated HTML file preserves the structure of the source code and displays different programming elements such as keywords, comments, strings, preprocessor directives, and numeric constants using distinct styles for better readability.

## Features

* Converts C source files into HTML documents.
* Preserves source code formatting and indentation.
* Highlights:

  * Keywords
  * Comments
  * Strings
  * Preprocessor directives
  * Numeric constants
  * Header files
* Generates HTML output that can be viewed in any web browser.
* Uses CSS styling for improved presentation.

## Project Structure

```text
s2html_main.c      - Main program
s2html_conv.c      - Source-to-HTML conversion functions
s2html_conv.h      - Function declarations for conversion module
s2html_event.c     - Event handling and parsing logic
s2html_event.h     - Header file for event module
styles.css         - CSS styles for syntax highlighting
test.c             - Sample input C file
test.c.html        - Generated HTML output
Source2html_doc.pdf - Project documentation
```

## Requirements

* GCC Compiler
* Linux/WSL Environment
* Standard C Library

## Compilation

```bash
gcc s2html_main.c s2html_conv.c s2html_event.c -o s2html
```

## Execution

```bash
./s2html test.c
```

## Sample Output

```text
Output file test.c.html generated
```

The generated HTML file can be opened in any web browser to view the syntax-highlighted source code.

## Applications

* Source code documentation
* Educational tools
* Code visualization
* Static code presentation on web pages

## Author

Varsha S

## Batch Number

25034
