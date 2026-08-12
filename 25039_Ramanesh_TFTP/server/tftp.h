#ifndef TFTP_H
#define TFTP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

#define PORT            3020
#define BUFFER_SIZE     516
#define DATA_SIZE       512
#define TIMEOUT_SEC     5
#define MAX_RETRIES     3

#define MODE_NORMAL     1
#define MODE_OCTET      2
#define MODE_NETASCII   3

typedef enum
{
    RRQ   = 1,
    WRQ   = 2,
    DATA  = 3,
    ACK   = 4,
    ERROR = 5

} tftp_opcode_t;

/* File Transfer Functions */

void send_file(int sockfd,
               struct sockaddr_in client_addr,
               socklen_t client_len,
               char *filename,
               int mode);

void receive_file(int sockfd,
                  struct sockaddr_in client_addr,
                  socklen_t client_len,
                  char *filename,
                  int mode);

/* Mode Helper Functions */

const char *get_mode_display(int mode);

const char *get_mode_string(int mode);

int get_mode_value(const char *mode);

#endif