#include "tftp.h"

const char *get_mode_string(int mode)
{
    switch(mode)
    {
        case MODE_NORMAL:
            return "Normal";

        case MODE_OCTET:
            return "octet";

        case MODE_NETASCII:
            return "netascii";

        default:
            return "octet";
    }
}

int get_mode_value( const char *mode)
{
    if(strcmp(mode,"octet") == 0)
        return MODE_OCTET;

    if(strcmp(mode,"netascii") == 0)
        return MODE_NETASCII;

    return MODE_NORMAL;
}

void send_file(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *filename,int mode)
{
    FILE *fp;

    char buffer[BUFFER_SIZE];
    char ack_buf[BUFFER_SIZE];
    char temp[DATA_SIZE];

    uint16_t opcode;
    uint16_t block = 1;
    uint16_t ack_block;

    size_t bytes_read;

    int retries;
    int n;
    int i;
    int j;

    fp = fopen(filename,"rb");

    if(fp == NULL)
    {
        perror("fopen");
        return;
    }

    printf("\nStarting File Transfer : %s\n",filename);

    while(1)
    {
        memset(buffer,0,sizeof(buffer));

        opcode = htons(DATA);

        memcpy(buffer,&opcode,2);

        opcode = htons(block);

        memcpy(buffer + 2,&opcode,2);

        if(mode == MODE_OCTET)
        {
            bytes_read = fread(buffer + 4,1,1,fp);
        }
        else if(mode == MODE_NETASCII)
        {
            bytes_read = fread(temp,1,DATA_SIZE,fp);

            j = 0;

            for(i = 0; i < (int)bytes_read; i++)
            {
                if(temp[i] == '\n')
                {
                    buffer[4 + j++] = '\r';
                }

                buffer[4 + j++] = temp[i];
            }

            bytes_read = j;
        }
        else
        {
            bytes_read = fread(buffer + 4,1,DATA_SIZE,fp);
        }

        retries = 0;
                while(retries < MAX_RETRIES)
        {
            if(sendto(sockfd,buffer,bytes_read + 4,0,(struct sockaddr *)&client_addr,client_len) < 0)
            {
                perror("sendto");
                fclose(fp);
                return;
            }

            printf("Sent DATA Block %u (%zu Bytes)\n",block,bytes_read);

            n = recvfrom(sockfd,ack_buf,sizeof(ack_buf),0,(struct sockaddr *)&client_addr,&client_len);

            if(n < 0)
            {
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    retries++;

                    printf("Timeout Waiting For ACK %u ""(Retry %d/%d)\n",block,retries,MAX_RETRIES);

                    continue;
                }

                perror("recvfrom");

                fclose(fp);

                return;
            }

            memcpy(&opcode,ack_buf,2);

            opcode = ntohs(opcode);

            if(opcode == ERROR)
            {
                printf("Received ERROR Packet\n");

                fclose(fp);

                return;
            }

            if(opcode != ACK)
            {
                printf("Unexpected Packet Received\n");

                continue;
            }

            memcpy(&ack_block,ack_buf + 2,sizeof(uint16_t));

            ack_block = ntohs(ack_block);

            if(ack_block == block)
            {
                printf("Received ACK %u\n",ack_block);

                break;
            }

            if(ack_block < block)
            {
                printf("Duplicate ACK %u Ignored\n",ack_block);

                continue;
            }

            printf("Invalid ACK Block %u\n",ack_block);
        }

        if(retries == MAX_RETRIES)
        {
            printf("\nTransfer Failed\n");
            printf("Maximum Retries Reached\n");

            fclose(fp);

            return;
        }

        if(bytes_read < DATA_SIZE)
        {
            printf("\nLast DATA Packet Sent\n");

            break;
        }

        block++;
    }

    printf("\nFile Transfer Completed Successfully\n");

    fclose(fp);
}
void receive_file(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *filename,int mode)
{
    FILE *fp;

    char buffer[BUFFER_SIZE];
    char ack[4];
    char temp[DATA_SIZE];

    uint16_t opcode;
    uint16_t block;
    uint16_t ack_opcode;
    uint16_t ack_block;

    uint16_t expected_block = 1;

    int retries = 0;
    int n;
    int i;
    int j;

    fp = fopen(filename,"wb");

    if(fp == NULL)
    {
        perror("fopen");
        return;
    }

    printf("\nReceiving File : %s\n",filename);

    while(1)
    {
        n = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client_addr,&client_len);

        if(n < 0)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                retries++;

                if(retries >= MAX_RETRIES)
                {
                    printf("Receive Timeout\n");
                    printf("Maximum Retries Reached\n");

                    fclose(fp);

                    return;
                }

                printf("Waiting For DATA Block %u ""(Retry %d/%d)\n",expected_block,retries,MAX_RETRIES);

                continue;
            }

            perror("recvfrom");

            fclose(fp);

            return;
        }

        retries = 0;

        memcpy(&opcode,buffer,sizeof(uint16_t));

        opcode = ntohs(opcode);

        if(opcode == ERROR)
        {
            printf("Received ERROR Packet\n");

            fclose(fp);

            return;
        }

        if(opcode != DATA)
        {
            printf("Unexpected Packet Received\n");

            continue;
        }

        memcpy(&block,buffer + 2,sizeof(uint16_t));

        block = ntohs(block);

        if(block < expected_block)
        {
            printf("Duplicate DATA Block %u\n",block);

            ack_opcode = htons(ACK);
            ack_block = htons(block);

            memcpy(ack,&ack_opcode,2);
            memcpy(ack + 2,&ack_block,2);

            sendto(sockfd,ack,sizeof(ack),0,(struct sockaddr *)&client_addr,client_len);

            continue;
        }

        if(block != expected_block)
        {
            printf("Out Of Order DATA Block %u\n",block);

            continue;
        }

        if(mode == MODE_NETASCII)
        {
            j = 0;

            for(i = 4; i < n; i++)
            {
                if(buffer[i] == '\r' &&
                   (i + 1) < n &&
                   buffer[i + 1] == '\n')
                {
                    temp[j++] = '\n';
                    i++;
                }
                else
                {
                    temp[j++] = buffer[i];
                }
            }

            fwrite(temp,1,j,fp);
        }
        else
        {
            fwrite(buffer + 4,1,n - 4,fp);
        }

        ack_opcode = htons(ACK);
        ack_block = htons(block);

        memcpy(ack,&ack_opcode,2);

        memcpy(ack + 2,&ack_block,2);

        if(sendto(sockfd,ack,sizeof(ack),0,(struct sockaddr *)&client_addr,client_len) < 0)
        {
            perror("sendto");

            fclose(fp);

            return;
        }

        printf("Received DATA Block %u\n",block);

        expected_block++;
        if((n - 4) < DATA_SIZE)
        {
            printf("\nLast DATA Block Received\n");
            break;
        }
    }

    printf("\nFile Received Successfully\n");

    fclose(fp);
}