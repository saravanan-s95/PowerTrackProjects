#include "tftp.h" // Common TFTP definitions

void handle_client(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *buffer); // Handle request

int main()
{
    int sockfd; // Server socket
    int n; // Received bytes
    struct sockaddr_in server_addr; // Server address
    struct sockaddr_in client_addr; // Client address
    socklen_t client_len = sizeof(client_addr); // Client length
    struct timeval tv; // Timeout structure
    char buffer[BUFFER_SIZE]; // Receive buffer

    sockfd = socket(AF_INET,SOCK_DGRAM,0); // Create UDP socket

    if(sockfd < 0)
    {
        perror("socket"); // Socket creation failed
        return 1;
    }

    tv.tv_sec = TIMEOUT_SEC; // Timeout seconds
    tv.tv_usec = 0; // Timeout microseconds

    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv)); // Set timeout

    memset(&server_addr,0,sizeof(server_addr)); // Clear structure

    server_addr.sin_family = AF_INET; // IPv4 family
    server_addr.sin_port = htons(PORT); // Server port
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
    {
        perror("bind"); // Bind failed
        close(sockfd); // Close socket
        return 1;
    }

    printf("========================================\n"); // Border
    printf("TFTP Server Listening On Port %d\n",PORT); // Status
    printf("========================================\n"); // Border

    while(1)
    {
        memset(buffer,0,sizeof(buffer)); // Clear buffer

        n = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client_addr,&client_len); // Receive request

        if(n < 0)
        {
            perror("recvfrom"); // Receive failed
            continue;
        }

        handle_client(sockfd,client_addr,client_len,buffer); // Process request
    }

    close(sockfd); // Close socket

    return 0; // Program success
}

void handle_client(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *buffer)
{
    uint16_t opcode; // Packet opcode
    char filename[256]; // Filename
    FILE *fp; // File pointer

    opcode = ntohs(*(uint16_t *)buffer); // Extract opcode

    strcpy(filename,buffer + 2); // Extract filename

    printf("\nRequest Received For File : %s\n",filename); // Display filename

    if(opcode == RRQ)
    {
        printf("RRQ Request\n"); // Read request

        fp = fopen(filename,"rb"); // Open file

        if(fp == NULL)
        {
            char err[BUFFER_SIZE]; // Error packet

            printf("File Not Found\n"); // Error message

            memset(err,0,sizeof(err)); // Clear packet

            *(uint16_t *)err = htons(ERROR); // Error opcode

            *(uint16_t *)(err + 2) = htons(1); // Error code

            strcpy(err + 4,"File Not Found"); // Error text

            sendto(sockfd,err,strlen(err + 4) + 4,0,(struct sockaddr *)&client_addr,client_len); // Send error

            return;
        }

        fclose(fp); // Close file

        printf("Starting Download : %s\n",filename); // Status

        send_file(sockfd,client_addr,client_len,filename,MODE_NORMAL); // Send file
    }
    else if(opcode == WRQ)
    {
        char ack[4]; // ACK packet

        printf("WRQ Request\n"); // Write request

        fp = fopen(filename,"wb"); // Create file

        if(fp == NULL)
        {
            printf("File Creation Failed\n"); // Error
            return;
        }

        fclose(fp); // Close file

        *(uint16_t *)ack = htons(ACK); // ACK opcode

        *(uint16_t *)(ack + 2) = htons(0); // Block number 0

        sendto(sockfd,ack,4,0,(struct sockaddr *)&client_addr,client_len); // Send ACK

        printf("ACK 0 Sent\n"); // Status

        receive_file(sockfd,client_addr,client_len,filename,MODE_NORMAL); // Receive file
    }
    else
    {
        printf("Unknown Opcode : %u\n",opcode); // Invalid request
    }
}