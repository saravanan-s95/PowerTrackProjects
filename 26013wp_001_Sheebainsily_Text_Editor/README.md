# Text Editor in C

## Project Overview

The **Text Editor** is a console-based text editing application developed using the C programming language. It allows users to perform basic text editing operations through a command-line interface.

The project demonstrates the use of **doubly linked lists, stacks, dynamic memory allocation, file handling, and string operations**.

## Features

- **Insert Text:** Insert text into the editor.
- **Delete Text:** Delete text from the editor.
- **Undo and Redo:** Undo or redo supported editing operations.
- **Search:** Find specific text in the document.
- **Replace:** Replace existing text with new text.
- **Cursor Navigation:** Navigate through the document.
- **Copy, Cut, and Paste:** Perform clipboard operations.
- **File Operations:** Save and open text files.
- **Display Text:** Print the current document contents.

## Technologies Used

- Programming Language: C
- Compiler: GCC
- Operating System: Ubuntu Linux / WSL
- Editor: Visual Studio Code
- Build Tool: Make

## Data Structures Used

- **Doubly Linked List:** Stores and manages document lines.
- **Stacks:** Support undo and redo operations.
- **Dynamic Memory Allocation:** Manages memory for document contents and related data.

## Project Structure

```text
text_editor_project/
├── main.c
├── text_editor.h
├── editor_internal.h
├── text_ops.c
├── file_ops.c
├── display_search.c
├── history.c
├── Makefile
├── README.md
└── sample.txt
```

## Compilation

Open the terminal in the project directory and run:

```bash
make clean
make
```

Alternatively, compile directly using GCC:

```bash
gcc -std=c99 -Wall -Wextra -pedantic main.c text_ops.c file_ops.c display_search.c history.c -o text_editor
```

## Execution

Run the application using:

```bash
./text_editor
```

The editor displays the following prompt:

```text
Text Editor v1.0 - type 'help' for commands.

editor>
```

Enter commands at the `editor>` prompt to perform editing operations.

## Testing

The following operations can be tested:

1. Insert text and display the document.
2. Undo and redo editing operations.
3. Search for text.
4. Replace existing text.
5. Copy, cut, and paste text.
6. Save the document to a file.
7. Open a saved file and verify its contents.

Example commands:

```text
insert Hello
print
undo
redo
find Hello
replace Hello|Hi
print
save test.txt
open test.txt
exit
```

Run these commands individually at the `editor>` prompt.

## Learning Outcomes

- Improved understanding of C programming.
- Practical implementation of linked lists and stacks.
- Experience with dynamic memory allocation.
- Understanding of file handling and string manipulation.
- Practice with modular programming and multi-file compilation.
- Experience testing and debugging a C application.

## Author

**Sheeba Insily**

## Conclusion

This project demonstrates the implementation of a console-based text editor using C. It combines data structures, file handling, and modular programming to provide basic text editing functionality.
