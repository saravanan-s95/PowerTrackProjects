/* NAME: THARSHINI S
   DATE: 03/06/2026
   DESCRIPTION: Developed a TFTP-based client-server file transfer application in C using UDP sockets for uploading and downloading files. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tftp.h"
#include "client.h"


int main() {
    char command[256];//stores user command
    tftp_client_t client; //create client structure
    memset(&client, 0, sizeof(client));  // Initialize all members to 0

    // Main loop for command-line interface
    while (1) //client runs until user enters "bye"
    { 
        printf("tftp> "); //prompt
        fgets(command, sizeof(command), stdin); //read user input

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // Process the command
        process_command(&client, command); //2nd cmd for processing
    }

    return 0;
}

//Function to process commands
void process_command(tftp_client_t *client, char *command)
{
    char cmd[50];
    char filename[50];
    char mode[20];

    //reads data from a string and stores it into variables.
    int arg_count=sscanf(command,"%s %s %s",cmd,filename,mode);//returns the number of successfully matched and assigned inputs
    if(arg_count==0)
    {
        return;
    }

    if(strcmp(cmd,"connect")==0)
    {
        connect_to_server(client,filename,PORT);
    }
    else if(strcmp(cmd,"put")==0)
    {
        if(arg_count!=3)
        {
            printf("Usage: put <filename> <octet|netascii>\n");
            return;
        }
        put_file(client,filename,mode);
    }
    else if(strcmp(cmd,"get")==0)
    {
        if(arg_count!=3)
        {
            printf("Usage: get <filename> <octet|netascii>\n");
            return;
        }
        get_file(client,filename,mode);
    }
    else if((strcmp(cmd,"bye")==0)||(strcmp(cmd,"quit")==0))
    {
        disconnect(client);
        exit(0);
    }
    else if(strcmp(cmd,"help")==0)
    {
        printf("Supported commands:\n");
        printf("connect <ip>\n");
        printf("put <filename> <octet|netascii>\n");
        printf("get <filename> <octet|netascii>\n");
        printf("bye\n");

    }
    else
    {
        printf("Unknown Command\nType help to get supported features\n");
    }   
}

// This function is to initialize socket with given server IP, no packets sent to server in this function
void connect_to_server(tftp_client_t *client, char *ip, int port)
{
    // Create UDP socket
    client->sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(client->sockfd<0) //check error
    {
        perror("socket");
        return;
    }

    struct timeval tv;
    tv.tv_sec=5; //timeout sec
    tv.tv_usec=0; //set microseconds 

    // Set socket timeout option
    setsockopt(client->sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv)); //it dont wait forever for recvfrom

    memset(&client->server_addr,0,sizeof(client->server_addr)); //clear server addr struct, after all bytes becomes 0

    // Set up server address
    client->server_addr.sin_family=AF_INET; //use Ipv4
    //set port number
    client->server_addr.sin_port=htons(port);

    //convert ip address
    inet_pton(AF_INET,ip,&client->server_addr.sin_addr); //inet_pton() converts into binary format

    //store structure size
    client->server_len=sizeof(client->server_addr); //used later in sento() and recvfrom()

    strcpy(client->server_ip,ip); //save ip string, used for displaying connection info in later
    printf("Connected to %s:%d\n",ip,port);
}

void put_file(tftp_client_t *client, char *filename, char *mode) //here user wants to upload file to server
{
    if(client->sockfd <= 0)
    {
        printf("ERROR: Not connected to server\n");
        return;
    }

    //handling invalid mode before sending the request
    if(strcmp(mode,"octet")!=0 && strcmp(mode,"netascii")!=0)
    {
        printf("Invalid mode! Use 'octet' or 'netascii'\n");
        return;
    }

    // Send WRQ request and send file
    send_request(client->sockfd,client->server_addr,filename,mode,WRQ); //send an WRQ(write request)2 packet

    //send file based on mode
    if(strcmp(mode,"octet")==0)
    {
        send_file_octet(client->sockfd,client->server_addr,client->server_len,filename); //client start sending file data
    }

    else if(strcmp(mode,"netascii")==0)
    {
        send_file_netascii(client->sockfd,client->server_addr,client->server_len,filename); //client start sending file data
    }
   
}

void get_file(tftp_client_t *client, char *filename, char *mode) //here client wants to download file from the server
{
    if(client->sockfd <= 0)
    {
        printf("ERROR: Not connected to server\n");
        return;
    }

    //handling invalid mode before sending the request
    if(strcmp(mode,"octet")!=0 && strcmp(mode,"netascii")!=0)
    {
        printf("Invalid mode! Use 'octet' or 'netascii'\n");
        return;
    }
    // Send RRQ and recive file 
    send_request(client->sockfd,client->server_addr,filename,mode,RRQ); //send an RRQ(read request)1 packet

    //receive file based on mode
    if(strcmp(mode,"octet")==0)
    {
        receive_file_octet(client->sockfd,client->server_addr,client->server_len,filename); //client waits for DATA packets
    }
    else if(strcmp(mode,"netascii")==0)
    {
        receive_file_netascii(client->sockfd,client->server_addr,client->server_len,filename); //client waits for DATA packets
    }    
}

void disconnect(tftp_client_t *client) //called when user types "bye" or "quit"
{
    // close fd
    close(client->sockfd);
    printf("Disconnected\n");
   
}
void send_request(int sockfd, struct sockaddr_in server_addr, char *filename, char *mode, int opcode)
{
    tftp_packet packet; //create a packet structure
    memset(&packet,0,sizeof(packet)); //clear packet

    //set opcode
    packet.opcode=htons(opcode);

    //store filename
    strcpy(packet.body.request.filename,filename);

    //store transfer mode
    strcpy(packet.body.request.mode,mode);

    //send packet
    sendto(sockfd,&packet,sizeof(packet),0,(struct sockaddr *)&server_addr,sizeof(server_addr)); //sends req packet to server

}
