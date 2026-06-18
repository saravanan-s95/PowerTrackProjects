# Source to HTML Converter (S2HTML)

## What Is This Project?

This project is a Source to HTML Converter developed in C. It reads a source code file and converts it into an HTML document while preserving the code structure and formatting.

The generated HTML file can be viewed in any web browser, making source code easier to read and share. The converter handles various source code elements and transforms them into a properly formatted HTML representation.

## Uses Of The Project

This project is useful for understanding file processing, parsing, and HTML generation in C programming.

Main uses include:

* Converting source code files into HTML format
* Displaying source code in web browsers
* Learning file handling in C
* Understanding parser-based design
* Practicing event-driven programming concepts
* Generating readable and shareable code documentation

## Features

* Reads source code files as input
* Generates corresponding HTML output
* Preserves source code formatting
* Handles comments, strings, and code structure
* Creates browser-friendly HTML files
* Modular implementation using multiple source files

## Project Files

* `s2html_main.c` : Main program and execution flow.
* `s2html_conv.c` : Source-to-HTML conversion functions.
* `s2html_event.c` : Event handling and parser-related functions.
* `README.md` : Project documentation.

## Requirements

You need a C compiler installed, such as GCC.

Check GCC installation:

```bash
gcc --version
```

## How To Compile

Compile the project using:

```bash
gcc s2html_main.c s2html_conv.c s2html_event.c -o s2html
```

Open the generated HTML file in a web browser to view the formatted source code.

## Learning Outcomes

This project helps in understanding:

* File handling in C
* String processing
* Parser and event-driven design
* HTML generation techniques
* Modular programming concepts
* Software project organization

## Author

**Name:** Riddhi Balaji Shitole
**ID:** 25034f_036

## Notes

* The input file should be a valid source code file.
* The generated HTML file can be opened in any modern web browser.
* This project is intended for educational and learning purposes.
