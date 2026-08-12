
// Name : Ramanesh M
// Batch no : 25039_003
// Date of completion : 11/07/26


#include "client.h"

char logged_username[USERNAME_SIZE];


//  register_user
//  Sends registration request to server
 
void register_user(int sockfd)
{
    MESSAGE msg;

    memset(&msg, 0, sizeof(MESSAGE));

    printf("\n========== REGISTER ==========\n");

    printf("Enter Username : ");
    scanf("%29s", msg.sender);

    printf("Enter Password : ");
    scanf("%29s", msg.password);

    msg.option = REGISTER;

    send(sockfd,
         &msg,
         sizeof(MESSAGE),
         0);
}



//   login_user
//   Sends login request to server
//   1 on successful request
 
int login_user(int sockfd)
{
    MESSAGE msg;

    memset(&msg, 0, sizeof(MESSAGE));

    printf("\n============ LOGIN ============\n");

    printf("Enter Username : ");
    scanf("%29s", msg.sender);

    printf("Enter Password : ");
    scanf("%29s", msg.password);

    msg.option = LOGIN;

    send(sockfd,
         &msg,
         sizeof(MESSAGE),
         0);

    //  Store username locally.
    //  It will be used for all chat messages.
    
    strcpy(logged_username,
           msg.sender);

    return 1;
}

 //single_chat
 //Sends a private message
 
void single_chat(int sockfd)
{
    MESSAGE msg;

    memset(&msg, 0, sizeof(MESSAGE));

    msg.option = SINGLE_CHAT;


     //Sender is the currently logged-in user.
     
    strcpy(msg.sender,
           logged_username);

    printf("\n========== SINGLE CHAT ==========\n");

    printf("Enter Receiver : ");
    scanf("%29s", msg.receiver);

    printf("Enter Message : ");

    getchar();

    fgets(msg.message,
          BUFFER_SIZE,
          stdin);

    msg.message[strcspn(msg.message, "\n")] = '\0';

    send(sockfd,
         &msg,
         sizeof(MESSAGE),
         0);
}

//group_chat
//Sends message to all online users

void group_chat(int sockfd)
{
    MESSAGE msg;

    memset(&msg, 0, sizeof(MESSAGE));

    msg.option = GROUP_CHAT;

    // Sender is the currently logged-in user.
    
    strcpy(msg.sender,
           logged_username);

    printf("\n========== GROUP CHAT ==========\n");

    printf("Enter Message : ");

    getchar();

    fgets(msg.message,
          BUFFER_SIZE,
          stdin);

    msg.message[strcspn(msg.message, "\n")] = '\0';

    send(sockfd,
         &msg,
         sizeof(MESSAGE),
         0);
}


//  logout_user
//  Sends logout request to server

void logout_user(int sockfd)
{
    MESSAGE msg;

    memset(&msg, 0, sizeof(MESSAGE));

    msg.option = LOGOUT;

    strcpy(msg.sender,
           logged_username);

    send(sockfd,
         &msg,
         sizeof(MESSAGE),
         0);

    memset(logged_username,
           0,
           sizeof(logged_username));
}


/*---------------------------------------------------------------------------
 * Function    : chat_menu
 * Description : Displays chat menu
 *-------------------------------------------------------------------------*/
void chat_menu(int sockfd)
{
    int choice;

    while(1)
    {
        printf("\n");
        printf("=================================\n");
        printf("            CHAT MENU\n");
        printf("=================================\n");

        printf("1. Single Chat\n");
        printf("2. Group Chat\n");
        printf("3. Logout\n");

        printf("Enter Choice : ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:

                single_chat(sockfd);

                break;

            case 2:

                group_chat(sockfd);

                break;

            case 3:

                logout_user(sockfd);

                return;

            default:

                printf("Invalid Choice\n");
        }
    }
}


/*---------------------------------------------------------------------------
 * Function    : main
 * Description : Main Client Program
 *-------------------------------------------------------------------------*/
int main()
{
    int sockfd;

    int choice;

    pthread_t tid;

    memset(logged_username,
           0,
           sizeof(logged_username));

    /*
     * Create socket
     */
    sockfd = create_socket();

    /*
     * Connect to server
     */
    connect_server(sockfd);

    /*
     * Create receiver thread.
     */
    if(pthread_create(&tid,
                      NULL,
                      receiver_thread,
                      (void *)&sockfd) != 0)
    {
        perror("pthread_create");

        close(sockfd);

        return 1;
    }

    while(1)
    {
        printf("\n");
        printf("=================================\n");
        printf("          TCP/IP CHAT ROOM\n");
        printf("=================================\n");

        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");

        printf("Enter Choice : ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:

                register_user(sockfd);

                break;

            case 2:

                login_user(sockfd);

                /*
                 * Give the receiver thread a small amount
                 * of time to process the login response.
                 */
                sleep(1);

                chat_menu(sockfd);

                break;

            case 3:

                close(sockfd);

                pthread_cancel(tid);

                pthread_join(tid,
                             NULL);

                printf("Client Exited\n");

                return 0;

            default:

                printf("Invalid Choice\n");
        }
    }

    return 0;
}