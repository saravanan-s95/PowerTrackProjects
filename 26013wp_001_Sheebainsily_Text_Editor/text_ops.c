
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "editor_internal.h"

static Line *new_line(const char *text)
{
    Line *n = calloc(1, sizeof(*n));
    if (!n)
        return NULL;

    n->text = strdup(text ? text : "");
    if (!n->text) {
        free(n);
        return NULL;
    }
    return n;
}

static void clear_lines(TextEditor *e)
{
    Line *p = e->head;

    while (p) {
        Line *next = p->next;
        free(p->text);
        free(p);
        p = next;
    }

    e->head = e->tail = e->cursor = NULL;
}

char *editor_get_document(const TextEditor *e)
{
    size_t total = 1;

    for (Line *p = e->head; p; p = p->next)
        total += strlen(p->text) + (p->next ? 1 : 0);

    char *out = malloc(total);
    if (!out)
        return NULL;

    char *q = out;

    for (Line *p = e->head; p; p = p->next) {
        size_t n = strlen(p->text);
        memcpy(q, p->text, n);
        q += n;

        if (p->next)
            *q++ = '\n';
    }

    *q = '\0';
    return out;
}

int editor_set_document(TextEditor *e, const char *doc)
{
    char *copy = strdup(doc ? doc : "");
    if (!copy)
        return -1;

    clear_lines(e);

    char *start = copy;
    char *p;

    do {
        p = strchr(start, '\n');
        if (p)
            *p = '\0';

        Line *n = new_line(start);
        if (!n) {
            free(copy);
            clear_lines(e);
            return -1;
        }

        n->prev = e->tail;

        if (e->tail)
            e->tail->next = n;
        else
            e->head = n;

        e->tail = n;

        if (!p)
            break;

        start = p + 1;
    } while (1);

    e->cursor = e->head;
    e->cursor_line = 1;
    e->cursor_col = 0;

    free(copy);
    return 0;
}

static void record(TextEditor *e, char *before,
                   size_t line, size_t col)
{
    char *after = editor_get_document(e);

    if (before && after)
        history_record(e, before, line, col);
    else
        free(before);

    free(after);
}

void editor_init(TextEditor *e)
{
    memset(e, 0, sizeof(*e));
    editor_set_document(e, "");
}

void editor_destroy(TextEditor *e)
{
    clear_lines(e);
    free(e->clipboard);
    free(e->filename);

    extern void history_destroy(void);
    history_destroy();
}

int editor_insert(TextEditor *e, const char *text)
{
    if (!e || !e->cursor || !text)
        return -1;

    char *before = editor_get_document(e);
    size_t oldl = e->cursor_line;
    size_t oldc = e->cursor_col;

    size_t a = strlen(e->cursor->text);
    size_t b = strlen(text);

    char *s = malloc(a + b + 1);
    if (!s) {
        free(before);
        return -1;
    }

    memcpy(s, e->cursor->text, e->cursor_col);
    memcpy(s + e->cursor_col, text, b);
    memcpy(s + e->cursor_col + b,
           e->cursor->text + e->cursor_col,
           a - e->cursor_col + 1);

    free(e->cursor->text);
    e->cursor->text = s;
    e->cursor_col += b;

    record(e, before, oldl, oldc);
    return 0;
}

int editor_delete(TextEditor *e, size_t count)
{
    if (!e || !e->cursor || !e->cursor_col)
        return -1;

    if (count > e->cursor_col)
        count = e->cursor_col;

    char *before = editor_get_document(e);
    size_t oldl = e->cursor_line;
    size_t oldc = e->cursor_col;
    size_t len = strlen(e->cursor->text);

    memmove(e->cursor->text + e->cursor_col - count,
            e->cursor->text + e->cursor_col,
            len - e->cursor_col + 1);

    e->cursor_col -= count;
    record(e, before, oldl, oldc);
    return 0;
}

int editor_newline(TextEditor *e)
{
    if (!e || !e->cursor)
        return -1;

    char *before = editor_get_document(e);
    size_t oldl = e->cursor_line;
    size_t oldc = e->cursor_col;

    char *right = strdup(e->cursor->text + e->cursor_col);
    if (!right) {
        free(before);
        return -1;
    }

    Line *n = calloc(1, sizeof(*n));
    if (!n) {
        free(right);
        free(before);
        return -1;
    }

    e->cursor->text[e->cursor_col] = '\0';
    n->text = right;
    n->prev = e->cursor;
    n->next = e->cursor->next;

    if (n->next)
        n->next->prev = n;
    else
        e->tail = n;

    e->cursor->next = n;
    e->cursor = n;
    e->cursor_line++;
    e->cursor_col = 0;

    record(e, before, oldl, oldc);
    return 0;
}

