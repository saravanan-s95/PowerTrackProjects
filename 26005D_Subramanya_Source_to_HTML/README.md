# Source-to-HTML

Converts a C source file into a syntax-highlighted HTML page that can be opened in any browser.
It reuses the tokenizer from my Lexical Analyzer project: each token is wrapped in a coloured
`<span>`, and spaces, tabs and newlines are kept so the page looks like the original file.

## Build and run

```
make                                   # build ./build/lexer
./build/lexer input.c                  # writes input.c.html
./build/lexer input.c out.html         # choose the output name
./build/lexer input.c out.html -n      # add line numbers
./build/lexer input.c -n               # line numbers, default output name
make test                              # run every file in tests/ in both modes
make clean                             # remove build/
```

Rules: the input must be a `.c` file, the output (if given) must be a `.html` file,
and `-n` can appear anywhere after the input file.

`make test` runs every `.c` file in `tests/` with and without `-n`, and checks that the
number of `<span>` tags equals the number of `</span>` tags in each output.
It does not check colours, so open `build/sample.c.n.html` in a browser to check those by eye.

## Colours

| Token                                               | Colour    |
|-----------------------------------------------------|-----------|
| Comments (`//` and `/* */`)                         | blue      |
| Type keywords (`int`, `char`, `void`..)             | green     |
| Control keywords (`if`, `for`, `return`..)          | goldenrod |
| Preprocessor directives (`#include`, `#define`..)   | purple    |
| Header files (`<stdio.h>`, `"my.h"`)                | red       |
| Strings                                             | magenta   |
| Numbers                                             | brown     |
| Character literals                                  | firebrick |
| Everything else (identifiers, operators, symbols)   | default   |

## How it works

1. `get_token()` reads the input file and returns one token per call (keyword, number, string,
   comment, operator, whitespace and so on). It returns `FAILURE` only at end of file.
2. Whitespace is not skipped. Each space, tab and newline is returned as its own token so the
   layout is preserved inside `<pre>`.
3. After `#include`, a flag makes the next `<...>` or `"..."` be read as one header token
   instead of five separate pieces.
4. `main()` writes the HTML header, loops over the tokens until end of file, then writes the footer.
5. `print_html_token()` looks up the CSS class for the token type, prints
   `<span class="...">`, writes the text, then prints `</span>`. `<`, `>` and `&` are escaped
   as `&lt;`, `&gt;` and `&amp;` so the browser does not treat them as HTML.
6. With `-n`, the printer remembers whether it is at the start of a line and prints the line
   number before the first character of each line, including lines inside multi-line comments.

## Layout

```
inc/lexer.h      token types, structures, function declarations
src/main.c       argument checks, file opening, main loop
src/lexer.c      get_token(): decides what each token is
src/readers.c    readers for words, numbers, strings, comments, operators, directives, headers
src/tables.c     keyword, directive and operator tables
src/print.c      HTML output: page header/footer, coloured tokens, escaping, line numbers
tests/           sample.c (Sample code), broken.c (unterminated header), full_test.c (Shows limitations of code)
Makefile         build, test, clean
```

## Known limitations

- Numbers with a prefix or suffix (`0xFF`, `25.5f`, `10UL`, `1.2e-3`) are split into several
  tokens, so only the plain digits are coloured.
- Type names such as `size_t` and `pid_t`, and macros such as `NULL`, are not coloured, because
  only the keywords in the tables are.
- A single token longer than `MAX_LEXEME` (set in `inc/lexer.h`) is cut off.
- The page background is light grey. There is no dark theme.