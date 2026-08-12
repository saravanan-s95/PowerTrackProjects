
#include "client.h"

void *receiver_thread(void *arg)
{
    int sockfd;

    MESSAGE msg;

    sockfd = *(int *)arg;

    while(1)
    {
        memset(&msg, 0, sizeof(MESSAGE));

        if(recv(sockfd,
                &msg,
                sizeof(MESSAGE),
                0) <= 0)
        {
            printf("\nServer Disconnected\n");

            close(sockfd);

            pthread_exit(NULL);
        }

        switch(msg.option)
        {
            case REGISTER:

                printf("\n%s\n", msg.message);

                break;

            case LOGIN:

                printf("\n%s\n", msg.message);

                break;

            case SINGLE_CHAT:

                printf("\n----------------------------------\n");
                printf("Private Message\n");
                printf("From : %s\n", msg.sender);
                printf("Message : %s\n", msg.message);
                printf("----------------------------------\n");

                break;

            case GROUP_CHAT:

                printf("\n----------------------------------\n");
                printf("Group Message\n");
                printf("From : %s\n", msg.sender);
                printf("Message : %s\n", msg.message);
                printf("----------------------------------\n");

                break;

            case ONLINE_USERS:

                printf("\n----------------------------------\n");
                printf("Online Users\n");
                printf("----------------------------------\n");

                printf("%s", msg.message);

                printf("----------------------------------\n");

                break;

            case LOGOUT:

                printf("\n%s\n", msg.message);

                /*
                 * Do NOT exit the thread here.
                 * The socket connection stays open after logout, and the
                 * user may log in again in the same session — this thread
                 * needs to keep listening for that session's messages too.
                 * It only ends when recv() fails (socket closed) or the
                 * main thread cancels it on program exit.
                 */

                break;

            default:

                printf("\nUnknown message received from server\n");

                break;
        }
    }

    return NULL;
}