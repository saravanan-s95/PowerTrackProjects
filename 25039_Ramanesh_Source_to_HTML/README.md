
# Source to HTML Converter

## Student Information

**Student Name:** Ramanesh M  
**Batch Number:** 25039  
**Project Title:** Source to HTML Converter  

---

## Project Description

The **Source to HTML Converter** is a C-based application developed to convert C source code files into HTML documents with syntax highlighting. The program performs lexical analysis by scanning the input source code and identifying different lexical tokens such as keywords, identifiers, operators, delimiters, numbers, strings, comments, and preprocessor directives. Each token is formatted with appropriate HTML tags and CSS styles to improve readability. The generated HTML file preserves the original code structure, indentation, and formatting, allowing users to view source code directly in a web browser with enhanced visualization.

---

## Objectives

- Convert C source code into HTML format.
- Perform lexical analysis of source code.
- Generate syntax-highlighted HTML output.
- Preserve code formatting and indentation.
- Demonstrate compiler design concepts through tokenization.

---

## Features

- Converts C source files to HTML.
- Syntax highlighting for keywords, identifiers, operators, numbers, strings, comments, and delimiters.
- Preserves indentation and whitespace.
- Browser-compatible HTML output.
- Lightweight and easy-to-use implementation.
- Supports Makefile-based compilation.

---

## Technologies Used

- C Programming
- HTML
- CSS
- GCC Compiler
- Make Utility (Makefile)
- Linux / WSL

---

## Software Requirements

- GCC Compiler
- Make Utility
- Linux / WSL or MinGW
- Visual Studio Code (Optional)
- Web Browser (Chrome, Firefox, Edge)

---

## Project Structure

```
Source_to_HTML/
│
├── main.c
├── lexer.c
├── keyword.c
├── html.c
├── utils.c
├── lexer.h
├── html.h
├── Makefile
├── README.md
└── sample.c
```

---

## Working Principle

1. The user provides a C source file as input.
2. The program reads the source file character by character.
3. The lexical analyzer classifies tokens such as keywords, identifiers, operators, numbers, strings, comments, and delimiters.
4. Each token is wrapped with corresponding HTML formatting.
5. The formatted content is written into an HTML file.
6. The generated HTML file is opened in any web browser to display syntax-highlighted source code.

---

## Compilation

Compile the project using the Makefile:

```bash
make
```

This generates the executable:

```text
source_to_html
```

---

## Running the Project

Execute the program:

```bash
./source_to_html sample.c
```

or

```bash
./source_to_html input.c output.html
```

(depending on the implementation).

---

## Cleaning the Project

Remove object files and executable:

```bash
make clean
```

---

## Input

A C source file.

Example:

```c
#include <stdio.h>

int main()
{
    printf("Hello World");
    return 0;
}
```

---

## Output

The program generates an HTML file containing syntax-highlighted source code while preserving the original formatting and indentation. The output file can be viewed in any modern web browser.

---

## Advantages

- Improves source code readability.
- Generates portable HTML output.
- Preserves original code formatting.
- Demonstrates lexical analysis concepts.
- Easy to extend for additional programming languages.

---

## Applications

- Compiler Design Laboratory
- Source Code Documentation
- Online Code Viewers
- Programming Tutorials
- Educational Tools
- Static Code Visualization

---

## Future Enhancements

- Support for multiple programming languages.
- Dark and light syntax themes.
- Line numbering.
- Search and highlighting functionality.
- PDF export support.
- Interactive HTML output with collapsible code blocks.

---

## Conclusion

The **Source to HTML Converter** project demonstrates the practical application of lexical analysis and file processing by converting C source code into an HTML document with syntax highlighting. By recognizing different lexical tokens and applying HTML formatting, the project enhances the readability and presentation of source code. It serves as an effective educational tool for understanding compiler design concepts while providing a useful utility for code documentation, visualization, and web-based sharing.
