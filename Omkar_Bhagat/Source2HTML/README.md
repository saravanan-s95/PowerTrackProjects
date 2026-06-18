# Source To HTML Converter

## Overview

Source To HTML Converter is a C-based utility that converts C source code files into HTML format with syntax highlighting. The generated HTML file improves readability by displaying different programming constructs in different colors.

## Features

* Converts C source files into HTML format
* Highlights preprocessor directives
* Highlights header files
* Highlights reserved keywords
* Highlights numeric constants
* Highlights string literals
* Highlights character constants
* Highlights single-line comments
* Highlights multi-line comments
* Preserves source code formatting

## Project Structure

```text
s2html_main.c      - Main driver program
s2html_event.c     - Parser implementation
s2html_event.h     - Parser definitions
s2html_conv.c      - HTML conversion functions
s2html_conv.h      - HTML conversion definitions
styles.css         - CSS styles for syntax highlighting
tags               - Reserved keyword information
```

## Compilation

```bash
gcc s2html_main.c s2html_event.c s2html_conv.c -o s2html
```

## Execution

Generate HTML file using:

```bash
./s2html test.c
```

Output:

```text
test.c.html
```
## How to Open the Generated HTML File
### Windows (WSL)

```bash
explorer.exe test.c.html
```


