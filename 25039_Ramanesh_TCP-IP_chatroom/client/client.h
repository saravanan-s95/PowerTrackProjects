
#ifndef CLIENT_H
#define CLIENT_H

/* Header Files */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include<pthread.h>
#include<fcntl.h>

/* Common Header */

#include "common.h"

/* Socket Functions */

int create_socket(void);

void connect_server(int sockfd);

/* Receiver Thread */

void *receiver_thread(void *arg);

/* Client Operations */

void register_user(int sockfd);

int login_user(int sockfd);

void single_chat(int sockfd);

void group_chat(int sockfd);

void logout_user(int sockfd);

void chat_menu(int sockfd);

#endif