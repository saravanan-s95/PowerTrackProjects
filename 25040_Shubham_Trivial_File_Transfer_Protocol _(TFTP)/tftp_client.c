#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>

#include "tftp.h"
#include "tftp_client.h"

struct sockaddr_in server_info;

static void print_menu(void)
{
    printf("\nCommands:\n");
    printf("  connect <IP> [port]       Connect to server\n");
    printf("  get <filename>             Receive a file from server\n");
    printf("  put <filename>             Send a file to server\n");
    printf("  mode <octet|netascii|mail> Set transfer mode\n");
    printf("  bye / quit                 Close and exit\n");
    printf("  help                       Show this menu\n\n");
}

int main(void)
{
    char command[256];
    tftp_client_t client;

    memset(&client, 0, sizeof(client));
    client.sockfd = -1;

    print_menu();

    while (1) {
        printf("tftp> ");
        fflush(stdout);

        if (!fgets(command, sizeof(command), stdin))
            break;

        command[strcspn(command, "\n")] = '\0';

        if (command[0] == '\0')
            continue;

        process_command(&client, command);

        if (!strcmp(command, "quit") || !strcmp(command, "bye"))
            break;
    }

    disconnect(&client);
    return 0;
}

void process_command(tftp_client_t *client, char *command)
{
    char copy[256];
    char *args[4];
    int argc = 0;

    strncpy(copy, command, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char *token = strtok(copy, " \t");
    while (token && argc < 4) {
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }

    if (argc == 0)
        return;

    if (!strcmp(args[0], "help")) 
    {
        print_menu();
    } else if (strcmp(args[0], "connect") == 0) 
    {
        if (argc < 2) {
            printf("Usage: connect <IP> [port]\n");
            return;
        }

        int port = (argc >= 3) ? atoi(args[2]) : PORT;
        connect_to_server(client, args[1], port);

    } 
    else if (strcmp(args[0], "get") == 0) 
    {
        if (argc < 2) {
            printf("Usage: get <filename>\n");
            return;
        }
        get_file(client, args[1]);

    } else if (strcmp(args[0], "put") ==0) 
    {
        if (argc < 2) {
            printf("Usage: put <filename>\n");
            return;
        }
        put_file(client, args[1]);

    } else if (strcmp(args[0], "mode")==0) 
    {
        if (argc < 2) 
        {
            printf("Usage: mode <octet|netascii|mail>\n");
        } 
        else if (!strcmp(args[1], "octet") ||   !strcmp(args[1], "netascii") ||!strcmp(args[1], "mail")) 
        {
            printf("Transfer mode set to %s.\n", args[1]);
        } 
        else 
        {
            printf("Unsupported mode: %s\n", args[1]);
        }

    } 
    else if (!strcmp(args[0], "bye") || !strcmp(args[0], "quit")) 
    {
        disconnect(client);
        strcpy(command, args[0]);

    } 
    else 
    {
        printf("Unknown command. Type 'help'.\n");
    }
}

void connect_to_server(tftp_client_t *client, char *ip, int port)
{
    if (client->connected)
        disconnect(client);

    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client->sockfd < 0) {
        perror("socket");
        return;
    }

    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;

    if (setsockopt(client->sockfd, SOL_SOCKET, SO_RCVTIMEO,  &tv, sizeof(tv)) < 0) {
        perror("setsockopt");
        close(client->sockfd);
        client->sockfd = -1;
        return;
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons((uint16_t)port);

    if (inet_pton(AF_INET, ip, &server_info.sin_addr) != 1) {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        close(client->sockfd);
        client->sockfd = -1;
        return;
    }

    client->server_addr = server_info;
    client->server_len = sizeof(server_info);

    strncpy(client->server_ip, ip, sizeof(client->server_ip) - 1);
    client->server_ip[sizeof(client->server_ip) - 1] = '\0';
    client->connected = 1;

    printf("Connected to %s:%d\n", ip, port);
}

void disconnect(tftp_client_t *client)
{
    if (client->sockfd >= 0) {
        close(client->sockfd);
        client->sockfd = -1;
    }
    client->connected = 0;
    printf("Disconnected.\n");
}

void send_request(int sockfd, struct sockaddr_in server_addr,char *filename, int opcode)
{
    unsigned char packet[BUFFER_SIZE];
    size_t filename_len = strlen(filename);
    const char *mode = "octet";
    size_t mode_len = strlen(mode);

    if (filename_len == 0 ||
        filename_len + mode_len + 4 >= sizeof(packet)) {
        fprintf(stderr, "Invalid filename.\n");
        return;
    }

    uint16_t op = htons((uint16_t)opcode);
    memcpy(packet, &op, 2);

    memcpy(packet + 2, filename, filename_len + 1);
    memcpy(packet + 2 + filename_len + 1, mode, mode_len + 1);

    size_t packet_len = 2 + filename_len + 1 + mode_len + 1;

    if (sendto(sockfd, packet, packet_len, 0, (struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
        perror("sendto");
}

void receive_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode)
{
    /*
     * Kept as part of the original project API.
     * A receive request is simply an RRQ/WRQ packet.
     */
    send_request(sockfd, server_addr, filename, opcode);
}
