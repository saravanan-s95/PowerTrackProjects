#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "udp.h"
#include "tftp.h"

int server_sockfd;
int client_sockfd;

struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

/* Create and bind server socket */
void udp_bind_server(int port)
{
    server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    printf("Server listening on port %d\n", port);
}

/* Rebind server to another port */
void udp_rebind_server(int port)
{
    close(server_sockfd);

    server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
}

/* Create client socket */
void udp_bind_client()
{
    client_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_sockfd < 0)
    {
        perror("socket");
        exit(1);
    }
}

/* Send packet */
void udp_send_packet(int sockfd, char *packet, int len, struct sockaddr_in *addr)
{
    sendto(sockfd, packet, len, 0, (struct sockaddr *)addr, sizeof(*addr));
}