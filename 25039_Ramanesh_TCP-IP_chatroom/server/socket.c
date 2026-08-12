#include "server.h"

static struct sockaddr_in server_addr;

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

void bind_socket(int sockfd)
{
    int opt = 1;

    if(setsockopt(sockfd,
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  &opt,
                  sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Bind Successful\n");
}

void listen_socket(int sockfd)
{
    if(listen(sockfd, MAX_CLIENTS) < 0)
    {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server Listening...\n");
}

int accept_client(int sockfd)
{
    int clientfd;

    struct sockaddr_in client_addr;

    socklen_t addr_len = sizeof(client_addr);

    clientfd = accept(sockfd,
                      (struct sockaddr *)&client_addr,
                      &addr_len);

    if(clientfd < 0)
    {
        perror("accept");
        return -1;
    }

    printf("----------------------------------\n");
    printf("Client Connected\n");
    printf("IP Address : %s\n",inet_ntoa(client_addr.sin_addr));

    printf("Port       : %d\n",ntohs(client_addr.sin_port));

    printf("----------------------------------\n");

    return clientfd;
}