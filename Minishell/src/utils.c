#include "header.h"

void extract_external_commands(const char *cmdv[], int *cmd_count) {
    FILE *fptr = fopen("external_commands.txt", "r");
    if (fptr == NULL) {
        printf("Unable to open file\n");
        return;
    }
    int cmd_ind = 0;
    while (feof(fptr) == 0) {
        char local_buffer[100];
        if (fgets(local_buffer, 100, fptr) == NULL)
            break;
        local_buffer[strcspn(local_buffer, "\n")] = '\0';
        char *curr_cmd = malloc((strlen(local_buffer) + 1) * sizeof(char));
        strcpy(curr_cmd, local_buffer);
        cmdv[cmd_ind++] = curr_cmd;
    }
    *cmd_count = cmd_ind;
    return;
}

char bin_search(const char *const argv[], int size, const char *target) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(argv[mid], target);
        if (cmp == 0)
            return 1;
        else if (cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;
}

void getwords(char *tokens[], int *count) {
    int token_count = 0;
    int prev_ind = -1;
    int curr_ind = 0;
    // use prev ind to keep track of last ' '
    while (input_string[curr_ind]) {
        char curr = input_string[curr_ind];
        char prev = (prev_ind == -1) ? ' ' : input_string[prev_ind];
        if ((prev == '\t' || prev == ' ') && curr != ' ') {
            // word found when prev is ' ' and curr is not ' '
            prev_ind = curr_ind;
            // iterate forward till curr is NULL or ' ' that is the end of word
            while (input_string[curr_ind] != '\0' &&
                   input_string[curr_ind] != ' ')
                curr_ind++;
            // word length = curr pos - prev , malloc and copy the pointer to it
            int len = curr_ind - prev_ind + 1;
            char *curr_token = malloc(len * sizeof(char));
            strncpy(curr_token, input_string + prev_ind, len);
            curr_token[len - 1] = '\0';
            tokens[token_count++] = curr_token;
        } else {
            prev_ind = curr_ind++;
        }
    }
    tokens[token_count++] = NULL;
    *count = token_count;
}

void setup_pipe_commands(char *tokens[], int *pipe_count, int *cmd_ind) {
    int count = 0;
    int ind = 0;
    cmd_ind[ind++] = 0;

    for (int i = 0; tokens[i]; i++) {
        if (tokens[i][0] == '|' && tokens[i][1] == '\0') {
            count++;
            cmd_ind[ind++] = i + 1;
            free(tokens[i]);
            tokens[i] = NULL;
        }
    }
    *pipe_count = count;
}

void free_tokens(char *tokens[], int token_count) {
    for (int i = 0; i < token_count; i++) {
        if (tokens[i])
            free(tokens[i]);
    }
}
