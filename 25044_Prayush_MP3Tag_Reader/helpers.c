#include "helpers.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Status readIntFromBuffer(unsigned int *a, unsigned char *buffer) {
    *a = ((unsigned int)buffer[0] << 24) | ((unsigned int)buffer[1] << 16) |
         ((unsigned int)buffer[2] << 8) | ((unsigned int)buffer[3]);

    return success;
}

Status readSynchSafeIntFromBuffer(unsigned int *a, unsigned char *buffer) {
    *a = (buffer[0] << 21) | (buffer[1] << 14) | (buffer[2] << 7) | (buffer[3]);

    return success;
}

Status writeIntToBuffer(const unsigned int *a, unsigned char *buffer) {
    buffer[0] = (*a >> 24) & 0xFF;
    buffer[1] = (*a >> 16) & 0xFF;
    buffer[2] = (*a >> 8) & 0xFF;
    buffer[3] = (*a) & 0xFF;

    return success;
}

Status writeSynchSafeIntToBuffer(const unsigned int *a, unsigned char *buffer) {
    buffer[0] = (*a >> 21) & (0x7F);
    buffer[1] = (*a >> 14) & (0x7F);
    buffer[2] = (*a >> 7) & (0x7F);
    buffer[3] = (*a) & (0x7F);

    return success;
}

Status readInt(int *value) {
    char buffer[20];
    fgets(buffer, 20, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    for (int i = 0; buffer[i]; i++) {
        if (!isdigit(buffer[i]))
            return failure;
    }

    *value = atoi(buffer);
    return success;
}
