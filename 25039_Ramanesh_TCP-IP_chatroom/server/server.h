
#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#include "common.h"

/*--------------------------- DATABASE FILE ---------------------------*/

#define DATABASE_FILE "users.dat"

/*--------------------------- SOCKET FUNCTIONS ------------------------*/

int create_socket(void);

void bind_socket(int sockfd);

void listen_socket(int sockfd);

int accept_client(int sockfd);

/*--------------------------- CLIENT HANDLER ---------------------------*/

void *client_handler(void *arg);

/*--------------------------- DATABASE FUNCTIONS -----------------------*/

int user_exists(char *username);

int add_user(USER_INFO *user);

int verify_user(USER_INFO *user);

int update_status(char *username,
                  int status,
                  int socketfd);

int get_socketfd(char *username);

int remove_user(char *username);

void send_online_users(int clientfd);

/*--------------------------- REGISTER FUNCTIONS ----------------------*/

void register_user(int clientfd,
                   MESSAGE *msg);

/*--------------------------- LOGIN FUNCTIONS -------------------------*/

void login_user(int clientfd,
                MESSAGE *msg);

void logout_user(int clientfd,
                 MESSAGE *msg);

/*--------------------------- CHAT FUNCTIONS ---------------------------*/

void single_chat(int clientfd,
                 MESSAGE *msg);

void group_chat(int clientfd,
                MESSAGE *msg);

#endif