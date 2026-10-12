#include "mp3.h"
#include <stdio.h>
#include <string.h>

#define TAG_WIDTH 20
#define CONTENT_WIDTH 50
#define TABLE_WIDTH (2 + TAG_WIDTH + 3 + CONTENT_WIDTH)

/* ================= TABLE EDGES ================= */

void printEdge(char ch) {
    printf("+");
    for (int i = 0; i < TABLE_WIDTH - 2; i++)
        printf("%c", ch);
    printf("+\n");
}

void printCellEdge(void) { printEdge('-'); }

void printTableEdge(void) { printEdge('='); }

/* ================= TABLE HEADER ================= */

void printTableHeaders(void) {
    printTableEdge();

    printf("| ");
    printf("MP3 TAGS");
    for (int i = 0; i < (TABLE_WIDTH - 2 - 1 - 8); i++)
        printf(" ");
    printf("|\n");

    printCellEdge();

    printf("| %-*s | %-*s |\n", TAG_WIDTH - 2, "TAG", CONTENT_WIDTH, "CONTENT");

    printCellEdge();
}

/* ================= FRAME DISPLAY ================= */

void displayFrame(Frame *frame) {
    int len = strlen(frame->content);
    int i = 0;

    /* First line prints tag */
    printf("| %-*s | ", TAG_WIDTH - 2, frame->tag);

    int printed = 0;
    for (; printed < CONTENT_WIDTH && i < len; printed++, i++)
        putchar(frame->content[i]);

    for (; printed < CONTENT_WIDTH; printed++)
        printf(" ");

    printf(" |\n");

    /* Remaining wrapped lines */
    while (i < len) {
        printf("| %-*s | ", TAG_WIDTH - 2, "");

        printed = 0;
        for (; printed < CONTENT_WIDTH && i < len; printed++, i++)
            putchar(frame->content[i]);

        for (; printed < CONTENT_WIDTH; printed++)
            printf(" ");

        printf(" |\n");
    }

    printCellEdge();
}

/* ================= MESSAGE FUNCTIONS (UNCHANGED) ================= */

void printMessageEdge(int msgLength) {
    printf("+");
    for (int i = 1; i < msgLength - 1; i++)
        printf("=");
    printf("+\n");
}

void printMessage(const char *str) {
    int msgLength = strlen(str);
    msgLength += 10;
    printMessageEdge(msgLength);
    printf("||   %s   ||\n", str);
    printMessageEdge(msgLength);
}

void print_usage(const char *prog_name) {
    printf("Usage:\n\n");

    printf("1. View all MP3 tags:\n");
    printf("   %s -v <file.mp3>\n\n", prog_name);

    printf("2. Edit a specific tag:\n");
    printf("   %s -e <file.mp3> <flag> <content>\n\n", prog_name);

    printf("Available Flags:\n");
    printf("   -t   Edit Title\n");
    printf("   -a   Edit Artist\n");
    printf("   -A   Edit Album\n");
    printf("   -y   Edit Year\n");
    printf("   -c   Edit Comment\n");
    printf("   -C   Edit Composer\n\n");

    printf("Examples:\n");
    printf("   %s -v song.mp3\n", prog_name);
    printf("   %s -e song.mp3 -t \"New Title\"\n", prog_name);
    printf("   %s -e song.mp3 -a \"Artist Name\"\n", prog_name);
    printf("   %s -e song.mp3 -y \"2025\"\n\n", prog_name);

    printf("Notes:\n");
    printf("   - Use quotes if the content contains spaces.\n");
    printf("   - Only one tag can be edited per command.\n");
}
