# Source To HTML Converter

## Description
This project converts a C source file into an HTML file with syntax highlighting.

## Features
- Keywords highlighting
- Preprocessor directives highlighting
- Numbers highlighting
- String literals highlighting
- Character constants highlighting
- Single-line comments highlighting
- Multi-line comments highlighting

## Files

```
main.c
parser.c
parser.h
html.c
html.h
keyword.c
keyword.h
test.c
```

## Compilation

```bash
gcc main.c parser.c keyword.c html.c
```

## Execution

```bash
./a.out test.c
```

## Output

```
test.c.html
```
