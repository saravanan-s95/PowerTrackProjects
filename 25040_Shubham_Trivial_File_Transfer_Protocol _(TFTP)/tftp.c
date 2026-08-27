/* Common file for server & client */

#include "tftp.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

static void set_socket_timeout(int sockfd)
{
    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

static int send_error(int sockfd, const struct sockaddr_in *addr,
                      socklen_t addr_len, uint16_t code, const char *message)
{
    unsigned char packet[BUFFER_SIZE];
    uint16_t op = htons(ERROR);
    uint16_t ec = htons(code);
    size_t msg_len = strlen(message);

    if (msg_len > BUFFER_SIZE - 5)
        msg_len = BUFFER_SIZE - 5;

    memcpy(packet, &op, 2);
    memcpy(packet + 2, &ec, 2);
    memcpy(packet + 4, message, msg_len);
    packet[4 + msg_len] = '\0';

    return (int)sendto(sockfd, packet, 5 + msg_len, 0,
                       (const struct sockaddr *)addr, addr_len);
}

static int receive_ack(int sockfd, struct sockaddr_in *addr,
                       socklen_t *addr_len, uint16_t expected_block)
{
    unsigned char packet[BUFFER_SIZE];
    struct sockaddr_in from;
    socklen_t from_len = sizeof(from);

    for (;;) {
        ssize_t n = recvfrom(sockfd, packet, sizeof(packet), 0,
                             (struct sockaddr *)&from, &from_len);

        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return -2;
            return -1;
        }

        if (n < 4)
            continue;

        uint16_t op, block;
        memcpy(&op, packet, 2);
        memcpy(&block, packet + 2, 2);
        op = ntohs(op);
        block = ntohs(block);

        if (op == ERROR) {
            fprintf(stderr, "TFTP ERROR: %s\n",
                    n > 4 ? (char *)(packet + 4) : "Unknown error");
            return -1;
        }

        if (op == ACK && block == expected_block) {
            *addr = from;
            *addr_len = from_len;
            return 0;
        }
    }
}

void send_file(int sockfd, struct sockaddr_in client_addr,
               socklen_t client_len, char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        send_error(sockfd, &client_addr, client_len, 1, "File not found");
        perror("fopen");
        return;
    }

    set_socket_timeout(sockfd);

    unsigned char packet[BUFFER_SIZE];
    unsigned char ack[BUFFER_SIZE];
    uint16_t block = 1;

    for (;;) {
        size_t bytes_read = fread(packet + 4, 1, DATA_SIZE, fp);

        if (ferror(fp)) {
            send_error(sockfd, &client_addr, client_len, 0, "File read error");
            fclose(fp);
            return;
        }

        uint16_t op = htons(DATA);
        uint16_t block_net = htons(block);
        memcpy(packet, &op, 2);
        memcpy(packet + 2, &block_net, 2);

        size_t packet_len = bytes_read + 4;
        int acknowledged = 0;

        for (int retry = 0; retry < MAX_RETRIES; ++retry) {
            if (sendto(sockfd, packet, packet_len, 0,
                       (struct sockaddr *)&client_addr, client_len) < 0) {
                perror("sendto");
                fclose(fp);
                return;
            }

            struct sockaddr_in from;
            socklen_t from_len = sizeof(from);
            ssize_t n = recvfrom(sockfd, ack, sizeof(ack), 0,
                                 (struct sockaddr *)&from, &from_len);

            if (n >= 4) {
                uint16_t ack_op, ack_block;
                memcpy(&ack_op, ack, 2);
                memcpy(&ack_block, ack + 2, 2);
                ack_op = ntohs(ack_op);
                ack_block = ntohs(ack_block);

                if (ack_op == ACK && ack_block == block) {
                    acknowledged = 1;
                    client_addr = from;
                    client_len = from_len;
                    break;
                }
            } else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recvfrom");
                fclose(fp);
                return;
            }
        }

        if (!acknowledged) {
            fprintf(stderr, "Timeout waiting for ACK block %u\n", block);
            fclose(fp);
            return;
        }

        if (bytes_read < DATA_SIZE)
            break;

        block++;
        if (block == 0) /* TFTP block number wraps after 65535 */
            block = 1;
    }

    fclose(fp);
    printf("Sent file: %s\n", filename);
}

void receive_file(int sockfd, struct sockaddr_in client_addr,
                  socklen_t client_len, char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        send_error(sockfd, &client_addr, client_len, 2, "Access violation");
        perror("fopen");
        return;
    }

    set_socket_timeout(sockfd);

    unsigned char packet[BUFFER_SIZE];
    uint16_t expected_block = 1;

    uint16_t ack_op = htons(ACK);
    uint16_t zero = htons(0);
    unsigned char ack[4];
    memcpy(ack, &ack_op, 2);
    memcpy(ack + 2, &zero, 2);

    if (sendto(sockfd, ack, sizeof(ack), 0,
               (struct sockaddr *)&client_addr, client_len) < 0) {
        perror("sendto");
        fclose(fp);
        return;
    }

    for (;;) {
        int received = 0;

        for (int retry = 0; retry < MAX_RETRIES && !received; ++retry) {
            struct sockaddr_in from;
            socklen_t from_len = sizeof(from);

            ssize_t n = recvfrom(sockfd, packet, sizeof(packet), 0,
                                 (struct sockaddr *)&from, &from_len);

            if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    sendto(sockfd, ack, sizeof(ack), 0,
                           (struct sockaddr *)&client_addr, client_len);
                    continue;
                }

                perror("recvfrom");
                fclose(fp);
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
                fprintf(stderr, "Client TFTP ERROR: %s\n",
                        n > 4 ? (char *)(packet + 4) : "Unknown error");
                fclose(fp);
                return;
            }

            if (op != DATA)
                continue;

            if (block == expected_block) {
                size_t data_len = (size_t)n - 4;

                if (fwrite(packet + 4, 1, data_len, fp) != data_len) {
                    send_error(sockfd, &client_addr, client_len, 0,
                               "File write error");
                    fclose(fp);
                    return;
                }

                uint16_t current_ack = htons(block);
                memcpy(ack + 2, &current_ack, 2);

                if (sendto(sockfd, ack, sizeof(ack), 0,
                           (struct sockaddr *)&from, from_len) < 0) {
                    perror("sendto");
                    fclose(fp);
                    return;
                }

                client_addr = from;
                client_len = from_len;
                received = 1;

                if (data_len < DATA_SIZE) {
                    fclose(fp);
                    printf("Received file: %s\n", filename);
                    return;
                }

                expected_block++;
                if (expected_block == 0)
                    expected_block = 1;
            } else if (block < expected_block) {
                /* Duplicate DATA: resend the last ACK. */
                uint16_t duplicate_ack = htons(block);
                memcpy(ack + 2, &duplicate_ack, 2);
                sendto(sockfd, ack, sizeof(ack), 0,
                       (struct sockaddr *)&from, from_len);
            }
        }

        if (!received) {
            fprintf(stderr, "Timeout waiting for DATA block %u\n",
                    expected_block);
            fclose(fp);
            return;
        }
    }
}
