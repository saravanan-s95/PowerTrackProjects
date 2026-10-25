# Source2Html (s2html)

A C program that converts a `.c` source file into a syntax-highlighted HTML page. Feed it a source file, and it produces an HTML file you can open in any browser to view your code with color-coded comments, keywords, strings, numbers, and preprocessor directives — just like a code editor, but as a static webpage.

## How It Works

The parser is built as a **state machine** (`s2html_event.c`). It reads the source file one character at a time and moves between states like `IDLE`, `STRING`, `SINGLE_LINE_COMMENT`, `PREPROCESSOR_DIRECTIVE`, `RESERVE_KEYWORD`, etc. As it enters and leaves each state, it buffers the relevant characters and emits an **event** (e.g. `PEVENT_STRING`, `PEVENT_COMMENT`) once that chunk of text is complete.

Each event is then handed to `source_to_html()` (`s2html_conv.c`), which wraps the text in the right `<span>` tag with a CSS class, so the browser renders it in color.

**Flow:** `main.c` → reads file → `get_parser_event()` (state machine) → returns one event at a time → `source_to_html()` writes the matching HTML → repeat until EOF → write closing HTML tags.

### Color Scheme (`styles.css`)

| Element                  | Color      |
|---------------------------|------------|
| Comments                  | blue       |
| Reserved keywords (data types) | green |
| Reserved keywords (others)| goldenrod  |
| Preprocessor directives   | purple     |
| Header files               | red        |
| Strings                   | magenta    |
| Numeric constants          | brown      |
| ASCII characters            | firebrick  |

## Project Structure

s2html_main.c - Program entry point — opens files, drives the parse loop
s2html_event.c/.h - The state machine: reads chars, tracks state, emits events
s2html_conv.c/.h - Converts events into HTML spans; writes HTML header/footer
styles.css - Color scheme for the generated HTML
test.c - Sample C file to test the converter on
test_c.html - Sample output for test.c


## Building

```bash
gcc s2html_main.c
```

(`s2html_main.c` directly `#include`s `s2html_event.c` and `s2html_conv.c`, so no separate object files or Makefile are needed.)

## Running

This generates `<source_file.c>.html` in the same directory. Open it in any browser to view the highlighted code.

**Example:**

```bash
./a.out test.c
```

This produces `test.c.html`.
