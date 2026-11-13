/*
 * Project: Text Editor
 * File: main.c
 * Author: Sheeba Insily
 * Description: Console-based text editor using C.
 * Features: Insert, delete, undo, redo, search, replace,
 *           cursor navigation, and file operations.
 * Data Structures: Doubly linked list and stacks.
 * Compilation: make
 * Execution: ./text_editor
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_editor.h"

static void trim_newline(char *s) {
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r')) s[--n] = '\0';
}

static char *read_argument(const char *prompt) {
    char buffer[4096];
    fputs(prompt, stdout);
    if (!fgets(buffer, sizeof buffer, stdin)) return NULL;
    trim_newline(buffer);
    return strdup(buffer);
}

static void show_menu(void) {
    puts("\n========== TEXT EDITOR v1.0 ==========");
    puts(" insert <text>       Insert text at cursor");
    puts(" newline             Create a new line at cursor");
    puts(" delete <count>      Delete characters before cursor");
    puts(" left/right/up/down  Move cursor");
    puts(" home/end            Move to line start/end");
    puts(" print               Display document");
    puts(" status              Show cursor position");
    puts(" copy/cut/paste      Clipboard operations on current line");
    puts(" find <text>         Search document");
    puts(" replace <old>|<new> Replace all occurrences");
    puts(" open <path>         Open a text file");
    puts(" save [path]         Save document");
    puts(" undo/redo           Undo or redo edits");
    puts(" help                Show commands");
    puts(" exit                Quit");
    puts("======================================");
}

int main(void) {
    TextEditor editor;
    char input[4096];
    editor_init(&editor);
    puts("Text Editor v1.0 - type 'help' for commands.");
    while (1) {
        char *arg;
        fputs("\neditor> ", stdout);
        if (!fgets(input, sizeof input, stdin)) break;
        trim_newline(input);
        if (!input[0]) continue;
        if (!strcmp(input, "exit") || !strcmp(input, "quit")) break;
        if (!strcmp(input, "help")) { show_menu(); continue; }
        if (!strcmp(input, "print")) { editor_print(&editor); continue; }
        if (!strcmp(input, "status")) { editor_status(&editor); continue; }
        if (!strcmp(input, "left")) { editor_move_left(&editor); continue; }
        if (!strcmp(input, "right")) { editor_move_right(&editor); continue; }
        if (!strcmp(input, "up")) { editor_move_up(&editor); continue; }
        if (!strcmp(input, "down")) { editor_move_down(&editor); continue; }
        if (!strcmp(input, "home")) { editor_home(&editor); continue; }
        if (!strcmp(input, "end")) { editor_end(&editor); continue; }
        if (!strcmp(input, "newline")) {
            if (!editor_newline(&editor)) puts("New line created.");
            else puts("Could not create new line.");
            continue;
        }
        if (!strcmp(input, "copy")) { if (editor_copy_line(&editor)) puts("Copy failed."); else puts("Current line copied."); continue; }
        if (!strcmp(input, "cut")) { if (editor_cut_line(&editor)) puts("Cut failed."); else puts("Current line cut."); continue; }
        if (!strcmp(input, "paste")) { if (editor_paste(&editor)) puts("Paste failed."); else puts("Clipboard pasted."); continue; }
        if (!strcmp(input, "undo")) { puts(editor_undo(&editor) ? "Nothing to undo." : "Undo successful."); continue; }
        if (!strcmp(input, "redo")) { puts(editor_redo(&editor) ? "Nothing to redo." : "Redo successful."); continue; }
        if (!strncmp(input, "insert ", 7)) {
            if (editor_insert(&editor, input + 7)) puts("Insert failed.");
            else puts("Text inserted.");
            continue;
        }
        if (!strncmp(input, "delete ", 7)) {
            char *endp;
            unsigned long n = strtoul(input + 7, &endp, 10);
            if (*endp || endp == input + 7) puts("Usage: delete <count>");
            else if (editor_delete(&editor, (size_t)n)) puts("Nothing deleted.");
            else puts("Characters deleted.");
            continue;
        }
        if (!strncmp(input, "find ", 5)) {
            if (editor_find(&editor, input + 5)) puts("No match found.");
            continue;
        }
        if (!strncmp(input, "replace ", 8)) {
            char *sep = strchr(input + 8, '|');
            if (!sep) { puts("Usage: replace <old>|<new>"); continue; }
            *sep = '\0';
            printf("Replaced %d occurrence(s).\n", editor_replace(&editor, input + 8, sep + 1));
            continue;
        }
        if (!strncmp(input, "open ", 5)) {
            if (editor_open(&editor, input + 5)) perror("Open failed");
            else printf("Opened %s\n", input + 5);
            continue;
        }
        if (!strcmp(input, "save")) {
            if (!editor.filename) {
                arg = read_argument("Save path: ");
                if (!arg) continue;
                if (editor_save(&editor, arg)) perror("Save failed");
                else printf("Saved to %s\n", arg);
                free(arg);
            } else if (editor_save(&editor, editor.filename)) perror("Save failed");
            else printf("Saved to %s\n", editor.filename);
            continue;
        }
        if (!strncmp(input, "save ", 5)) {
            if (editor_save(&editor, input + 5)) perror("Save failed");
            else printf("Saved to %s\n", input + 5);
            continue;
        }
        puts("Unknown command. Type 'help'.");
    }
    editor_destroy(&editor);
    puts("Goodbye.");
    return 0;
}
