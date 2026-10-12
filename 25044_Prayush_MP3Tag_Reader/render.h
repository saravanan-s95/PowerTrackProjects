#ifndef RENDER_TABLE_H
#define RENDER_TABLE_H
typedef struct Frame Frame;

void printCellEdge(void);
void printTableEdge(void);
void printTableHeaders(void);
void printMessage(const char *str);
void print_usage(const char *prog_name);

void displayFrame(Frame *frame);
#endif // !RENDER_TABLE_H
