#ifndef TFTP_UDP
#define TFTP_UDP

#include <sys/socket.h>

/* Create and bind server socket */
void udp_bind_server();

/* Rebind server socket to new port */
void udp_rebind_server();

/* Create client socket */
void udp_bind_client();

/* Send packet using UDP */
void udp_send_packet();

#endif