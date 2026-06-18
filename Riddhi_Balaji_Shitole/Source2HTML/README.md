# Source to HTML Converter
Overview
The Source to HTML Converter is a C-based application that converts C source code files into HTML format with syntax highlighting. 
The generated HTML file improves readability by displaying different programming elements such as keywords, comments, strings, header files, numeric constants, 
and preprocessor directives in different styles using CSS.

# Features
Converts C source files into HTML documents.

# Syntax highlighting for:

Reserved keywords
Preprocessor directives
Header files
Comments (Single-line and Multi-line)
String literals
Character constants
Numeric constants
Generates browser-viewable HTML output.
Uses CSS styling for better code presentation.
Supports user-defined and standard header files.

# Project Structure
Source-to-HTML/
│
├── s2html_main.c        # Main driver program
├── s2html_conv.c        # HTML conversion functions
├── s2html_conv.h        # Conversion function declarations
├── s2html_event.c       # Parser and event handling
├── s2html_event.h       # Event definitions
├── styles.css           # HTML styling file
├── test.c              # Sample input file
├── test.c.html         # Generated HTML output

# Working Principle
->The project follows an event-driven parsing approach:
->Reads the source file character by character.
->Identifies programming constructs using a parser.
->Generates parser events such as:
      Preprocessor Directive
      Reserved Keyword
      String Literal
      Header File
      Numeric Constant
      Comments
      Character Constants
      Converts identified elements into corresponding HTML tags.
      Applies CSS classes for syntax highlighting.
      Produces a formatted HTML file.

# Supported Syntax Elements
Element	Example
Preprocessor Directive	#include, #define
Header Files	<stdio.h>, "myheader.h"
Keywords	int, char, if, else, while
Strings	"Hello World"
Comments	// comment, /* comment */
Numeric Constants	10, 3.14
Character Constants	'A'

# Compilation
Use GCC compiler:

gcc s2html_main.c s2html_conv.c s2html_event.c -o s2html
   Usage
# Syntax
./s2html <source_file>
# Example
./s2html test.c
# Output:
test.c.html
Custom Output Name
./s2html test.c output
Generated file:
 output.html
