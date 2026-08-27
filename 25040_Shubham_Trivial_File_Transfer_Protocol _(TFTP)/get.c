#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "tftp.h"
void get_file(tftp_client_t *client, char *filename)
{
    if (!client->connected) {
        printf("Connect to a server first.\n");
        return;
    }
    send_request(client->sockfd, client->server_addr, filename, RRQ);
    FILE *output = fopen(filename, "wb");
    if (!output) {
        perror("fopen");
        return;
    }

    unsigned char packet[BUFFER_SIZE];
    unsigned char ack[4];
    uint16_t expected = 1;

    while(1)
    {
        int received = 0;

        for (int i = 0; i< MAX_RETRIES && !received; i++) {
            struct sockaddr_in from;
            socklen_t from_len = sizeof(from);

            ssize_t n = recvfrom(client->sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&from, &from_len);

            if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    send_request(client->sockfd, client->server_addr, filename, RRQ);
                    continue;
                }
                perror("recvfrom");
                fclose(output);
                return;
            }

            if (n < 4)
                continue;

            uint16_t op, block;
            memcpy(&op, packet, 2);
            memcpy(&block, packet + 2, 2);
            op = ntohs(op);
            block = ntohs(block);

            if (op == ERROR) {
                printf("Server error: %s\n",n > 4 ? (packet + 4) : "Unknown error");
                fclose(output);
                return;
            }

            if (op != DATA || block != expected)
                continue;

            size_t data_len = (size_t)n - 4;

            if (fwrite(packet + 4, 1, data_len, output) != data_len) {
                perror("fwrite");
                fclose(output);
                return;
            }

            uint16_t ack_op = htons(ACK);
            uint16_t ack_block = htons(block);
            memcpy(ack, &ack_op, 2);
            memcpy(ack + 2, &ack_block, 2);

            sendto(client->sockfd, ack, sizeof(ack), 0,
                   (struct sockaddr *)&from, from_len);

            client->server_addr = from;
            client->server_len = from_len;
            received = 1;

            if (data_len < DATA_SIZE) {
                fclose(output);
                printf("Downloaded file: %s\n", filename);
                return;
            }
            expected++;
            if (expected == 0)
                expected = 1;
        }
        if (!received) {
            printf("Download failed: timeout on block %u.\n", expected);
            fclose(output);
            return;
        }
    }
}
