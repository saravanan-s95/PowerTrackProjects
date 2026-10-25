# Text Editor

A command-line text editor built from scratch in C.

The editor stores text as a **doubly linked list of lines**, where each line is a fixed-size character buffer. It supports inserting, deleting, copying, cutting, pasting, cursor navigation, undo/redo, and loading/saving `.txt` files.

## Features

- **Insert** text at the current cursor position or as a new line
- **Delete** characters (left/right of cursor) or delete an entire line
- **Cursor navigation** — up, down, left, right, start/end of line, start/end of document
- **Copy / Cut / Paste** — by character count or entire line
- **Undo / Redo** — backed by a dynamic array stack of logged actions
- **Open / Save** `.txt` files

## Project Structure

```
main.c                  Program entry point, command loop, menu dispatch
text_editor.c/.h        Core editor: insert, delete, copy/cut/paste, print
cursor_navigations.c/.h Cursor movement logic
undo_redo.c/.h          Undo/redo stack and action logging
file_ops.c/.h           Opening/loading and saving .txt files
makefile                Build script
default.txt             Sample file to test the "open file" command
```

### Core data structures

- **`Node`** — one line of text (`char line[MAX_CHARS]`) plus `prev`/`next` pointers, forming a doubly linked list.
- **`TextEditor`** — holds `head`, `tail`, `current` (the line the cursor is on), `cursorLine`, and `cursorPos`.
- **`Action`** — one undo/redo log entry: operation type, affected text, and cursor position at the time.
- **`DynamicArrayStack`** — a simple array-backed stack of `Action`s used for both the undo stack and the redo stack.

## Building and Running

**Requirements:** `gcc` and `make` (Linux/macOS/WSL — any standard C toolchain works).

# Build
make

# Run
./a.out
```

To clean build artifacts:

```bash
make clean
```

## Usage

On launch, the editor prints a numbered menu and then repeatedly prompts `Enter the command :`. Commands are typed by name (not by number):

```
insert         - insert text (prompts: at current cursor position / at new line)
delete         - delete text (prompts: no. of characters / entire line)
move cursor    - open the cursor navigation sub-menu
copy           - copy characters or the current line
cut            - cut characters or the current line
paste          - paste clipboard contents at cursor or as a new line
undo / redo    - undo or redo the last edit
print          - print the full document with cursor position
open file      - load a .txt file into the editor (once per session)
save file      - save the current document to a .txt file
exit           - save (optional) and quit
```

**Example session:**

```
Enter the command : open file
Enter the filename to be opened/saved : default.txt
File opened successfully

Enter the command : print
Line 1: Hello World
Line 2: This is a new line
Line 3: This is line 3  bye hi|
Cursor position: Line 3, Column 23

Enter the command : insert Hello
Enter your choice : 1
Text inserted successfully in current line

Enter the command : undo
Undo successful

Enter the command : save file
Enter the filename to be opened/saved : output.txt
File saved successfully

Enter the command : exit
```

A sample file, `default.txt`, is included so the "open file" command can be tested immediately.

## Notes

- `MAX_CHARS` (line buffer size) and `STACK_CAPACITY` (undo/redo depth) are defined in `text_editor.h` / `undo_redo.h` and can be adjusted there.
- Undo/redo only tracks insert and delete operations, per project scope — copy/paste ultimately re-use `insertText`/`insertTextnewline`, so they are covered indirectly.
- A file can currently be opened only once per session, into an empty editor.
