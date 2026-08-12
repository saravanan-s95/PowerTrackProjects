#include "server.h"

int main()
{
    int serverfd;

    pthread_t tid;

    // Create Socket 
    serverfd = create_socket();

    // Bind Socket 
    bind_socket(serverfd);

    //Listen for Clients 
    listen_socket(serverfd);

    printf("\n=====================================\n");
    printf(" TCP/IP Chat Server Started\n");
    printf(" Listening on Port : %d\n", PORT);
    printf("=====================================\n");

    while(1)
    {
        int *pclientfd = malloc(sizeof(int));

        if(pclientfd == NULL)
        {
            perror("malloc");
            continue;
        }

        *pclientfd = accept_client(serverfd);

        if(*pclientfd < 0)
        {
            free(pclientfd);
            continue;
        }

        printf("Client Connected (Socket FD = %d)\n", *pclientfd);

        /*
         * Pass a heap-allocated pointer to the thread.
         * client_handler() takes ownership and frees it.
         * (Passing the address of a stack variable here would be
         * undefined behavior once main() loops around and reuses it.)
         */
        pthread_create(&tid,NULL,client_handler,(void *)pclientfd);

        pthread_detach(tid);
    }

    close(serverfd);

    return 0;
}