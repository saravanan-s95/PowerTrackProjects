#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_CLIENTS 10

int *clients;
int *client_count;
char (*client_names)[50];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int client_sockfd;
} ClientData;


// Remove disconnected client from the shared client list
void remove_client(int client_sockfd)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < *client_count; i++)
    {
        if (clients[i] == client_sockfd)
        {
            for (int j = i; j < *client_count - 1; j++)
            {
                clients[j] = clients[j + 1];
                strcpy(client_names[j], client_names[j + 1]);
            }

            (*client_count)--;

            printf("Client disconnected\n");
            printf("Total clients: %d\n", *client_count);

            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}


// Send message to all clients except the sender
void broadcast_message(int client_sockfd, char *message)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < *client_count; i++)
    {
        if (clients[i] != client_sockfd)
        {
            send(clients[i],
                 message,
                 strlen(message) + 1,
                 0);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}


// Handle one connected client
void *handle_client(void *arg)
{
    ClientData *data = (ClientData *)arg;

    int client_sockfd = data->client_sockfd;

    free(data);

    char user_data[100];
    char username[50];
    char password[50];
    char command[20];
    char reply[100];

    // Receive REGISTER or LOGIN request
    int bytes = recv(client_sockfd,
                     user_data,
                     sizeof(user_data) - 1,
                     0);

    if (bytes <= 0)
    {
        close(client_sockfd);
        return NULL;
    }

    user_data[bytes] = '\0';

    printf("Request from client: %s\n", user_data);

    sscanf(user_data,
           "%19s %49s %49s",
           command,
           username,
           password);


    // ================= REGISTER =================

    if (strcmp(command, "REGISTER") == 0)
    {
        FILE *fp;

        char file_username[50];
        char file_password[50];

        int user_exists = 0;

        fp = fopen("users.txt", "r");

        if (fp != NULL)
        {
            while (fscanf(fp,
                          "%49s %49s",
                          file_username,
                          file_password) == 2)
            {
                if (strcmp(file_username, username) == 0)
                {
                    user_exists = 1;
                    break;
                }
            }

            fclose(fp);
        }

        if (user_exists)
        {
            strcpy(reply, "Username already exists");
        }
        else
        {
            fp = fopen("users.txt", "a");

            if (fp == NULL)
            {
                strcpy(reply, "File open failed");
            }
            else
            {
                fprintf(fp,
                        "%s %s\n",
                        username,
                        password);

                fclose(fp);

                strcpy(reply,
                       "Registration successful");
            }
        }

        send(client_sockfd,
             reply,
             strlen(reply) + 1,
             0);

        close(client_sockfd);

        return NULL;
    }


    // ================= LOGIN =================

    else if (strcmp(command, "LOGIN") == 0)
    {
        FILE *fp;

        char file_username[50];
        char file_password[50];

        int login_success = 0;

        fp = fopen("users.txt", "r");

        if (fp != NULL)
        {
            while (fscanf(fp, "%49s %49s", file_username, file_password) == 2)
            {
                if (strcmp(file_username, username) == 0 && strcmp(file_password, password) == 0)
                {
                    login_success = 1;
                    break;
                }
            }

            fclose(fp);
        }

        if (login_success)
        {
            strcpy(reply, "Login successful");
        }
        else
        {
            strcpy(reply, "Login failed");
        }

        send(client_sockfd, reply, strlen(reply) + 1, 0);


        // Add client only after successful login
        if (login_success)
        {
            pthread_mutex_lock(&clients_mutex);

            if (*client_count < MAX_CLIENTS)
            {
                clients[*client_count] = client_sockfd;

                strcpy(client_names[*client_count], username);

                (*client_count)++;

                printf("Total clients: %d\n", *client_count);
            }

            pthread_mutex_unlock(&clients_mutex);


            // Notify other clients that a new user joined
            char join_message[100];

            sprintf(join_message, "%s joined the chat room", username);

            broadcast_message(client_sockfd, join_message);


            // ================= CHAT LOOP =================

            char message[100];

            while (1)
            {
                memset(message, 0, sizeof(message));

                bytes = recv(client_sockfd, message, sizeof(message) - 1, 0);

                if (bytes <= 0)
                {
                    remove_client(client_sockfd);

                    close(client_sockfd);

                    break;
                }

                message[bytes] = '\0';

                printf("Message from %s: %s\n", username, message);


                // ================= /WHO =================

                if (strcmp(message, "/who") == 0)
                {
                    char online_users[500];

                    strcpy(online_users, "Online users:\n");

                    pthread_mutex_lock(&clients_mutex);

                    for (int i = 0; i < *client_count; i++)
                    {
                        strcat(online_users, client_names[i]);

                        strcat(online_users, "\n");
                    }

                    pthread_mutex_unlock(&clients_mutex);

                    send(client_sockfd, online_users, strlen(online_users) + 1, 0);

                    continue;
                }


                // ================= /HELP =================

                if (strcmp(message, "/help") == 0)
                {
                    char help_message[100];

                    strcpy(help_message, "/who - Online users\n");

                    strcat(help_message, "/logout - Logout\n");

                    strcat(help_message, "/help - Commands");

                    send(client_sockfd, help_message, strlen(help_message) + 1, 0);

                    continue;
                }


                // ================= /LOGOUT =================

                if (strcmp(message, "/logout") == 0)
                {
                    printf("%s logged out\n", username);

                    char leave_message[100];

                    sprintf(leave_message, "%s left the chat room", username);

                    broadcast_message(client_sockfd, leave_message);

                    remove_client(client_sockfd);

                    close(client_sockfd);

                    break;
                }


                // ================= CHAT MESSAGE =================

                char chat_message[150];

                sprintf(chat_message, "%s: %s", username, message);

                broadcast_message(client_sockfd, chat_message);
            }
        }
        else
        {
            close(client_sockfd);
        }
    }


    // Invalid command
    else
    {
        strcpy(reply,
               "Invalid command");

        send(client_sockfd,
             reply,
             strlen(reply) + 1,
             0);

        close(client_sockfd);
    }

    return NULL;
}


int main()
{
    // Shared memory for client socket descriptors
    clients = mmap(NULL, sizeof(int) * MAX_CLIENTS, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Shared memory for client count
    client_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Shared memory for usernames
    client_names = mmap(NULL, sizeof(char) * MAX_CLIENTS * 50, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *client_count = 0;


    int sockfd;
    int client_sockfd;

    struct sockaddr_in server;


    // Create TCP socket
    sockfd = socket(AF_INET,
                    SOCK_STREAM,
                    0);

    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    printf("Socket created successfully\n");


    int opt = 1;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


    // Configure server IP and port
    server.sin_family = AF_INET;

    server.sin_port = htons(6333);

    server.sin_addr.s_addr = INADDR_ANY;


    // Bind socket to port
    if (bind(sockfd,
             (struct sockaddr *)&server,
             sizeof(server)) < 0)
    {
        printf("Bind failed\n");

        close(sockfd);

        return 1;
    }

    printf("Bind successful\n");


    // Start listening for clients
    if (listen(sockfd, 5) < 0)
    {
        printf("Listen failed\n");

        close(sockfd);

        return 1;
    }

    printf("Server is listening on port 6333\n");


    // Accept clients continuously
    while (1)
    {
        client_sockfd = accept(sockfd,
                               NULL,
                               NULL);

        if (client_sockfd < 0)
        {
            printf("Accept failed\n");
            continue;
        }

        printf("Client connected\n");


        ClientData *data =
            malloc(sizeof(ClientData));

        if (data == NULL)
        {
            printf("Memory allocation failed\n");

            close(client_sockfd);

            continue;
        }

        data->client_sockfd = client_sockfd;


        // Create a separate thread for each client
        pthread_t thread;

        if (pthread_create(&thread,
                           NULL,
                           handle_client,
                           data) != 0)
        {
            printf("Thread creation failed\n");

            free(data);

            close(client_sockfd);

            continue;
        }


        // Automatically release thread resources
        pthread_detach(thread);
    }


    close(sockfd);

    return 0;
}