#include "main.h"

int sockfd;
char my_username[30];
int logged_in = 0;

void read_line(char *buf, int size);
void show_main_menu(void);
void login(void);
void signup(void);
void enter_chat_room(void);
void show_chat_menu(void);
void single_chat(void);
void group_chat(void);
void *listen_for_messages(void *arg);
void handle_exit(int sig);

int main(void)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        perror("connect failed");
        exit(1);
    }
    printf("Connected to the chat server.\n");

    signal(SIGINT, handle_exit); /* Ctrl+C logs out cleanly instead of just dying */

    while (1)
        show_main_menu();

    return 0;
}

/* reads one line of input and strips the trailing newline that fgets leaves */
void read_line(char *buf, int size)
{
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';
}

void handle_exit(int sig)
{
    (void)sig;
    if (logged_in)
    {
        Message msg;
        memset(&msg, 0, sizeof(msg));
        msg.type = LOGOUT;
        send(sockfd, &msg, sizeof(msg), 0);
    }
    printf("\nGoodbye!\n");
    close(sockfd);
    exit(0);
}

void show_main_menu(void)
{
    printf("\n1. Login\n2. Register\n3. Exit\nChoice: ");
    char choice[10];
    read_line(choice, sizeof(choice));
    int c = atoi(choice);

    if (c == 1)
        login();
    else if (c == 2)
        signup();
    else if (c == 3)
        handle_exit(0);
    else
        printf("Invalid choice.\n");
}

void login(void)
{
    Message msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = LOGIN;

    printf("Username: ");
    read_line(msg.username, sizeof(msg.username));
    printf("Password: ");
    read_line(msg.password, sizeof(msg.password));

    send(sockfd, &msg, sizeof(msg), 0);

    Message reply;
    if (recv(sockfd, &reply, sizeof(reply), 0) <= 0)
    {
        printf("Server disconnected.\n");
        exit(0);
    }

    if (reply.type == LOGIN_OK)
    {
        printf("Login successful!\n");
        strcpy(my_username, msg.username);
        enter_chat_room();
    }
    else if (reply.type == LOGIN_BAD_USER)
        printf("No such user.\n");
    else if (reply.type == LOGIN_BAD_PASS)
        printf("Wrong password.\n");
}

void signup(void)
{
    Message msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = SIGNUP;

    printf("Choose a username: ");
    read_line(msg.username, sizeof(msg.username));
    printf("Choose a password: ");
    read_line(msg.password, sizeof(msg.password));

    send(sockfd, &msg, sizeof(msg), 0);

    Message reply;
    if (recv(sockfd, &reply, sizeof(reply), 0) <= 0)
    {
        printf("Server disconnected.\n");
        exit(0);
    }

    if (reply.type == SIGNUP_OK)
    {
        printf("Registration successful!\n");
        strcpy(my_username, msg.username);
        enter_chat_room();
    }
    else
        printf("That username is already taken.\n");
}

/* shared path after a successful login OR signup */
void enter_chat_room(void)
{
    logged_in = 1;

    /* the server sends the current online-user list right after login */
    printf("--- Online users ---\n");
    while (1)
    {
        StatusUpdate update;
        recv(sockfd, &update, sizeof(update), 0);
        if (update.last)
            break;
        printf("%s is online\n", update.username);
    }

    /* a background thread listens for incoming messages so we can keep
     * typing without missing anything that arrives while we type */
    pthread_t tid;
    pthread_create(&tid, NULL, listen_for_messages, NULL);
    pthread_detach(tid);

    while (1)
        show_chat_menu();
}

void show_chat_menu(void)
{
    printf("\n1. Single Chat\n2. Group Chat\n3. Logout\nChoice: ");
    char choice[10];
    read_line(choice, sizeof(choice));
    int c = atoi(choice);

    if (c == 1)
        single_chat();
    else if (c == 2)
        group_chat();
    else if (c == 3)
    {
        Message msg;
        memset(&msg, 0, sizeof(msg));
        msg.type = LOGOUT;
        send(sockfd, &msg, sizeof(msg), 0);
        printf("You have logged out.\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("Invalid choice.\n");
}

/* type /quit at the "> " prompt to leave single/group chat and go back */

void single_chat(void)
{
    char target[30];
    printf("Chat with: ");
    read_line(target, sizeof(target));
    printf("(type /quit to go back)\n");

    while (1)
    {
        printf("> ");
        char text[256];
        read_line(text, sizeof(text));
        if (strcmp(text, "/quit") == 0)
            return;
        if (strlen(text) == 0)
            continue;

        Message msg;
        memset(&msg, 0, sizeof(msg));
        msg.type = SINGLE_CHAT;
        strcpy(msg.username, my_username);
        strcpy(msg.target, target);
        strcpy(msg.message, text);
        send(sockfd, &msg, sizeof(msg), 0);
    }
}

void group_chat(void)
{
    printf("(type /quit to go back)\n");
    while (1)
    {
        printf("> ");
        char text[256];
        read_line(text, sizeof(text));
        if (strcmp(text, "/quit") == 0)
            return;
        if (strlen(text) == 0)
            continue;

        Message msg;
        memset(&msg, 0, sizeof(msg));
        msg.type = GROUP_CHAT;
        strcpy(msg.username, my_username);
        strcpy(msg.message, text);
        send(sockfd, &msg, sizeof(msg), 0);
    }
}

/* runs on its own thread for as long as we're logged in */
void *listen_for_messages(void *arg)
{
    (void)arg;
    while (1)
    {
        char buffer[1024];
        int n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0)
        {
            printf("\nServer disconnected.\n");
            exit(0);
        }

        if (n == sizeof(StatusUpdate))
        {
            StatusUpdate *update = (StatusUpdate *)buffer;
            if (update->event == JOINED)
                printf("\n%s just joined the chat.\n> ", update->username);
            else if (update->event == LEFT)
                printf("\n%s left the chat.\n> ", update->username);
        }
        else if (n == sizeof(Message))
        {
            Message *msg = (Message *)buffer;
            printf("\n%s: %s\n> ", msg->username, msg->message);
        }
        fflush(stdout);
    }
    return NULL;
}
