# Text Editor

A console-based text editor in C which allows basic text editor functionalities like insertion,
deletion, undo, redo and navigation which uses doubly linked list where each node stores a single line and makes use of stack to implement undo-redo functionalities.
## Features

* insert data/newline
* delete characters/entire line
* navigation - up, down, left, right, start of file, end of file, start of line and end of line
* undo the changes made to file
* redo the changes made by undo
* display contents of file, cursor position and line number where the cursor is currently in
* load from file into doubly linked list
* save to file from doubly linked list
* optional save to file before exiting

## Build

```bash
make
```

## Usage
### Without input file
```bash
./txt_editor.out 
```
or 
### With input file
```bash
./txt_editor.out <filename.txt>
```

## Commands
```
insert <text>     - insert text at cursor
insert            - insert new blank line
delete <n>        - delete n characters from cursor
delete            - delete entire current line
navigation        - enter navigation mode
undo / redo
print             - display document + cursor
save              - save to file
exit              - exit (prompts to save if unsaved changes)
```
## Navigation commands
```
up                - move cursor up
down              - move cursor down
left              - move cursor left
right             - move cursor right
home              - move cursor to the start of line
end               - move cursor to the end of line
sof               - (start of file)move cursor to the start of file
eof               - (end of file)move cursor to the end of file
exit              - exit the navigation
```
## Sample output
```
Enter command: insert hello world
Enter command: print
Line:1 hello world|
Cursor position: Line 1, Column 11
```