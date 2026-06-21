/*
 * udp.h - Socket definitions
 */
#ifndef UDP_H
#define UDP_H

#include <sys/socket.h>
#include <netinet/in.h>

void udp_bind_server(int *sockfd, int port);
void udp_rebind_server(int *sockfd, int port);
void udp_bind_client(int *sockfd);
void udp_send_packet(int sockfd, struct sockaddr_in *dest_addr, char *buf, int len);

#endif