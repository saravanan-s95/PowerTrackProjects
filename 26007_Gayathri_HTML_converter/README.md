

### AIM:
To develop a C program that converts a C source file into a syntax-highlighted HTML file.

### DESCRIPTION:
This project reads a C program (input.c) and generates an HTML file (output_colour.html) with colour-coded syntax highlighting for better readability. It is useful for presenting code on web pages.

### How it Works:
1. The program reads the `input.c` file character by character.
2. It identifies C keywords (like int, float, if, else, for, while), comments (//, /* */), strings (" "), and header files.
3. It wraps these tokens with HTML `<span>` tags with different colours.
   - Keywords -> Blue
   - Comments -> Green
   - Strings -> Red
   - Header files -> orange
4. Finally, it writes the coloured output into `output_colour.html`.

