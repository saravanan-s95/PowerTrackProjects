#ifndef COMMON_H
#define COMMON_H

#define PORT            6333
#define SERVER_IP       "127.0.0.1"

#define MAX_CLIENTS     20
#define BUFFER_SIZE     1024

#define USERNAME_SIZE   30
#define PASSWORD_SIZE   30

#define ONLINE          1
#define OFFLINE         0

#define REGISTER        1
#define LOGIN           2
#define SINGLE_CHAT     3
#define GROUP_CHAT      4
#define LOGOUT          5
#define ONLINE_USERS    6

#define SUCCESS         1
#define FAILURE         0


typedef struct
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];

}USER_INFO;


typedef struct
{
    int option;                     // REGISTER, LOGIN, CHAT...
    int status;                     // SUCCESS / FAILURE

    char sender[USERNAME_SIZE];
    char receiver[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    char message[BUFFER_SIZE];

}MESSAGE;


typedef struct
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];

    int status;                     // ONLINE / OFFLINE
    int socketfd;                   // active socket fd if online

}DATABASE;

#endif