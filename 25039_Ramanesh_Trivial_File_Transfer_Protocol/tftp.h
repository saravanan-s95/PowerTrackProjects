#ifndef TFTP_H
#define TFTP_H

#include <stdio.h> // Standard I/O
#include <stdlib.h> // Standard library
#include <stdint.h> // Fixed width integers
#include <string.h> // String functions
#include <unistd.h> // POSIX functions
#include <arpa/inet.h> // Socket functions
#include <sys/socket.h> // Socket API
#include <sys/types.h> // System data types
#include <sys/time.h> // Timeout structure

#define PORT 3020 // Server port
#define BUFFER_SIZE 516 // 512 bytes data + 4 bytes header
#define DATA_SIZE 512 // TFTP data size
#define TIMEOUT_SEC 5 // Timeout in seconds
#define MAX_RETRIES 3 // Maximum retransmissions

#define MODE_NORMAL 1 // Normal mode
#define MODE_OCTET 2 // Octet mode
#define MODE_NETASCII 3 // Netascii mode

typedef enum
{
    RRQ = 1, // Read request
    WRQ = 2, // Write request
    DATA = 3, // Data packet
    ACK = 4, // Acknowledgement
    ERROR = 5 // Error packet
}tftp_opcode_t;

void send_file(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *filename,int mode); // Send file
void receive_file(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *filename,int mode); // Receive file

#endif