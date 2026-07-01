
// Name : Ramanesh M
// Batch : 25039_003
// Date Of completion : 13/05/2026



#include "tftp_client.h" // Client header

int validate_ip(char *ip)
{
    struct sockaddr_in sa; // Temporary address structure

    return inet_pton(AF_INET,ip,&(sa.sin_addr)); // Validate IPv4 address
}

void process_command(tftp_client_t *client,int choice)
{
    switch(choice)
    {
        case 1:
            connect_to_server(client); // Connect to server
            break;

        case 2:
            put_file_client(client); // Upload file
            break;

        case 3:
            get_file_client(client); // Download file
            break;

        case 4:
        {
            int mode; // Transfer mode

            printf("\n1. Normal (512 Bytes)\n"); // Normal mode
            printf("2. Octet (1 Byte)\n"); // Octet mode
            printf("3. Netascii\n"); // Netascii mode
            printf("Enter Mode : "); // Prompt

            scanf("%d",&mode); // Read mode
            getchar(); // Remove newline

            if(mode >= MODE_NORMAL && mode <= MODE_NETASCII)
            {
                client->mode = mode; // Update mode
                printf("Mode Changed Successfully\n"); // Success message
            }
            else
            {
                printf("Invalid Mode\n"); // Invalid mode
            }

            break;
        }

        case 5:
            disconnect_client(client); // Exit client
            break;

        default:
            printf("Invalid Choice\n"); // Invalid menu choice
    }
}

void connect_to_server(tftp_client_t *client)
{
    char ip[INET_ADDRSTRLEN]; // Server IP
    struct timeval tv; // Timeout structure

    printf("Enter Server IP : "); // Prompt

    scanf("%15s",ip); // Read IP
    getchar(); // Remove newline

    if(!validate_ip(ip))
    {
        printf("Invalid IP Address\n"); // Invalid IP
        return;
    }

    client->sockfd = socket(AF_INET,SOCK_DGRAM,0); // Create UDP socket

    if(client->sockfd < 0)
    {
        perror("socket"); // Socket creation failed
        return;
    }

    tv.tv_sec = TIMEOUT_SEC; // Timeout seconds
    tv.tv_usec = 0; // Timeout microseconds

    setsockopt(client->sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv)); // Set receive timeout

    memset(&client->server_addr,0,sizeof(client->server_addr)); // Clear structure

    client->server_addr.sin_family = AF_INET; // IPv4 family
    client->server_addr.sin_port = htons(PORT); // Server port

    inet_pton(AF_INET,ip,&client->server_addr.sin_addr); // Convert IP

    client->server_len = sizeof(client->server_addr); // Save length

    strcpy(client->server_ip,ip); // Store IP

    client->connected = 1; // Mark connected

    printf("Connected To Server %s\n",ip); // Success message
}

void put_file_client(tftp_client_t *client)
{
    char filename[256]; // Filename
    char buffer[BUFFER_SIZE]; // Request buffer
    char ack[BUFFER_SIZE]; // ACK buffer
    FILE *fp; // File pointer
    int n; // Received bytes

    if(!client->connected)
    {
        printf("Connect To Server First\n"); // Connection check
        return;
    }

    system("ls"); // Display files

    printf("Enter Filename : "); // Prompt

    scanf("%255s",filename); // Read filename
    getchar(); // Remove newline

    fp = fopen(filename,"rb"); // Open file

    if(fp == NULL)
    {
        printf("File Not Found\n"); // Missing file
        return;
    }

    fclose(fp); // Close file

    memset(buffer,0,sizeof(buffer)); // Clear buffer

    *(uint16_t *)buffer = htons(WRQ); // WRQ opcode

    strcpy(buffer + 2,filename); // Copy filename

    sendto(client->sockfd,buffer,strlen(filename)+3,0,(struct sockaddr *)&client->server_addr,client->server_len); // Send WRQ

    printf("WRQ Sent\n"); // Status

    n = recvfrom(client->sockfd,ack,sizeof(ack),0,NULL,NULL); // Receive ACK

    if(n >= 4 && ntohs(*(uint16_t *)ack) == ACK)
    {
        printf("Received ACK 0\n"); // ACK received

        send_file(client->sockfd,client->server_addr,client->server_len,filename,client->mode); // Send file
    }
    else
    {
        printf("ACK Not Received\n"); // ACK missing
    }
}

void get_file_client(tftp_client_t *client)
{
    char filename[256]; // Filename
    char buffer[BUFFER_SIZE]; // Request buffer

    if(!client->connected)
    {
        printf("Connect To Server First\n"); // Connection check
        return;
    }

    printf("Enter Filename : "); // Prompt

    scanf("%255s",filename); // Read filename
    getchar(); // Remove newline

    memset(buffer,0,sizeof(buffer)); // Clear buffer

    *(uint16_t *)buffer = htons(RRQ); // RRQ opcode

    strcpy(buffer + 2,filename); // Copy filename

    sendto(client->sockfd,buffer,strlen(filename)+3,0,(struct sockaddr *)&client->server_addr,client->server_len); // Send RRQ

    printf("RRQ Sent\n"); // Status

    receive_file(client->sockfd,client->server_addr,client->server_len,filename,client->mode); // Receive file
}

void disconnect_client(tftp_client_t *client)
{
    if(client->connected)
    {
        close(client->sockfd); // Close socket
    }

    printf("Exiting TFTP Client\n"); // Exit message

    exit(0); // Terminate application
}

int main()
{
    tftp_client_t client; // Client structure
    int choice; // Menu choice

    memset(&client,0,sizeof(client)); // Initialize structure

    client.mode = MODE_NORMAL; // Default mode

    while(1)
    {
        printf("\n========================================\n"); // Menu border
        printf("              TFTP CLIENT\n"); // Title
        printf("========================================\n"); // Menu border
        printf("1. Connect To Server\n"); // Option 1
        printf("2. Put File\n"); // Option 2
        printf("3. Get File\n"); // Option 3
        printf("4. Change Mode\n"); // Option 4
        printf("5. Exit\n"); // Option 5
        printf("========================================\n"); // Menu border
        printf("Enter Choice : "); // Prompt

        scanf("%d",&choice); // Read choice
        getchar(); // Remove newline

        process_command(&client,choice); // Process choice
    }

    return 0; // Program success
}