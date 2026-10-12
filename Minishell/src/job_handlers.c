#include "header.h"

int insert_job(int pid, char *process_name, job **head) {
    job *new_job = malloc(sizeof(job));
    if (new_job == NULL)
        return 1;
    new_job->pid = pid;
    new_job->process_name = malloc((strlen(process_name) + 1) * sizeof(char));
    if (new_job->process_name == NULL) {
        free(new_job);
        return 1;
    }
    strcpy(new_job->process_name, process_name);
    new_job->next = *head;
    *head = new_job;
    return 0;
}

void print_jobs(job *head) {
    if (head == NULL) {
        printf("%sNo Process in job list\n%s", ANSI_COLOR_RED,
               ANSI_COLOR_RESET);
        return;
    }
    print_jobs(head->next);
    printf("Process ID : %d | Process Name : %s\n", head->pid,
           head->process_name);
}

void remove_job(job **head) {
    job *to_free = *head;
    *head = (*head)->next;
    free(to_free->process_name);
    free(to_free);
}
