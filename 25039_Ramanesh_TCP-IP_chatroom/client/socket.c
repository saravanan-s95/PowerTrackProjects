

#include "client.h"

//Creates a TCP socket for the client
 
int create_socket(void)
{
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Socket Created Successfully\n");

    return sockfd;
}


//Connects the client socket to the chat server
void connect_server(int sockfd)
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(PORT);

    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if(connect(sockfd,
               (struct sockaddr *)&server_addr,
               sizeof(server_addr)) < 0)
    {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to Server Successfully\n");
}
