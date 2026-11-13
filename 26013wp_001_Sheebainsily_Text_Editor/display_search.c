#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "editor_internal.h"

int editor_find(const TextEditor *e, const char *needle)
{
    if (!e || !needle || !*needle) {
        puts("Search text cannot be empty.");
        return -1;
    }

    size_t line = 1;
    int found = 0;

    for (Line *p = e->head; p; p = p->next, ++line) {
        const char *at = p->text;
        while ((at = strstr(at, needle)) != NULL) {
            printf("Found at line %zu, column %zu\n",
                   line, (size_t)(at - p->text));
            ++found;
            ++at;
        }
    }

    if (!found)
        puts("No match found.");
    else
        printf("%d match(es).\n", found);

    return found ? 0 : -1;
}

int editor_replace(TextEditor *e, const char *old_text, const char *new_text)
{
    if (!e || !old_text || !*old_text || !new_text)
        return 0;

    char *before = editor_get_document(e);
    if (!before)
        return 0;

    size_t old_line = e->cursor_line;
    size_t old_col = e->cursor_col;
    int count = 0;
    size_t old_len = strlen(old_text);
    size_t new_len = strlen(new_text);

    for (Line *line = e->head; line; line = line->next) {
        char *at = line->text;

        while ((at = strstr(at, old_text)) != NULL) {
            size_t prefix = (size_t)(at - line->text);
            size_t line_len = strlen(line->text);

            if (new_len > old_len) {
                size_t extra = new_len - old_len;
                char *grown = realloc(line->text, line_len + extra + 1);
                if (!grown) {
                    /* Keep any successful replacements and record them. */
                    if (count > 0)
                        history_record(e, before, old_line, old_col);
                    else
                        free(before);
                    return count;
                }
                line->text = grown;
                at = line->text + prefix;
            }

            memmove(at + new_len, at + old_len,
                    strlen(at + old_len) + 1);
            memcpy(at, new_text, new_len);
            ++count;
            at += new_len;
        }
    }

    if (count > 0)
        history_record(e, before, old_line, old_col);
    else
        free(before);

    return count;
}
