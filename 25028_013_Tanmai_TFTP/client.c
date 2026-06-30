#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "udp.h"
#include "tftp.h"

int sockfd;
struct sockaddr_in server_addr;
packet_t pkt;

/* Function Prototypes */
void connect_to_server();
void get_file();
void put_file();
void disconnect_server();

/* Client code for TFTP Protocol */

int main(int argc, char *argv[])
{
    int choice;

    while (1)
    {
        printf("\n===== TFTP CLIENT =====\n");
        printf("1. Connect\n");
        printf("2. Get File\n");
        printf("3. Put File\n");
        printf("4. Exit\n");
        printf("Enter choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            connect_to_server();
            break;

        case 2:
            get_file();
            break;

        case 3:
            put_file();
            break;

        case 4:
            disconnect_server();
            exit(0);

        default:
            printf("Invalid Choice\n");
        }
    }

    return 0;
}
void connect_to_server()
{
    char ip[20];
    int port;

    printf("Enter Server IP : ");
    scanf("%s", ip);

    printf("Enter Port : ");
    scanf("%d", &port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("socket");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    printf("Connected to %s:%d\n", ip, port);
}

void get_file()
{
    char filename[100];

    printf("Enter filename : ");
    scanf("%s", filename);

    memset(&pkt, 0, sizeof(pkt));

    pkt.opcode = OPCODE_RRQ;

    strcpy(pkt.filename, filename);

    strcpy(pkt.mode, MODE_OCTET);

    sendto(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("RRQ sent for file : %s\n", filename);

    packet_t ack_pkt;

    recvfrom(sockfd, &ack_pkt, sizeof(ack_pkt), 0, NULL, NULL);

    if (ack_pkt.opcode == OPCODE_ACK)
    {
        printf("ACK received from server\n");
    }

    FILE *fp;

    fp = fopen("received.txt", "wb");

    if (fp == NULL)
    {
        printf("Unable to create file\n");
        return;
    }

    while (1)
    {
        packet_t data_pkt;

        recvfrom(sockfd, &data_pkt, sizeof(data_pkt), 0, NULL, NULL);

        if (data_pkt.opcode == OPCODE_ERROR)
        {
            printf("ERROR : %s\n", data_pkt.estring);

            fclose(fp);
            return;
        }

        if (data_pkt.opcode == OPCODE_DATA)
        {
            printf("DATA Block %d received (%d bytes)\n", data_pkt.blocknum, data_pkt.data_length);

            fwrite(data_pkt.data, 1, data_pkt.data_length, fp);

            memset(&ack_pkt, 0, sizeof(ack_pkt));

            ack_pkt.opcode = OPCODE_ACK;

            ack_pkt.blocknum = data_pkt.blocknum;

            sendto(sockfd, &ack_pkt, sizeof(ack_pkt), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

            printf("ACK Block %d sent\n", ack_pkt.blocknum);

            if (data_pkt.data_length < BLIMIT)
            {
                break;
            }
        }
    }

    fclose(fp);

    printf("\nFile saved as received.txt\n");
}

void put_file()
{
    char filename[100];

    printf("Enter filename : ");
    scanf("%s", filename);

    packet_t pkt;

    memset(&pkt, 0, sizeof(pkt));

    pkt.opcode = OPCODE_WRQ;

    strcpy(pkt.filename, filename);

    strcpy(pkt.mode, MODE_OCTET);

    sendto(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("WRQ sent for file : %s\n", filename);

    /* Receive ACK from server */
    packet_t ack_pkt;

    recvfrom(sockfd, &ack_pkt, sizeof(ack_pkt), 0, NULL, NULL);

    if (ack_pkt.opcode == OPCODE_ACK)
    {
        printf("ACK Block %d received\n", ack_pkt.blocknum);

        FILE *fp = fopen(filename, "r");

        if (fp != NULL)
        {
            packet_t data_pkt;

            memset(&data_pkt, 0, sizeof(data_pkt));

            data_pkt.opcode = OPCODE_DATA;

            fgets(data_pkt.data, sizeof(data_pkt.data), fp);

            sendto(sockfd, &data_pkt, sizeof(data_pkt), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

            printf("DATA sent to server\n");

            fclose(fp);
        }
    }
}

void disconnect_server()
{
    close(sockfd);
    printf("Disconnected from server\n");
}