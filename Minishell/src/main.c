#include "header.h"

char prompt[100];
job *g_job_list;
char input_string[100];
pid_t child_pid;

int main() {
    sprintf(prompt, "%sMini$hell%s> %s", ANSI_COLOR_BLUE, ANSI_COLOR_YELLOW,
            ANSI_COLOR_RESET);
    scan_input();
}
