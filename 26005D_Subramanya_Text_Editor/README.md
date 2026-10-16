# Text Editor (C)

A console-based text editor written in C, built as a DSA course project. Implements text editing, cursor navigation, multi-level undo/redo, and file I/O — using a doubly linked list for line storage and a dynamic array-based stack for undo/redo history.

## Features

- Insert and delete text at the cursor position
- Multi-level undo/redo (stack-based)
- Cursor navigation: up, down, left, right, start of line, end of line, goto specific line
- Open, save, and close files
- Auto-refreshing, color-coded terminal display with a highlighted cursor position

## Data Structures

- **Doubly Linked List** — each line of text is a `Node`, linked to the line before and after it. Chosen because text editing involves frequent inserts/deletes, often mid-document — once the correct line is located, insertion/deletion is O(1) pointer rewiring, versus O(n) shifting in a plain array.
- **Dynamic Array Stack** — undo/redo history is stored as a resizable array of `Action` records, doubling in capacity when full, giving amortized O(1) push/pop.

## Project Structure

.
├── src/ # all .c source files
├── include/ # all .h header files
├── obj/ # build artifacts (generated, not tracked)
├── Makefile
├── test.txt
└── README.md


| File | Responsibility |
|---|---|
| `text_editor.c/h` | `insertText` | `deleteCharacters` | structs & helpers (`Node`, `TextEditor`, `getNodeAtLine`) |
| `text_ops.c/h` | `copyText`, `cutText`, `pasteText` |
| `cursor_navigations.c/h` | Cursor movement: up/down/left/right/start/end/goto |
| `redo_undo.c/h` | `push_st`, `pop_st`, `undo`, `redo`, `Action`/`DynamicArrayStack` |
| `file_ops.c` | `openFile`, `saveFile`, `closeFile`, `createNode` |
| `main.c` | Command-line loop, parsing, and dispatch |

## Build & Run

```bash
make
./texteditor
```

Run `make clean` to remove build artifacts, or `make rebuild` for a fresh build.

## Commands

| Command | Description |
|---|---|
| `insert <text>` | Insert text at the current cursor position |
| `delete <n>` | Delete `n` characters before the cursor |
| `undo` | Undo the last operation |
| `redo` | Redo the last undone operation |
| `up` / `down` / `left` / `right` | Move the cursor |
| `start` / `end` | Jump to start/end of the current line |
| `goto <line>` | Jump cursor to a specific line number |
| `print` | Refresh the display (also happens automatically after every command) |
| `save <filename>` | Save the current document to a file |
| `exit` | Quit the editor |

## Design Notes

- **Cursor position updates automatically** after every insert/delete/undo/redo — matching how most real editors keep the cursor at the point of the most recent change.
- **Undo/redo use the same `Action` struct** for both directions: undo reverses the stored action (insert↔delete), redo replays it exactly as recorded. A new edit after an undo clears the redo stack, since that history no longer applies to the changed document.
- **Error handling** uses a `Status_e` (`SUCCESS`/`FAILURE`) return convention across core functions, so the command loop can skip the auto-refresh on failure and keep the error message visible instead of clearing it.

## Known Limitations / Possible Extensions

- No support yet for creating or deleting whole lines (e.g., pressing Enter mid-line to split it, or Backspace merging two lines) — would require extending `Action` with a grouped-operation identifier (`groupId`) so undo/redo can treat a multi-node change as one atomic step.
- Assumes non-empty input files.
- Search and replace not yet implemented.
- Command input is line-buffered (`fgets`), not raw single-keypress terminal input — real-time key-by-key response (e.g., WASD-style navigation without pressing Enter) would require reconfiguring terminal mode via `termios`.