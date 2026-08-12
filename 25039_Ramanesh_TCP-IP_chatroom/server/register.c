
#include "server.h"

void register_user(int clientfd, MESSAGE *msg)
{
    USER_INFO user;

    MESSAGE response;

    memset(&user, 0, sizeof(USER_INFO));
    memset(&response, 0, sizeof(MESSAGE));

    printf("\n----------------------------------\n");
    printf("REGISTER REQUEST\n");
    printf("Username : %s\n", msg->sender);
    printf("----------------------------------\n");

    /* Copy username and password */

    strcpy(user.username, msg->sender);
    strcpy(user.password, msg->password);

    /* Check whether username already exists */

    if(user_exists(user.username))
    {
        response.option = REGISTER;
        response.status = FAILURE;

        strcpy(response.message,
               "Username Already Exists");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("Registration Failed : Username Already Exists\n");

        return;
    }

    /* Add user to database */

    if(add_user(&user))
    {
        response.option = REGISTER;
        response.status = SUCCESS;

        strcpy(response.message,
               "Registration Successful");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("User Registered Successfully\n");
    }
    else
    {
        response.option = REGISTER;
        response.status = FAILURE;

        strcpy(response.message,
               "Registration Failed");

        send(clientfd,
             &response,
             sizeof(MESSAGE),
             0);

        printf("Registration Failed\n");
    }
}