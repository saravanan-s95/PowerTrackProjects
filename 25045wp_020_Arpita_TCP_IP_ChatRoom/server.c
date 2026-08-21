#include "main.h"

/* ---------- registered users ("database"), loaded from user.txt ---------- */
char db_users[MAX_USERS][30];
char db_passwords[MAX_USERS][20];
int db_count = 0;

/* ---------- users who are currently online (two parallel arrays) --------- */
char online_names[MAX_USERS][30];
int online_sockets[MAX_USERS];
int online_count = 0;

/* protects the online_* arrays above, since many client threads touch them */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void load_users_from_file(void);
void save_user_to_file(char *username, char *password);
int find_user(char *username);
void add_online_user(char *username, int sockfd);
void remove_online_user(int sockfd);
void send_online_list(int sockfd);
void notify_others(char *username, int event, int except_sockfd);
void send_to_user(char *target, Message *msg);
void send_to_everyone_except(char *username, Message *msg);
void *handle_client(void *arg);

int main(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int yes = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    listen(server_fd, 10);

    load_users_from_file();
    setvbuf(stdout, NULL, _IOLBF, 0); /* print log lines immediately */
    printf("Chat Server: waiting for clients on %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1)
    {
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(server_fd, NULL, NULL);
        if (*client_sock < 0)
        {
            free(client_sock);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_sock);
        pthread_detach(tid); /* we will never pthread_join this thread */
    }
}

/* Runs once per connected client, on its own thread, for its whole session. */
void *handle_client(void *arg)
{
    int sockfd = *(int *)arg;
    free(arg);
    char my_username[30] = ""; /* empty until this client logs in */

    while (1)
    {
        Message msg;
        if (recv(sockfd, &msg, sizeof(msg), 0) <= 0)
            break; /* client disconnected */

        if (msg.type == LOGIN)
        {
            Message reply;
            memset(&reply, 0, sizeof(reply));
            int i = find_user(msg.username);

            if (i == -1)
            {
                reply.type = LOGIN_BAD_USER;
                send(sockfd, &reply, sizeof(reply), 0);
            }
            else if (strcmp(db_passwords[i], msg.password) != 0)
            {
                reply.type = LOGIN_BAD_PASS;
                send(sockfd, &reply, sizeof(reply), 0);
            }
            else
            {
                reply.type = LOGIN_OK;
                send(sockfd, &reply, sizeof(reply), 0);

                strcpy(my_username, msg.username);
                add_online_user(my_username, sockfd);
                send_online_list(sockfd);
                notify_others(my_username, JOINED, sockfd);
                printf("Chat Server: %s logged in\n", my_username);
            }
        }
        else if (msg.type == SIGNUP)
        {
            Message reply;
            memset(&reply, 0, sizeof(reply));

            if (find_user(msg.username) != -1)
            {
                reply.type = SIGNUP_USER_TAKEN;
                send(sockfd, &reply, sizeof(reply), 0);
            }
            else
            {
                strcpy(db_users[db_count], msg.username);
                strcpy(db_passwords[db_count], msg.password);
                db_count++;
                save_user_to_file(msg.username, msg.password);

                reply.type = SIGNUP_OK;
                send(sockfd, &reply, sizeof(reply), 0);

                strcpy(my_username, msg.username);
                add_online_user(my_username, sockfd);
                send_online_list(sockfd);
                notify_others(my_username, JOINED, sockfd);
                printf("Chat Server: %s registered and logged in\n", my_username);
            }
        }
        else if (msg.type == LOGOUT)
        {
            break;
        }
        else if (msg.type == SINGLE_CHAT)
        {
            send_to_user(msg.target, &msg);
        }
        else if (msg.type == GROUP_CHAT)
        {
            send_to_everyone_except(msg.username, &msg);
        }
    }

    if (strlen(my_username) > 0)
    {
        notify_others(my_username, LEFT, sockfd);
        remove_online_user(sockfd);
        printf("Chat Server: %s left\n", my_username);
    }
    close(sockfd);
    return NULL;
}

/* ------------------------- registered-user "database" ------------------- */

int find_user(char *username)
{
    for (int i = 0; i < db_count; i++)
        if (strcmp(db_users[i], username) == 0)
            return i;
    return -1;
}

void load_users_from_file(void)
{
    FILE *f = fopen("user.txt", "a"); /* create the file if it doesn't exist */
    if (f)
        fclose(f);

    f = fopen("user.txt", "r");
    if (!f)
        return;

    /* each line looks like: username,password */
    while (fscanf(f, "%29[^,],%19[^\n]\n", db_users[db_count], db_passwords[db_count]) == 2)
        db_count++;

    fclose(f);
}

void save_user_to_file(char *username, char *password)
{
    FILE *f = fopen("user.txt", "a");
    if (!f)
        return;
    fprintf(f, "%s,%s\n", username, password);
    fclose(f);
}

/*  online-user list  */

void add_online_user(char *username, int sockfd)
{
    pthread_mutex_lock(&lock);
    strcpy(online_names[online_count], username);
    online_sockets[online_count] = sockfd;
    online_count++;
    pthread_mutex_unlock(&lock);
}

void remove_online_user(int sockfd)
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < online_count; i++)
    {
        if (online_sockets[i] == sockfd)
        {
            /* fill the gap by moving the last entry into this slot */
            strcpy(online_names[i], online_names[online_count - 1]);
            online_sockets[i] = online_sockets[online_count - 1];
            online_count--;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}

void send_online_list(int sockfd)
{
    pthread_mutex_lock(&lock);

    for (int i = 0; i < online_count; i++)
    {
        if (online_sockets[i] == sockfd)
            continue; /* don't list the user to themself */

        StatusUpdate update;
        memset(&update, 0, sizeof(update));
        strcpy(update.username, online_names[i]);
        update.last = 0;
        send(sockfd, &update, sizeof(update), 0);
    }

    StatusUpdate end_marker;
    memset(&end_marker, 0, sizeof(end_marker));
    end_marker.last = 1; /* tells the client "that's everyone" */
    send(sockfd, &end_marker, sizeof(end_marker), 0);

    pthread_mutex_unlock(&lock);
}

void notify_others(char *username, int event, int except_sockfd)
{
    pthread_mutex_lock(&lock);

    StatusUpdate update;
    memset(&update, 0, sizeof(update));
    strcpy(update.username, username);
    update.event = event;

    for (int i = 0; i < online_count; i++)
        if (online_sockets[i] != except_sockfd)
            send(online_sockets[i], &update, sizeof(update), 0);

    pthread_mutex_unlock(&lock);
}

/* must be called with "lock" already held */
static int try_send_to(char *target, Message *msg)
{
    for (int i = 0; i < online_count; i++)
    {
        if (strcmp(online_names[i], target) == 0)
        {
            send(online_sockets[i], msg, sizeof(*msg), 0);
            return 1;
        }
    }
    return 0;
}

void send_to_user(char *target, Message *msg)
{
    pthread_mutex_lock(&lock);

    int delivered = try_send_to(target, msg);
    if (!delivered)
    {
        Message error;
        memset(&error, 0, sizeof(error));
        strcpy(error.username, "SERVER");
        strcpy(error.message, "That user is offline or does not exist.");
        error.type = SINGLE_CHAT;
        try_send_to(msg->username, &error); /* tell the sender it failed */
    }

    pthread_mutex_unlock(&lock);
}

void send_to_everyone_except(char *username, Message *msg)
{
    pthread_mutex_lock(&lock);

    for (int i = 0; i < online_count; i++)
        if (strcmp(online_names[i], username) != 0)
            send(online_sockets[i], msg, sizeof(*msg), 0);

    pthread_mutex_unlock(&lock);
}
