#include "server.h"

int user_exists(char *username)
{
    int fd;
    DATABASE user;

    fd = open(DATABASE_FILE, O_RDONLY);

    if(fd < 0)
        return 0;

    while(read(fd, &user, sizeof(DATABASE)) > 0)
    {
        if(strcmp(user.username, username) == 0)
        {
            close(fd);
            return 1;
        }
    }

    close(fd);

    return 0;
}

int add_user(USER_INFO *user)
{
    int fd;
    DATABASE new_user;

    memset(&new_user, 0, sizeof(DATABASE));

    strcpy(new_user.username, user->username);
    strcpy(new_user.password, user->password);

    new_user.status = OFFLINE;
    new_user.socketfd = -1;

    fd = open(DATABASE_FILE,O_WRONLY | O_CREAT | O_APPEND,0644);

    if(fd < 0)
        return 0;

    write(fd, &new_user, sizeof(DATABASE));

    close(fd);

    return 1;
}

int verify_user(USER_INFO *user)
{
    int fd;
    DATABASE temp;

    fd = open(DATABASE_FILE, O_RDONLY);

    if(fd < 0)
        return 0;

    while(read(fd, &temp, sizeof(DATABASE)) > 0)
    {
        if(strcmp(temp.username, user->username) == 0 &&
           strcmp(temp.password, user->password) == 0)
        {
            close(fd);
            return 1;
        }
    }

    close(fd);

    return 0;
}

int update_status(char *username,int status,int sockfd)
{
    int fd;
    DATABASE user;

    fd = open(DATABASE_FILE, O_RDWR);

    if(fd < 0)
        return 0;

    while(read(fd, &user, sizeof(DATABASE)) > 0)
    {
        if(strcmp(user.username, username) == 0)
        {
            user.status = status;
            user.socketfd = sockfd;

            lseek(fd,-sizeof(DATABASE),SEEK_CUR);

            write(fd,&user,sizeof(DATABASE));

            close(fd);

            return 1;
        }
    }

    close(fd);

    return 0;
}

int get_socketfd(char *username)
{
    int fd;
    DATABASE user;

    fd = open(DATABASE_FILE, O_RDONLY);

    if(fd < 0)
        return -1;

    while(read(fd, &user, sizeof(DATABASE)) > 0)
    {
        if(strcmp(user.username, username) == 0)
        {
            close(fd);
            return user.socketfd;
        }
    }

    close(fd);

    return -1;
}

int remove_user(char *username)
{
    return update_status(username,OFFLINE,-1);
}

void send_online_users(int clientfd)
{
    int fd;
    DATABASE user;
    MESSAGE response;

    memset(&response, 0, sizeof(MESSAGE));

    response.option = ONLINE_USERS;
    response.status = SUCCESS;

    fd = open(DATABASE_FILE, O_RDONLY);

    if(fd < 0)
        return;

    while(read(fd, &user, sizeof(DATABASE)) > 0)
    {
        if(user.status == ONLINE)
        {
            strcat(response.message, user.username);
            strcat(response.message, "\n");
        }
    }

    close(fd);

    send(clientfd,
         &response,
         sizeof(MESSAGE),
         0);
}