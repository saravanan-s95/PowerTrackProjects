#ifndef TFTP_CLIENT_H
#define TFTP_CLIENT_H

#include "tftp.h" // Common TFTP definitions
#include <netinet/in.h> // sockaddr_in

typedef struct
{
    int sockfd; // Client socket
    struct sockaddr_in server_addr; // Server address
    socklen_t server_len; // Server address length
    char server_ip[INET_ADDRSTRLEN]; // Server IP address
    int mode; // Transfer mode
    int connected; // Connection status
}tftp_client_t;

void process_command(tftp_client_t *client,int choice); // Process menu choice
void connect_to_server(tftp_client_t *client); // Connect to server
void put_file_client(tftp_client_t *client); // Upload file
void get_file_client(tftp_client_t *client); // Download file
void disconnect_client(tftp_client_t *client); // Disconnect client
int validate_ip(char *ip); // Validate IPv4 address

#endif