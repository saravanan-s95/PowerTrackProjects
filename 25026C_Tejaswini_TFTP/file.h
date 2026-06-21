/*
 * file.h - File operations header
 */
#ifndef FILE_H
#define FILE_H

int file_open_read(char *filename);
int file_open_write(char *filename);
int file_buffer_from_pos(char *buffer, int size);
int file_append_from_buffer(char *buffer, int size);
int file_close(void);

#endif