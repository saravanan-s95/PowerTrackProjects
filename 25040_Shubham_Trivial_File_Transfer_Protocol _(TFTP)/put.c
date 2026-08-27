#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "tftp.h"

void put_file(tftp_client_t *client, char *filename)
{
    if (!client->connected) {
        printf("Connect to a server first.\n");
        return;
    }

    send_request(client->sockfd, client->server_addr, filename, WRQ);

    /*
     * The server's WRQ response is ACK block 0. Once it arrives,
     * send the file through the common receive/send implementation.
     */
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        return;
    }
    fclose(fp);

    /*
     * receive_file() is the server-side WRQ handler. The actual client
     * upload is implemented here to preserve the original function API.
     */
    FILE *input = fopen(filename, "rb");
    if (!input) 
    {
        perror("fopen");
        return;
    }

    unsigned char packet[BUFFER_SIZE];
    unsigned char ack[BUFFER_SIZE];
    uint16_t block = 1;

    while(1)
    {
        size_t count = fread(packet + 4, 1, DATA_SIZE, input);
        if (ferror(input)) {
            perror("fread");
            fclose(input);
            return;
        }

        uint16_t op = htons(DATA);
        uint16_t bn = htons(block);
        memcpy(packet, &op, 2);
        memcpy(packet + 2, &bn, 2);

        int ok = 0;
        for (int retry = 0; retry < MAX_RETRIES && !ok; ++retry) {
            if (sendto(client->sockfd, packet, count + 4, 0,(struct sockaddr *)&client->server_addr, client->server_len) < 0) 
            {
                perror("sendto");
                fclose(input);
                return;
            }
            struct sockaddr_in from;
            socklen_t from_len = sizeof(from);
            ssize_t n = recvfrom(client->sockfd, ack, sizeof(ack), 0,(struct sockaddr *)&from, &from_len);
            if (n >= 4) {
                uint16_t aop, ablock;
                memcpy(&aop, ack, 2);
                memcpy(&ablock, ack + 2, 2);
                aop = ntohs(aop);
                ablock = ntohs(ablock);

                if (aop == ACK && ablock == block)
                    ok = 1;
            } else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recvfrom");
                fclose(input);
                return;
            }
        }
        if (!ok) {
            printf("Upload failed: timeout on block %u.\n", block);
            fclose(input);
            return;
        }
        if (count < DATA_SIZE)
            break;
        block++;
        if (block == 0)
            block = 1;
    }
    fclose(input);
    printf("Uploaded file: %s\n", filename);
}