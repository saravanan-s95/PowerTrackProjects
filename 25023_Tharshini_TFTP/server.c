/* NAME: THARSHINI S
   DATE: 03/06/2026
   DESCRIPTION: Developed a TFTP-based client-server file transfer application in C using UDP sockets for uploading and downloading files. */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tftp.h"


void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet);

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    tftp_packet packet;


    // Create UDP socket
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0) //check error
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    // Set socket timeout option
    struct timeval tv;
    tv.tv_sec=5; //timeout sec
    tv.tv_usec=0; //set microseconds 

    //TODO Use setsockopt() to set timeout option
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
    
    // Set up server address
    memset(&server_addr,0,sizeof(server_addr)); //clear server addr struct, after all bytes becomes 0

    server_addr.sin_family=AF_INET; //use Ipv4
    server_addr.sin_addr.s_addr=INADDR_ANY;//Accept packets from any network interface
    //set port number
    server_addr.sin_port=htons(PORT);   

    // Bind the socket
    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
  

    printf("TFTP Server listening on port %d...\n", PORT);

    // Main loop to handle incoming requests
    while (1) {
        int n = recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0)
        {
            continue;
        }

         handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);
    return 0;
}

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet) 
{
    // Extract the TFTP operation (read or write) from the received packet
    // and call send_file or receive_file accordingly
    uint16_t opcode=ntohs(packet->opcode);
    if(opcode==RRQ)
    {
        if(strcmp(packet->body.request.mode,"octet")==0)
        {
            send_file_octet(sockfd,client_addr,client_len,packet->body.request.filename);
        }
        else if(strcmp(packet->body.request.mode,"netascii")==0)
        {
            send_file_netascii(sockfd,client_addr,client_len,packet->body.request.filename);
        }        
    }

    else if(opcode==WRQ)
    {
        if(strcmp(packet->body.request.mode,"octet")==0)
        {
            receive_file_octet(sockfd,client_addr,client_len,packet->body.request.filename);
        }
        else if(strcmp(packet->body.request.mode,"netascii")==0)
        {
             receive_file_netascii(sockfd,client_addr,client_len,packet->body.request.filename);
        } 
    }

    else
    {
        printf("Unknown opcode %d\n",opcode);
    }
}




