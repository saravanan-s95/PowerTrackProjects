#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stddef.h>

typedef struct Line {
    char *text;
    struct Line *prev;
    struct Line *next;
} Line;

typedef struct {
    Line *head;
    Line *tail;
    Line *cursor;
    size_t cursor_line;
    size_t cursor_col;
    char *clipboard;
    char *filename;
} TextEditor;

void editor_init(TextEditor *editor);
void editor_destroy(TextEditor *editor);
int editor_insert(TextEditor *editor, const char *text);
int editor_delete(TextEditor *editor, size_t count);
int editor_newline(TextEditor *editor);
void editor_move_left(TextEditor *editor);
void editor_move_right(TextEditor *editor);
void editor_move_up(TextEditor *editor);
void editor_move_down(TextEditor *editor);
void editor_home(TextEditor *editor);
void editor_end(TextEditor *editor);
void editor_print(const TextEditor *editor);
void editor_status(const TextEditor *editor);
int editor_open(TextEditor *editor, const char *path);
int editor_save(TextEditor *editor, const char *path);
int editor_find(const TextEditor *editor, const char *needle);
int editor_replace(TextEditor *editor, const char *old_text, const char *new_text);
int editor_copy_line(TextEditor *editor);
int editor_cut_line(TextEditor *editor);
int editor_paste(TextEditor *editor);
int editor_undo(TextEditor *editor);
int editor_redo(TextEditor *editor);
void editor_help(void);

#endif
