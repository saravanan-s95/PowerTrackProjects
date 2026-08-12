
#include "server.h"

//  single_chat
//  Sends a message to a particular online user

void single_chat(int clientfd, MESSAGE *msg)
{
    int receiverfd;

    MESSAGE response;

    memset(&response, 0, sizeof(MESSAGE));

//   Find receiver socket descriptor

    receiverfd = get_socketfd(msg->receiver);

    if(receiverfd < 0)
    {
        response.option = SINGLE_CHAT;
        response.status = FAILURE;

        strcpy(response.sender,
               msg->sender);

        strcpy(response.receiver,
               msg->receiver);

        strcpy(response.message,
               "Receiver is not online");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("Receiver %s is not online\n",
               msg->receiver);

        return;
    }

//   Send message to receiver

    send(receiverfd,
         msg,
         sizeof(MESSAGE),
         0);

    printf("\n----------------------------------\n");
    printf("SINGLE CHAT\n");
    printf("From    : %s\n", msg->sender);
    printf("To      : %s\n", msg->receiver);
    printf("Message : %s\n", msg->message);
    printf("----------------------------------\n");

//  Send confirmation to sender

    response.option = SINGLE_CHAT;
    response.status = SUCCESS;

    strcpy(response.sender,
           msg->sender);

    strcpy(response.receiver,
           msg->receiver);

    strcpy(response.message,
           "Message Sent");

    send(clientfd,
         &response,
         sizeof(MESSAGE),
         0);
}


//  group_chat
// Description : Sends message to all online users

void group_chat(int clientfd, MESSAGE *msg)
{
    int fd;

    DATABASE user;

    MESSAGE response;

    memset(&response, 0, sizeof(MESSAGE));

    fd = open(DATABASE_FILE, O_RDONLY);

    if(fd < 0)
    {
        perror("open");

        response.option = GROUP_CHAT;
        response.status = FAILURE;

        strcpy(response.message,
               "Unable to access database");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        return;
    }

//      Read all users from database

    while(read(fd,
               &user,
               sizeof(DATABASE)) > 0)
    {
        
       //   Send only to online users
         

        if(user.status == ONLINE &&
           user.socketfd != clientfd)
        {
            send(user.socketfd,
                 msg,
                 sizeof(MESSAGE),
                 0);
        }
    }

    close(fd);

    printf("\n----------------------------------\n");
    printf("GROUP CHAT\n");
    printf("From    : %s\n", msg->sender);
    printf("Message : %s\n", msg->message);
    printf("----------------------------------\n");

    
//      Confirmation to sender


    response.option = GROUP_CHAT;
    response.status = SUCCESS;

    strcpy(response.sender,
           msg->sender);

    strcpy(response.message,
           "Group Message Sent");

    send(clientfd,
         &response,
         sizeof(MESSAGE),
         0);
}