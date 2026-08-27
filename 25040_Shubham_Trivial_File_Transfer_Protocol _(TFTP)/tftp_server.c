#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "tftp.h"

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet);

int validate_request(unsigned char *raw, ssize_t len, uint16_t *opcode, char *filename, size_t filename_size)
{
    if (len < 4)
        return -1;

    uint16_t op;
    memcpy(&op, raw, 2);
    op = ntohs(op);

    if (op != RRQ && op != WRQ)
        return -1;

    size_t pos = 2;
    size_t i = 0;

    while (pos + i< (size_t)len && raw[pos + i] != '\0')
        i++;

    if (pos + i >= (size_t)len ||
        i == 0 || i >= filename_size)
        return -1;

    strcpy(filename, raw + pos);
    filename[i] = '\0';

    *opcode = op;
    return 0;
}

int main(void)
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    unsigned char raw_packet[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

   /// memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) 
    {
        perror("bind");
        close(sockfd);
        return 1;
    }

    printf("TFTP Server listening on port %d...\n", PORT);

    while (1) {
        memset(&client_addr, 0, sizeof(client_addr));
        client_len = sizeof(client_addr);

        ssize_t n = recvfrom(sockfd, raw_packet, sizeof(raw_packet), 0, (struct sockaddr *)&client_addr, &client_len);

        if (n < 0) {
            if (errno == EINTR)
                continue;
            perror("recvfrom");
            continue;
        }

        uint16_t opcode;
        char filename[256];

        if (validate_request(raw_packet, n, &opcode, filename, sizeof(filename)) < 0) 
        {
            fprintf(stderr, "Invalid TFTP request received.\n");
            continue;
        }

       
        // if (strstr(filename, "..") != NULL ||filename[0] == '/' || strchr(filename, '\\') != NULL) {
        //     fprintf(stderr, "Rejected unsafe filename: %s\n", filename);
        //     continue;
        // }

        tftp_packet packet;
        memset(&packet, 0, sizeof(packet));
        packet.opcode = opcode;
        strncpy(packet.body.request.filename,filename,sizeof(packet.body.request.filename) - 1);

        printf("Request from %s:%d: %s %s\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),opcode == RRQ ? "RRQ" : "WRQ",filename);

        handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);
    return 0;
}

void handle_client(int sockfd, struct sockaddr_in client_addr,socklen_t client_len, tftp_packet *packet)
{
    uint16_t opcode = ntohs(packet->opcode);
    if (opcode == RRQ) 
    {
        send_file(sockfd, client_addr, client_len, packet->body.request.filename);
    } else if (opcode == WRQ) 
    {
        receive_file(sockfd, client_addr, client_len, packet->body.request.filename);
    }
}
