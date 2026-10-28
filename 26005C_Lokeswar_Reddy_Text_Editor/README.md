# Text Editor in C using DSA 

A command-line text editor developed in **C** to explore the practical use of data structures, dynamic memory allocation, file handling, and modular programming.

The editor stores text line by line, tracks a cursor position, and provides common editing operations through a simple command-based interface.

## Features

- **Text editing:** Insert text and add new lines.
- **Delete operations:** Delete characters before the cursor or delete the current line.
- **Cursor navigation:** Move up, down, left, and right; jump to the beginning or end of a file or line.
- **Clipboard operations:** Copy, cut, and paste text.
- **Undo and redo:** Reverse and reapply supported editing operations.
- **File handling:** Open, save, and close text files.
- **Find and replace:** Search for text and replace matching text.
- **Display information:** Print the current text and view the cursor position.
- **Command help:** Display the available commands from inside the editor.

## Technologies Used

- **Language:** C
- **Concepts:** Data Structures and Algorithms, pointers, dynamic memory allocation, string handling, modular programming, and file I/O
- **Data structures:** Doubly linked list and dynamically resizable array-based stacks
- **Tools:** GCC compiler and terminal

## Data Structures Used

### 1. Doubly Linked List — Text Buffer

Each node stores a line of text and pointers to the previous and next nodes. The editor maintains references to the head, tail, and current cursor node.

This structure allows the editor to navigate between lines and insert or remove lines by updating node links.

### 2. Dynamic Array-Based Stacks — Undo and Redo

Two stacks maintain the history of editing actions:

- **Undo stack:** Stores actions that can be reversed.
- **Redo stack:** Stores actions that can be reapplied.

Each action records information such as the operation, text, line number, and cursor position, as needed to restore an earlier state.

## Project Structure

```text
DSA_4_Text_Editor/
├── 1.main.c                       # Program entry point and command loop
├── 2.Editor_functions.c            # Editor initialization and helper functions
├── 3.text_editor.c                 # Command handling and command help
├── 4.text_operation_functions.c    # Text editing, search, and replace operations
├── 5.file_ops.c                    # File operations
├── 6.cursor_nav.c                  # Cursor navigation
├── 8.redo.c                        # Redo functionality
├── 8.undo.c                        # Undo functionality
├── 9.stack_operations.c            # Stack operations for action history
└── header.h                        # Structures, constants, and function declarations
```

## Requirements

- GCC or another compatible C compiler
- A terminal environment such as Linux, macOS, or WSL

## Build and Run

Clone the repository:

```bash
git clone https://github.com/pathakuntalokeswarreddy/DSA_4_Text_Editor.git
cd DSA_4_Text_Editor
```

Compile the source files:

```bash
gcc -Wall -Wextra 1.main.c 2.Editor_functions.c 3.text_editor.c \
    4.text_operation_functions.c 5.file_ops.c 6.cursor_nav.c \
    8.redo.c 8.undo.c 9.stack_operations.c -o text_editor
```

Run the program:

```bash
./text_editor
```

> **Note:** The editor uses the `clear` terminal command when it starts, so a terminal environment is recommended.

## Available Commands

Type `help` inside the editor to display the command menu.

| Command | Description |
|---|---|
| `insert <text>` | Insert text at the current cursor position |
| `insert_new_line <text>` | Add a new line containing the supplied text |
| `delete <number>` | Delete the specified number of characters before the cursor |
| `delete_line` | Delete the current line |
| `up` | Move the cursor up |
| `down` | Move the cursor down |
| `left` | Move the cursor left |
| `right` | Move the cursor right |
| `start` | Move to the start of the file |
| `end` | Move to the end of the file |
| `line_start` | Move to the start of the current line |
| `line_end` | Move to the end of the current line |
| `open <filename>` | Open a text file |
| `save <filename>` | Save the text to a file |
| `close` | Close the current document in the editor |
| `find <text>` | Search for text |
| `replace <old> <new>` | Replace matching text |
| `copy` | Copy text using the editor's copy operation |
| `cut` | Cut text using the editor's cut operation |
| `paste` | Paste copied or cut text |
| `undo` | Undo a supported editing operation |
| `redo` | Redo a supported editing operation |
| `print` | Display the text buffer |
| `position` | Display the cursor position |
| `help` | Show available commands |
| `exit` | Exit the editor |

### Example Session

```text
help
insert Hello, world!
print
save notes.txt
exit
```

The commands are entered one at a time. Use `help` in the running program to review the supported operations.

## Learning Outcomes

This project provides practical experience with:

- Implementing a text buffer using a doubly linked list
- Managing pointers and dynamically allocated memory
- Using stacks to maintain undo and redo history
- Handling text files with C file I/O
- Splitting a program into multiple source files with a shared header
- Processing commands and validating user input
- Maintaining cursor position while modifying text

## Future Improvements

Possible extensions include:

- Improve input validation and error handling
- Add automated tests for editing and history operations
- Support more advanced search and replace options
- Improve handling of spaces in file names

## Author

**Lokeswar Reddy Pathakunta**

- GitHub: [@pathakuntalokeswarreddy](https://github.com/pathakuntalokeswarreddy)
- Repository: [DSA_4_Text_Editor](https://github.com/pathakuntalokeswarreddy/DSA_4_Text_Editor)

---

If you find this project useful for learning C and data structures, feel free to explore the source code.
