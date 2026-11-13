#ifndef EDITOR_INTERNAL_H
#define EDITOR_INTERNAL_H
#include "text_editor.h"

typedef struct {
    char *before;
    char *after;
    size_t cursor_line_before;
    size_t cursor_col_before;
    size_t cursor_line_after;
    size_t cursor_col_after;
} Snapshot;

int editor_set_document(TextEditor *editor, const char *document);
char *editor_get_document(const TextEditor *editor);
void history_clear_redo(void);
void history_record(TextEditor *editor, const char *before,
                    size_t line_before, size_t col_before);
#endif
