/*
 * udp.c - UDP socket helper implementations
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "udp.h"

void udp_bind_server(int *sockfd, int port)
{
    struct sockaddr_in addr;
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    // Let's fix standard SOCK_DGRAM instead of DIS_SOCK_DGRAM
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    int opt = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(*sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void udp_rebind_server(int *sockfd, int port)
{
    udp_bind_server(sockfd, port);
}

void udp_bind_client(int *sockfd)
{
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void udp_send_packet(int sockfd, struct sockaddr_in *dest_addr, char *buf, int len)
{
    sendto(sockfd, buf, len, 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr));
}