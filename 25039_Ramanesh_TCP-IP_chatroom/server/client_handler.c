

#include "server.h"


//  client_handler
//  Thread function for each connected client

void *client_handler(void *arg)
{
    int clientfd;

    MESSAGE msg;

    clientfd = *(int *)arg;

    //   Free memory allocated by server.c

    free(arg);

    while(1)
    {
        memset(&msg, 0, sizeof(MESSAGE));

        //  Receive request from client
        if(recv(clientfd,
                &msg,
                sizeof(MESSAGE),
                0) <= 0)
        {
            printf("\nClient Disconnected\n");
            printf("Socket FD : %d\n", clientfd);

            close(clientfd);

            pthread_exit(NULL);
        }

        printf("\n----------------------------------\n");
        printf("Request Received\n");
        printf("Option : %d\n", msg.option);
        printf("----------------------------------\n");

        switch(msg.option)
        {
            case REGISTER:

                printf("REGISTER Request\n");

                register_user(clientfd,&msg);

                break;


            case LOGIN:

                printf("LOGIN Request\n");

                login_user(clientfd,&msg);

                break;


            case SINGLE_CHAT:

                printf("SINGLE CHAT Request\n");

                single_chat(clientfd,&msg);

                break;


            case GROUP_CHAT:

                printf("GROUP CHAT Request\n");

                group_chat(clientfd,&msg);

                break;


            case LOGOUT:

                printf("LOGOUT Request\n");

                logout_user(clientfd,&msg);

                /*
                 * Do NOT close the connection or exit the thread here.
                 * The client's menu allows logging in again on the same
                 * TCP connection after a logout — this thread must keep
                 * servicing that connection. It only ends when recv()
                 * fails (client actually disconnects).
                 */

                break;


            default:

                printf("Invalid Request\n");

                break;
        }
    }

    return NULL;
}