#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "file.h"
#include "udp.h"
#include "tftp.h"

int main(void)
{
    int sockfd = -1;
    int len;
    int retry;
    char command[256];
    char mode[32] = MODE_OCTET;

    packetbuffer_t *out_buf;
    packetbuffer_t rx_buf[PACKETSIZE];
    packet_t pkt;
    struct sockaddr_in server_addr;
    struct sockaddr_in data_addr;
    socklen_t addr_len;
    int connected = 0;

    printf("TFTP Client Started\n");

    while (1)
    {
        printf("tftp> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = '\0';

        if (!strcmp(command, "bye") || !strcmp(command, "quit")) break;

        else if (!strncmp(command, "connect", 7))
        {
            char ip[64];
            int port;

            if (sscanf(command, "connect %63s %d", ip, &port) != 2)
            {
                printf("Usage : connect <ip> <port>\n");
                continue;
            }

            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            inet_pton(AF_INET, ip, &server_addr.sin_addr);

            if (sockfd != -1) close(sockfd);
            udp_bind_client(&sockfd);

            struct timeval tv;
            tv.tv_sec = TIMEOUT;
            tv.tv_usec = 0;
            setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

            connected = 1;
            printf("Connected to %s:%d\n", ip, port);
        }
        else if (!strncmp(command, "mode", 4))
        {
            char temp[32];
            if (sscanf(command, "mode %31s", temp) != 1)
            {
                printf("Current Mode : %s\n", mode);
                continue;
            }

            if (!strcmp(temp, MODE_OCTET) || !strcmp(temp, MODE_NETASCII))
            {
                strcpy(mode, temp);
                printf("Mode changed to %s\n", mode);
            }
            else
            {
                printf("Unsupported mode\n");
            }
        }
        else if (!strncmp(command, "get", 3))
        {
            char filename[256];
            bnum_t expected_block = 1;

            if (!connected)
            {
                printf("Connect first\n");
                continue;
            }

            if (sscanf(command, "get %255s", filename) != 1)
            {
                printf("Usage : get <filename>\n");
                continue;
            }

            if (file_open_write(filename) < 0)
            {
                printf("Unable to create file\n");
                continue;
            }

            packet_form_rrq(filename, mode, &out_buf, &len);
            udp_send_packet(sockfd, &server_addr, out_buf, len);

            data_addr = server_addr;

            while (1)
            {
                addr_len = sizeof(data_addr);
                len = recvfrom(sockfd, rx_buf, PACKETSIZE, 0, (struct sockaddr *)&data_addr, &addr_len);

                if (len < 0)
                {
                    printf("Timeout or no response\n");
                    break;
                }

                if (packet_parse(rx_buf, len, &pkt) < 0) continue;

                if (IS_ERROR(pkt.opcode))
                {
                    printf("Server Error : %s\n", pkt.estring);
                    break;
                }

                if (IS_DATA(pkt.opcode))
                {
                    if (pkt.blocknum == expected_block)
                    {
                        file_append_from_buffer(pkt.data, pkt.data_length);
                        packet_form_ack(expected_block, &out_buf, &len);
                        udp_send_packet(sockfd, &data_addr, out_buf, len);

                        if (pkt.data_length < BLIMIT)
                        {
                            printf("Download Complete\n");
                            break;
                        }
                        expected_block++;
                    }
                    else if (pkt.blocknum < expected_block)
                    {
                        // Duplicate old block, re-ACK it
                        packet_form_ack(pkt.blocknum, &out_buf, &len);
                        udp_send_packet(sockfd, &data_addr, out_buf, len);
                    }
                }
            }
            file_close();
        }
        else if (!strncmp(command, "put", 3))
        {
            char filename[256];
            char file_buf[BLIMIT];
            int bytes_read;
            bnum_t block_no = 1;

            if (!connected)
            {
                printf("Connect first\n");
                continue;
            }

            if (sscanf(command, "put %255s", filename) != 1)
            {
                printf("Usage : put <filename>\n");
                continue;
            }

            if (file_open_read(filename) < 0)
            {
                printf("Unable to open file\n");
                continue;
            }

            packet_form_wrq(filename, mode, &out_buf, &len);
            udp_send_packet(sockfd, &server_addr, out_buf, len);

            addr_len = sizeof(data_addr);
            len = recvfrom(sockfd, rx_buf, PACKETSIZE, 0, (struct sockaddr *)&data_addr, &addr_len);

            if (len <= 0)
            {
                printf("No response from server\n");
                file_close();
                continue;
            }

            if (packet_parse(rx_buf, len, &pkt) < 0 || !IS_ACK(pkt.opcode))
            {
                printf("Error or unexpected response from server\n");
                file_close();
                continue;
            }

            while (1)
            {
                bytes_read = file_buffer_from_pos(file_buf, BLIMIT);
                if (bytes_read < 0) break;

                packet_form_data(block_no, file_buf, bytes_read, &out_buf, &len);
                retry = 0;

                while (retry < TIMEOUT_LIMIT)
                {
                    udp_send_packet(sockfd, &data_addr, out_buf, len);
                    addr_len = sizeof(data_addr);
                    len = recvfrom(sockfd, rx_buf, PACKETSIZE, 0, (struct sockaddr *)&data_addr, &addr_len);

                    if (len > 0)
                    {
                        if (packet_parse(rx_buf, len, &pkt) == 0)
                        {
                            if (IS_ACK(pkt.opcode) && pkt.blocknum == block_no)
                            {
                                break;
                            }
                        }
                    }
                    retry++;
                }

                if (retry == TIMEOUT_LIMIT)
                {
                    printf("Transfer Failed\n");
                    break;
                }

                if (bytes_read < BLIMIT)
                {
                    printf("Upload Complete\n");
                    break;
                }
                block_no++;
            }
            file_close();
        }
        else
        {
            printf("Unknown Command\nCommands:\n connect <ip> <port>\n get <file>\n put <file>\n mode <octet|netascii>\n bye\n");
        }
    }

    if (sockfd != -1) close(sockfd);
    return 0;
}