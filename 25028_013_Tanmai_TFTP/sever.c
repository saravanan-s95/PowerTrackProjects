#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "udp.h"
#include "tftp.h"

int sockfd;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

int main()
{
    socklen_t client_len;
    packet_t pkt;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6969);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    printf("Server listening on port 6969\n");

    client_len = sizeof(client_addr);

    while (1)
    {
        recvfrom(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&client_addr, &client_len);

        printf("\nPacket Received\n");

        /* RRQ Handling */
        if (pkt.opcode == OPCODE_RRQ)
        {
            printf("RRQ received\n");
            printf("Filename : %s\n", pkt.filename);
            printf("Mode     : %s\n", pkt.mode);

            packet_t ack_pkt;

            memset(&ack_pkt, 0, sizeof(ack_pkt));
            ack_pkt.opcode = OPCODE_ACK;

            sendto(sockfd, &ack_pkt, sizeof(ack_pkt), 0, (struct sockaddr *)&client_addr, client_len);

            printf("ACK sent to client\n");

            packet_t data_pkt;

            FILE *fp = fopen(pkt.filename, "r");

            if (fp == NULL)
            {
                packet_t err_pkt;

                memset(&err_pkt, 0, sizeof(err_pkt));

                err_pkt.opcode = OPCODE_ERROR;

                err_pkt.ecode = ECODE_1;

                strcpy(err_pkt.estring, "File not found");

                sendto(sockfd, &err_pkt, sizeof(err_pkt), 0, (struct sockaddr *)&client_addr, client_len);

                printf("ERROR packet sent\n");

                continue;
            }

            bnum_t block = 1;
            int bytes_read;

            while (1)
            {
                memset(&data_pkt, 0, sizeof(data_pkt));

                data_pkt.opcode = OPCODE_DATA;
                data_pkt.blocknum = block;

                bytes_read = fread(data_pkt.data, 1, BLIMIT, fp);

                data_pkt.data_length = bytes_read;

                sendto(sockfd, &data_pkt, sizeof(data_pkt), 0, (struct sockaddr *)&client_addr, client_len);

                printf("DATA Block %d sent (%d bytes)\n", block, bytes_read);

                recvfrom(sockfd, &ack_pkt, sizeof(ack_pkt), 0, (struct sockaddr *)&client_addr, &client_len);

                if (ack_pkt.opcode == OPCODE_ACK)
                {
                    printf("ACK Block %d received\n", ack_pkt.blocknum);
                }

                if (bytes_read < BLIMIT)
                {
                    break;
                }

                block++;
            }

            fclose(fp);
        }

        /* WRQ Handling */
        else if (pkt.opcode == OPCODE_WRQ)
        {
            printf("WRQ received\n");

            printf("Filename : %s\n", pkt.filename);
            printf("Mode     : %s\n", pkt.mode);

            packet_t ack_pkt;

            memset(&ack_pkt, 0, sizeof(ack_pkt));
            ack_pkt.opcode = OPCODE_ACK;

            sendto(sockfd, &ack_pkt, sizeof(ack_pkt), 0, (struct sockaddr *)&client_addr, client_len);

            printf("ACK sent to client\n");

            packet_t data_pkt;

            recvfrom(sockfd, &data_pkt, sizeof(data_pkt), 0, (struct sockaddr *)&client_addr, &client_len);

            if (data_pkt.opcode == OPCODE_DATA)
            {
                FILE *fp = fopen("uploaded.txt", "w");

                if (fp != NULL)
                {
                    fprintf(fp, "%s", data_pkt.data);
                    fclose(fp);

                    printf("File saved as uploaded.txt\n");
                }

                ack_pkt.opcode = OPCODE_ACK;

                sendto(sockfd, &ack_pkt, sizeof(ack_pkt), 0, (struct sockaddr *)&client_addr, client_len);

                printf("ACK sent for DATA packet\n");
            }
        }
    }

    close(sockfd);

    return 0;
}