#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 6333
#define MAX_USERS   100

/* Every chat message (login, signup, chat text, ...) uses this ONE struct.
 * We send the raw bytes of this struct straight over the network. */
typedef struct
{
    char username[30];
    char password[20];
    char target[30];   /* only used for private chat: who to send to */
    char message[256];
    int type;           /* what kind of message this is - see enum below */
} Message;

/* A separate, smaller struct used only for "who is online" bookkeeping:
 * the startup online-list, and live join/leave notices. */
typedef struct
{
    char username[30];
    int event; /* JOINED or LEFT (ignored when this is part of the list) */
    int last;  /* 1 = this is the last entry of the online list          */
} StatusUpdate;

/* Values that go in Message.type */
enum
{
    LOGIN,
    SIGNUP,
    LOGOUT,
    SINGLE_CHAT,
    GROUP_CHAT,
    LOGIN_OK,
    LOGIN_BAD_USER,
    LOGIN_BAD_PASS,
    SIGNUP_OK,
    SIGNUP_USER_TAKEN
};

/* Values that go in StatusUpdate.event */
enum
{
    JOINED,
    LEFT
};

#endif