void editor_move_left(TextEditor *e)
{
    if (e->cursor_col)
        e->cursor_col--;
    else if (e->cursor->prev) {
        e->cursor = e->cursor->prev;
        e->cursor_line--;
        e->cursor_col = strlen(e->cursor->text);
    }
}

void editor_move_right(TextEditor *e)
{
    if (e->cursor_col < strlen(e->cursor->text))
        e->cursor_col++;
    else if (e->cursor->next) {
        e->cursor = e->cursor->next;
        e->cursor_line++;
        e->cursor_col = 0;
    }
}

void editor_move_up(TextEditor *e)
{
    if (e->cursor->prev) {
        e->cursor = e->cursor->prev;
        e->cursor_line--;

        if (e->cursor_col > strlen(e->cursor->text))
            e->cursor_col = strlen(e->cursor->text);
    }
}

void editor_move_down(TextEditor *e)
{
    if (e->cursor->next) {
        e->cursor = e->cursor->next;
        e->cursor_line++;

        if (e->cursor_col > strlen(e->cursor->text))
            e->cursor_col = strlen(e->cursor->text);
    }
}

void editor_home(TextEditor *e)
{
    e->cursor_col = 0;
}

void editor_end(TextEditor *e)
{
    e->cursor_col = strlen(e->cursor->text);
}

void editor_print(const TextEditor *e)
{
    size_t i = 1;

    for (Line *p = e->head; p; p = p->next, ++i)
        printf("%4zu | %s\n", i, p->text);

    if (!e->head)
        puts("(empty document)");

    editor_status(e);
}

void editor_status(const TextEditor *e)
{
    printf("Cursor: line %zu, column %zu\n",
           e->cursor_line, e->cursor_col);
}

int editor_copy_line(TextEditor *e)
{
    if (!e || !e->cursor)
        return -1;

    char *s = strdup(e->cursor->text ? e->cursor->text : "");
    if (!s)
        return -1;

    free(e->clipboard);
    e->clipboard = s;
    return 0;
}

int editor_cut_line(TextEditor *e)
{
    if (!e || !e->cursor)
        return -1;

    if (editor_copy_line(e))
        return -1;

    char *before = editor_get_document(e);
    if (!before)
        return -1;

    size_t oldl = e->cursor_line;
    size_t oldc = e->cursor_col;

    if (e->head == e->tail) {
        char *empty = strdup("");
        if (!empty) {
            free(before);
            return -1;
        }

        free(e->cursor->text);
        e->cursor->text = empty;
        e->cursor_col = 0;
    } else {
        Line *removed = e->cursor;
        Line *next_cursor = removed->next
                          ? removed->next : removed->prev;

        if (removed->prev)
            removed->prev->next = removed->next;
        else
            e->head = removed->next;

        if (removed->next)
            removed->next->prev = removed->prev;
        else
            e->tail = removed->prev;

        free(removed->text);
        free(removed);

        e->cursor = next_cursor;

        if (e->cursor == e->head)
            e->cursor_line = 1;
        else if (next_cursor->next)
            e->cursor_line = oldl;
        else
            e->cursor_line = oldl - 1;

        size_t len = strlen(e->cursor->text);
        e->cursor_col = oldc < len ? oldc : len;
    }

    record(e, before, oldl, oldc);
    return 0;
}

int editor_paste(TextEditor *e)
{
    if (!e || !e->cursor || !e->clipboard)
        return -1;

    char *before = editor_get_document(e);
    if (!before)
        return -1;

    size_t oldl = e->cursor_line;
    size_t oldc = e->cursor_col;

    Line *n = new_line(e->clipboard);
    if (!n) {
        free(before);
        return -1;
    }

    /* Insert the copied text as a new line after the current line. */
    n->prev = e->cursor;
    n->next = e->cursor->next;

    if (e->cursor->next)
        e->cursor->next->prev = n;
    else
        e->tail = n;

    e->cursor->next = n;

    record(e, before, oldl, oldc);
    return 0;
}
