#ifndef TFTP_FILE
#define TFTP_FILE

#include "tftp.h"

/* Function declarations for file operations */

/* Open file for reading */
int file_open_read();

/* Read data from file into buffer */
int file_buffer_from_pos();

/* Open file for writing */
int file_open_write();

/* Write buffer contents into file */
int file_append_from_buffer();

/* Close file */
int file_close();

#endif