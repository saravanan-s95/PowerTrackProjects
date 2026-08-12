// Name : Ramanesh M
// Batch : 25039_003
// Date Of completion : 13/05/2026

#include "tftp_client.h"

int validate_ip(const char *ip)
{
    struct sockaddr_in sa;

    return inet_pton(AF_INET,ip,&sa.sin_addr);
}

void process_command(tftp_client_t *client,int choice)
{
    switch(choice)
    {
        case 1:
            connect_to_server(client);
            break;

        case 2:
            put_file_client(client);
            break;

        case 3:
            get_file_client(client);
            break;

        case 4:
        {
            int mode;

            printf("\n1. Normal (512 Bytes)\n");
            printf("2. Octet (1 Byte)\n");
            printf("3. NetASCII\n");
            printf("Enter Mode : ");

            scanf("%d",&mode);
            getchar();

            if(mode >= MODE_NORMAL && mode <= MODE_NETASCII)
            {
                client->mode = mode;
                printf("Mode Changed To : %s\n",get_mode_string(mode));
            }
            else
            {
                printf("Invalid Mode\n");
            }

            break;
        }

        case 5:
            disconnect_client(client);
            break;

        default:
            printf("Invalid Choice\n");
    }
}

void connect_to_server(tftp_client_t *client)
{
    char ip[INET_ADDRSTRLEN];
    struct timeval tv;

    printf("Enter Server IP : ");

    scanf("%15s",ip);
    getchar();

    if(!validate_ip(ip))
    {
        printf("Invalid IP Address\n");
        return;
    }

    if(client->connected)
    {
        close(client->sockfd);
        client->connected = 0;
    }

    client->sockfd = socket(AF_INET,SOCK_DGRAM,0);

    if(client->sockfd < 0)
    {
        perror("socket");
        return;
    }

    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = 0;

    if(setsockopt(client->sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
    {
        perror("setsockopt");
        close(client->sockfd);
        return;
    }

    memset(&client->server_addr,0,sizeof(client->server_addr));

    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port   = htons(PORT);

    if(inet_pton(AF_INET,ip,&client->server_addr.sin_addr) <= 0)
    {
        printf("Invalid IP Address\n");
        close(client->sockfd);
        return;
    }

    client->server_len = sizeof(client->server_addr);

    strncpy(client->server_ip,ip,sizeof(client->server_ip)-1);

    client->server_ip[sizeof(client->server_ip)-1] = '\0';

    client->connected = 1;

    printf("Connected To Server %s\n",client->server_ip);
}
void put_file_client(tftp_client_t *client)
{
    char filename[256];
    char buffer[BUFFER_SIZE];
    char ack[BUFFER_SIZE];
    char *ptr;
    FILE *fp;
    int n;
    uint16_t opcode;
    uint16_t block;

    if(!client->connected)
    {
        printf("Connect To Server First\n");
        return;
    }

#ifdef __linux__
    system("ls");
#else
    system("dir");
#endif

    printf("Enter Filename : ");
    scanf("%255s",filename);
    getchar();

    fp = fopen(filename,"rb");

    if(fp == NULL)
    {
        printf("File Not Found : %s\n",filename);
        return;
    }

    fclose(fp);

    memset(buffer,0,sizeof(buffer));

    opcode = htons(WRQ);

    memcpy(buffer,&opcode,sizeof(opcode));

    ptr = buffer + 2;

    strcpy(ptr,filename);

    ptr += strlen(filename) + 1;

    strcpy(ptr,get_mode_string(client->mode));

    ptr += strlen(ptr) + 1;

    if(sendto(client->sockfd,buffer,ptr - buffer,0,(struct sockaddr *)&client->server_addr,client->server_len) < 0)
    {
        perror("sendto");
        return;
    }

    printf("WRQ Sent For File : %s\n",filename);

    n = recvfrom(client->sockfd,ack,sizeof(ack),0,(struct sockaddr *)&client->server_addr,&client->server_len);

    if(n < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            printf("Timeout Waiting For ACK\n");
        else
            perror("recvfrom");

        return;
    }

    memcpy(&opcode,ack,2);
    opcode = ntohs(opcode);

    if(opcode == ERROR)
    {
        printf("Server Returned ERROR\n");
        return;
    }

    memcpy(&block,ack + 2,2);
    block = ntohs(block);

    if(opcode == ACK && block == 0)
    {
        printf("Received ACK 0\n");

        send_file(client->sockfd,client->server_addr,client->server_len,filename,client->mode);
    }
    else
    {
        printf("Invalid ACK Received\n");
    }
}

void get_file_client(tftp_client_t *client)
{
    char filename[256];
    char buffer[BUFFER_SIZE];
    char *ptr;
    uint16_t opcode;

    if(!client->connected)
    {
        printf("Connect To Server First\n");
        return;
    }

    printf("Enter Filename : ");

    scanf("%255s",filename);
    getchar();

    memset(buffer,0,sizeof(buffer));

    opcode = htons(RRQ);

    memcpy(buffer,&opcode,sizeof(opcode));

    ptr = buffer + 2;

    strcpy(ptr,filename);

    ptr += strlen(filename) + 1;

    strcpy(ptr,get_mode_string(client->mode));

    ptr += strlen(ptr) + 1;

    if(sendto(client->sockfd,buffer,ptr - buffer,0,(struct sockaddr *)&client->server_addr,client->server_len) < 0)
    {
        perror("sendto");
        return;
    }

    printf("RRQ Sent For File : %s\n",filename);

    receive_file(client->sockfd,client->server_addr,client->server_len,filename,client->mode);
}
void disconnect_client(tftp_client_t *client)
{
    if(client->connected)
    {
        close(client->sockfd);

        client->connected = 0;
    }

    printf("Disconnected From Server\n");
    printf("Exiting TFTP Client...\n");

    exit(EXIT_SUCCESS);
}

int main(void)
{
    tftp_client_t client;
    int choice;

    memset(&client,0,sizeof(client));

    client.mode = MODE_NORMAL;
    client.connected = 0;

    while(1)
    {
        printf("\n");
        printf("=========================================\n");
        printf("              TFTP CLIENT\n");
        printf("=========================================\n");
        printf("1. Connect To Server\n");
        printf("2. Put File (Upload)\n");
        printf("3. Get File (Download)\n");
        printf("4. Change Mode (Current : %s)\n",
                get_mode_string(client.mode));
        printf("5. Exit\n");
        printf("=========================================\n");
        printf("Enter Choice : ");

        if(scanf("%d",&choice) != 1)
        {
            while(getchar()!='\n');
            printf("Invalid Choice\n");
            continue;
        }

        getchar();

        process_command(&client,choice);
    }

    return 0;
}