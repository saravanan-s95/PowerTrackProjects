#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int sockfd;
    struct sockaddr_in server;

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    printf("Socket created successfully\n");

    // Set server address and port
    server.sin_family = AF_INET;
    server.sin_port = htons(6333);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server\n");

    int choice;

    while (1)
    {
        printf("\n====================\n");
        printf("      CHAT ROOM\n");
        printf("====================\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        if (choice == 1)
        {
            char username[50];
            char password[50];
            char user_data[100];
            char reply[100];

            printf("Enter username: ");
            scanf("%49s", username);

            printf("Enter password: ");
            scanf("%49s", password);

            sprintf(user_data, "REGISTER %s %s", username, password);

            send(sockfd, user_data, sizeof(user_data), 0);

            recv(sockfd, reply, sizeof(reply), 0);

            printf("Server: %s\n", reply);
        }

        else if (choice == 2)
        {
            char username[50];
            char password[50];
            char user_data[100];
            char reply[100];

            printf("Enter username: ");
            scanf("%49s", username);

            printf("Enter password: ");
            scanf("%49s", password);

            sprintf(user_data, "LOGIN %s %s", username, password);

            send(sockfd, user_data, sizeof(user_data), 0);

            recv(sockfd, reply, sizeof(reply), 0);

            printf("Server: %s\n", reply);

            if (strcmp(reply, "Login successful") == 0)
            {
                char message[100];

                // Create process for receiving messages
                if (fork() == 0)
                {
                    char incoming[100];

                    while (1)
                    {
                        int bytes = recv(sockfd, incoming, sizeof(incoming) - 1, 0);

                        if (bytes <= 0)
                        {
                            break;
                        }

                        incoming[bytes] = '\0';

                        printf("\nMessage from other client: %s\n", incoming);
                        printf("Enter message: ");
                        fflush(stdout);
                    }

                    return 0;
                }

                // Parent process sends messages
                while (1)
                {
                    printf("Enter message: ");
                    scanf(" %[^\n]", message);

                    send(sockfd, message, strlen(message) + 1, 0);

                    if (strcmp(message, "/logout") == 0)
                    {
                        printf("Logged out\n");
                        break;
                    }
                }
            }
        }

        else if (choice == 3)
        {
            printf("Exiting...\n");
            break;
        }

        else
        {
            printf("Invalid choice\n");
        }
    }

    close(sockfd);

    return 0;
}