#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "file.h"
#include "udp.h"
#include "tftp.h"

#define START_PORT 20000

static int next_port = START_PORT;

void sigchld_handler(int signo)
{
    (void)signo;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void process_rrq(int sockfd, struct sockaddr_in *client_addr, packet_t *pkt)
{
    packetbuffer_t *out_buf;
    packetbuffer_t in_buf[PACKETSIZE];
    packet_t rx_pkt;
    socklen_t addr_len;
    char file_buf[BLIMIT];
    int len;
    int bytes_read;
    int retry;
    bnum_t block_no = 1;

    if (file_open_read(pkt->filename) < 0)
    {
        packet_form_error(ECODE_1, ESTRING_1, &out_buf, &len);
        udp_send_packet(sockfd, client_addr, out_buf, len);
        return;
    }

    while (1)
    {
        bytes_read = file_buffer_from_pos(file_buf, BLIMIT);
        if (bytes_read < 0) break;

        packet_form_data(block_no, file_buf, bytes_read, &out_buf, &len);
        retry = 0;

        while (retry < TIMEOUT_LIMIT)
        {
            udp_send_packet(sockfd, client_addr, out_buf, len);
            addr_len = sizeof(*client_addr);
            len = recvfrom(sockfd, in_buf, PACKETSIZE, 0, (struct sockaddr *)client_addr, &addr_len);

            if (len > 0)
            {
                if (packet_parse(in_buf, len, &rx_pkt) == 0)
                {
                    if (IS_ACK(rx_pkt.opcode) && rx_pkt.blocknum == block_no)
                    {
                        break;
                    }
                }
            }
            retry++;
        }

        if (retry == TIMEOUT_LIMIT)
        {
            printf("Client timeout\n");
            break;
        }

        if (bytes_read < BLIMIT) break;
        block_no++;
    }

    file_close();
    printf("RRQ Transfer Complete : %s\n", pkt->filename);
}

void process_wrq(int sockfd, struct sockaddr_in *client_addr, packet_t *pkt)
{
    packetbuffer_t *out_buf;
    packetbuffer_t in_buf[PACKETSIZE];
    packet_t rx_pkt;
    socklen_t addr_len;
    int len;
    bnum_t expected_block = 1;

    if (access(pkt->filename, F_OK) == 0)
    {
        packet_form_error(ECODE_7, ESTRING_7, &out_buf, &len);
        udp_send_packet(sockfd, client_addr, out_buf, len);
        return;
    }

    if (file_open_write(pkt->filename) < 0)
    {
        packet_form_error(ECODE_2, ESTRING_2, &out_buf, &len);
        udp_send_packet(sockfd, client_addr, out_buf, len);
        return;
    }

    packet_form_ack(0, &out_buf, &len);
    udp_send_packet(sockfd, client_addr, out_buf, len);

    while (1)
    {
        addr_len = sizeof(*client_addr);
        len = recvfrom(sockfd, in_buf, PACKETSIZE, 0, (struct sockaddr *)client_addr, &addr_len);

        if (len < 0) break;

        if (packet_parse(in_buf, len, &rx_pkt) < 0) continue;

        if (IS_DATA(rx_pkt.opcode))
        {
            if (rx_pkt.blocknum == expected_block)
            {
                file_append_from_buffer(rx_pkt.data, rx_pkt.data_length);
                packet_form_ack(expected_block, &out_buf, &len);
                udp_send_packet(sockfd, client_addr, out_buf, len);

                if (rx_pkt.data_length < BLIMIT) break;
                expected_block++;
            }
        }
    }

    file_close();
    printf("WRQ Transfer Complete : %s\n", pkt->filename);
}

int main(int argc, char *argv[])
{
    int server_fd;
    int worker_fd;
    int server_port;
    int worker_port;
    int len;
    socklen_t client_len;
    pid_t pid;
    struct sockaddr_in client_addr;
    struct timeval tv;
    packetbuffer_t rx_buf[PACKETSIZE];
    packet_t pkt;
    struct sigaction sa;

    if (argc != 2)
    {
        fprintf(stderr, "Usage : %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    server_port = atoi(argv[1]);
    udp_bind_server(&server_fd, server_port);

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigchld_handler;
    sigaction(SIGCHLD, &sa, NULL);

    printf("=====================================\n");
    printf("      TFTP SERVER STARTED\n");
    printf("      PORT : %d\n", server_port);
    printf("=====================================\n");

    while (1)
    {
        client_len = sizeof(client_addr);
        len = recvfrom(server_fd, rx_buf, PACKETSIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        if (len < 0) continue;
        if (packet_parse(rx_buf, len, &pkt) < 0) continue;
        if (!(IS_RRQ(pkt.opcode) || IS_WRQ(pkt.opcode))) continue;

        worker_port = next_port++;
        if (next_port > 30000) next_port = START_PORT;

        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            close(server_fd);
            udp_rebind_server(&worker_fd, worker_port);

            tv.tv_sec = TIMEOUT;
            tv.tv_usec = 0;
            setsockopt(worker_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

            printf("\n-------------------------------------\nChild Created\nPort : %d\n", worker_port);

            if (IS_RRQ(pkt.opcode))
            {
                printf("RRQ : %s\n", pkt.filename);
                process_rrq(worker_fd, &client_addr, &pkt);
            }
            else if (IS_WRQ(pkt.opcode))
            {
                printf("WRQ : %s\n", pkt.filename);
                process_wrq(worker_fd, &client_addr, &pkt);
            }

            close(worker_fd);
            exit(EXIT_SUCCESS);
        }
    }

    close(server_fd);
    return 0;
}