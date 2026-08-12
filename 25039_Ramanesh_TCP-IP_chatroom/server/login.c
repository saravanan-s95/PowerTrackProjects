

#include "server.h"

void login_user(int clientfd, MESSAGE *msg)
{
    USER_INFO user;

    MESSAGE response;

    memset(&user, 0, sizeof(USER_INFO));
    memset(&response, 0, sizeof(MESSAGE));

    printf("\n----------------------------------\n");
    printf("LOGIN REQUEST\n");
    printf("Username : %s\n", msg->sender);
    printf("----------------------------------\n");

    /* Copy username and password */

    strcpy(user.username, msg->sender);
    strcpy(user.password, msg->password);

    /* Verify username and password */

    if(verify_user(&user))
    {
        response.option = LOGIN;
        response.status = SUCCESS;

        strcpy(response.sender,
               user.username);

        strcpy(response.message,
               "Login Successful");

        /*
         * Store the user as online.
         *
         * If you are using the current DATABASE structure
         * with status and socketfd, update_status() can be
         * used here.
         */

        update_status(user.username,
                      ONLINE,
                      clientfd);

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("User Logged In Successfully\n");

        
        //  Send online users to the client.

        send_online_users(clientfd);
    }
    else
    {
        response.option = LOGIN;
        response.status = FAILURE;

        strcpy(response.message,
               "Invalid Username or Password");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("Login Failed\n");
    }
}


void logout_user(int clientfd, MESSAGE *msg)
{
    MESSAGE response;

    memset(&response, 0, sizeof(MESSAGE));

    printf("\n----------------------------------\n");
    printf("LOGOUT REQUEST\n");
    printf("Username : %s\n", msg->sender);
    printf("----------------------------------\n");

    if(remove_user(msg->sender))
    {
        response.option = LOGOUT;
        response.status = SUCCESS;

        strcpy(response.sender,
               msg->sender);

        strcpy(response.message,
               "Logout Successful");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("User Logged Out Successfully\n");
    }
    else
    {
        response.option = LOGOUT;
        response.status = FAILURE;

        strcpy(response.message,
               "Logout Failed");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("Logout Failed\n");
    }
}