# 📝 Console Text Editor in C

> A feature-rich terminal-based text editor developed entirely in **C**,
> demonstrating core systems programming concepts such as dynamic memory
> management, linked lists, stacks, file handling, and modular software
> design.

------------------------------------------------------------------------

## 🚀 Features

### 📄 File Operations

-   Open existing text files
-   Save edited files
-   Exit with save confirmation

### ✍️ Text Editing

-   Character insertion
-   Backspace & Delete
-   Insert new line
-   Line split and merge
-   Cursor navigation using arrow keys

### 🔎 Search

-   Search text
-   Search Next
-   Wrap-around search

### 📋 Clipboard

-   Copy line
-   Cut line
-   Paste line
-   Text selection
-   Copy selected text
-   Cut selected text
-   Paste selected text

### ↩️ Undo / Redo

-   Character insert/delete
-   Line split/merge
-   Selection cut/paste

### 🖥️ UI

-   Status bar
-   Cursor position
-   File modification status

------------------------------------------------------------------------

# 🧠 Data Structures

  Data Structure       Purpose
  -------------------- ------------------------------
  Doubly Linked List   Stores document line by line
  Stack                Undo / Redo history
  Clipboard            Stores copied or cut text
  Selection            Tracks selected text range

------------------------------------------------------------------------

# 📁 Project Structure

``` text
Text_Editor/
│
├── include/
│   └── header.h
│
├── src/
│   ├── main.c
│   ├── display.c
│   ├── cursor.c
│   ├── insert.c
│   ├── delete.c
│   ├── search.c
│   ├── selection.c
│   ├── clipboard.c
│   ├── undo_redo.c
│   ├── file.c
│   └── stack.c
│
├── sample/
│   └── sample.txt
│
├── README.md
├── Makefile
└── .gitignore
```

------------------------------------------------------------------------

# ⌨️ Keyboard Shortcuts

  Shortcut   Action
  ---------- -----------------------
  ← ↑ ↓ →    Move Cursor
  Ctrl + S   Save File
  Ctrl + F   Search
  Ctrl + T   Start / End Selection
  Ctrl + K   Copy
  Ctrl + U   Cut
  Ctrl + P   Paste
  Ctrl + Z   Undo
  Ctrl + Y   Redo
  Esc        Exit / Cancel

------------------------------------------------------------------------

# ⚙️ Build

``` bash
gcc src/*.c -Iinclude -o editor
```

### Run

Linux/macOS

``` bash
./editor
```

Windows (MinGW)

``` bash
editor.exe
```

------------------------------------------------------------------------

# 📚 Concepts Demonstrated

-   Dynamic Memory Allocation
-   Doubly Linked Lists
-   Stack Implementation
-   File Handling
-   String Manipulation
-   Modular Programming
-   State Management
-   Console Application Development

------------------------------------------------------------------------

# 💡 Future Improvements

-   Syntax highlighting
-   Replace / Replace All
-   Mouse support
-   Configurable themes
-   Cross-platform terminal support

------------------------------------------------------------------------

# 📸 Screenshots

Add screenshots or a GIF here after completing the project.

Example:

``` text
docs/editor_demo.gif
docs/home.png
```

------------------------------------------------------------------------

# 👩‍💻 Author

**Swati Pathak**

Bachelor of Engineering (Electronics & Communication)

Interested in: - Embedded Systems - Embedded C - STM32 - ESP32 - VLSI -
Firmware Development

------------------------------------------------------------------------

# ⭐ If you found this project useful

Please consider giving the repository a ⭐ on GitHub!
