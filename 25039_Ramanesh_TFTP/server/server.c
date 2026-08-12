

#include "tftp.h"

void handle_client(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *buffer);

int main(void)
{
    int sockfd;
    int n;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    socklen_t client_len = sizeof(client_addr);

    struct timeval tv;

    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd < 0)
    {
        perror("socket");
        return 1;
    }

    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;

    if(setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return 1;
    }

    memset(&server_addr,0,sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(sockfd);
        return 1;
    }

    printf("========================================\n");
    printf("      TFTP SERVER STARTED\n");
    printf("Listening On Port %d\n",PORT);
    printf("========================================\n");

    while(1)
    {
        memset(buffer,0,sizeof(buffer));

        n = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client_addr,&client_len);

        if(n < 0)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                continue;

            perror("recvfrom");
            continue;
        }

        handle_client(sockfd,client_addr,client_len,buffer);
    }

    close(sockfd);

    return 0;
}

void handle_client(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *buffer)
{
    uint16_t opcode;

    char filename[256];
    char mode[32];

    char *ptr;

    FILE *fp;

    memcpy(&opcode,buffer,sizeof(uint16_t));

    opcode = ntohs(opcode);

    ptr = buffer + 2;

    strcpy(filename,ptr);

    ptr += strlen(filename) + 1;

    strcpy(mode,ptr);

    printf("\n----------------------------------------\n");
    printf("Request Received\n");
    printf("Filename : %s\n",filename);
    printf("Mode     : %s\n",mode);
    printf("----------------------------------------\n");

    if(opcode == RRQ)
    {
        fp = fopen(filename,"rb");

        if(fp == NULL)
        {
            char err[BUFFER_SIZE];
            uint16_t err_opcode = htons(ERROR);
            uint16_t err_code = htons(1);

            memset(err,0,sizeof(err));

            memcpy(err,&err_opcode,2);
            memcpy(err + 2,&err_code,2);

            strcpy(err + 4,"File Not Found");

            sendto(sockfd,err,4 + strlen(err + 4) + 1,0,(struct sockaddr *)&client_addr,client_len);

            printf("ERROR : File Not Found\n");

            return;
        }

        fclose(fp);

        printf("Sending File...\n");

        send_file(sockfd,client_addr,client_len,filename,get_mode_value(mode));
    }

    else if(opcode == WRQ)
    {
        char ack[4];

        uint16_t ack_opcode = htons(ACK);
        uint16_t block = htons(0);

        fp = fopen(filename,"wb");

        if(fp == NULL)
        {
            perror("fopen");
            return;
        }

        fclose(fp);

        memcpy(ack,&ack_opcode,2);
        memcpy(ack + 2,&block,2);

        if(sendto(sockfd,ack,sizeof(ack),0,(struct sockaddr *)&client_addr,client_len) < 0)
        {
            perror("sendto");
            return;
        }

        printf("ACK 0 Sent\n");

        receive_file(sockfd,client_addr,client_len,filename,get_mode_value(mode));
    }

    else
    {
        printf("Unknown Opcode : %u\n",opcode);
    }
}