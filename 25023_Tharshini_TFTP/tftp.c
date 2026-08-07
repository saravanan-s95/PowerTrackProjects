/* NAME: THARSHINI S
   DATE: 03/06/2026
   DESCRIPTION: Developed a TFTP-based client-server file transfer application in C using UDP sockets for uploading and downloading files. */

#include "tftp.h"

void send_file_octet(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename) 
{
    FILE *fptr=fopen(filename,"rb");
    if(fptr==NULL)
    {
        perror("fopen");
        return;
    }

    tftp_packet packet; //used to send data packet
    tftp_packet ack; //used to receive ack packet

    uint16_t block=1; //initialize block number

    memset(&packet, 0, sizeof(packet));
    int read_bytes;
    //read file
    while((read_bytes=fread(packet.body.data_packet.data,1,512,fptr))>0)
    {
        packet.opcode=htons(DATA); //set DATA opcode
        packet.body.data_packet.block_number=htons(block);//set Block Number

        sendto(sockfd, &packet, 4 + read_bytes, 0,(struct sockaddr *)&client_addr, client_len); //why 4+bytes_read->tftp data packet format 2 bytes->Opcode, 2 bytes->Block Number, N bytes->Data
        if(recvfrom(sockfd, &ack, sizeof(ack), 0, NULL, NULL)<0)//wait for ack
        {
            perror("ACK timeout");
            break;
        } 

        block++; //next block

        if(read_bytes<512) //check last packet
        {
            break;
        }
    }

    fclose(fptr);
    printf("File sent successfully\n"); 

}

void receive_file_octet(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename) 
{
    FILE *fptr=fopen(filename,"wb");
    if(fptr==NULL)
    {
        perror("fopen");
        return;
    }

    tftp_packet packet; //used to store received data packet
    tftp_packet ack; //used to store ack packet to send
    while(1)
    {
        //receive data packet
        int r=recvfrom(sockfd,&packet,sizeof(packet),0,(struct sockaddr *)&client_addr,&client_len);

        if(r<0)
        {
            perror("recvfrom");
            break;
        }
        int data_len=r-4; //actual data size

        fwrite(packet.body.data_packet.data,1,data_len,fptr);//write data To file

        ack.opcode=htons(ACK); //ack packets preparing

        ack.body.ack_packet.block_number=packet.body.data_packet.block_number; //copy block number
        //send ack, ack packets conatins only opcode and block number
        sendto(sockfd,&ack,sizeof(uint16_t)*2,0,(struct sockaddr *)&client_addr,client_len);
        if(data_len<512) //check eof
        {
            break;
        }
    }
    fclose(fptr);
    printf("File received successfully\n");
    fflush(stdout);    
}


void send_file_netascii(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename)
{    
    FILE *fptr=fopen(filename,"r");
    if(fptr==NULL)
    {
        perror("fopen");
        return;
    }

    tftp_packet packet; //used to send data packet
    tftp_packet ack; //used to receive ack packet

    uint16_t block=1; //initialize block number
    int index=0;
    int ch;

    while(1)
    {
        memset(&packet, 0, sizeof(packet));
        index=0;
        while(index<512 &&(ch=fgetc(fptr))!=EOF)
        {
            if(ch=='\n')
            {
                if(index<511)
                {
                    packet.body.data_packet.data[index++]='\r';
                    packet.body.data_packet.data[index++]='\n';
                }
                else
                {
                    break;
                }
            }
            else if(ch=='\r')
            {
                if(index<511)
                {
                    packet.body.data_packet.data[index++]='\r';
                    packet.body.data_packet.data[index++]='\0';
                }
                else
                {
                    break;
                }
            }
            else
            {
                packet.body.data_packet.data[index++]=ch;
            }
        }

        packet.opcode=htons(DATA);
        packet.body.data_packet.block_number=htons(block);

        sendto(sockfd, &packet, 4 + index, 0,(struct sockaddr *)&client_addr, client_len); //why 4+bytes_read->tftp data packet format 2 bytes->Opcode, 2 bytes->Block Number, N bytes->Data
        if(recvfrom(sockfd, &ack, sizeof(ack), 0, NULL, NULL)<0)//wait for ack
        {
            perror("ACK timeout");
            break;
        } 

        block++; //next block

        if(index<512) //check last packet
        {
            break;
        }
    }
    fclose(fptr);
    printf("Netascii file sent successfully\n");

}

void receive_file_netascii(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename)
{
    FILE *fptr=fopen(filename,"w");
    if(fptr==NULL)
    {
        perror("fopen");
        return;
    }

    tftp_packet packet; //used to store received data packet
    tftp_packet ack; //used to store ack packet to send
    while(1)
    {
        //receive data packet
        int r=recvfrom(sockfd,&packet,sizeof(packet),0,(struct sockaddr *)&client_addr,&client_len);

        if(r<0)
        {
            perror("recvfrom");
            break;
        }
        int data_len=r-4; //actual data size
        for(int i=0;i<data_len;i++)
        {
            if(packet.body.data_packet.data[i]=='\r')
            {
                if(i+1<data_len)
                {
                    if(packet.body.data_packet.data[i+1]=='\n')
                    {
                        fputc('\n',fptr);
                        i++;
                    }
                    else if(packet.body.data_packet.data[i+1]=='\0')
                    {
                        fputc('\r',fptr);
                        i++;
                    }
                    else
                    {
                        fputc('\r',fptr);
                    }
                }
                else
                {
                    fputc('\r',fptr);
                }
            }

            else
            {
                fputc(packet.body.data_packet.data[i],fptr);
            }
        }
        ack.opcode=htons(ACK); //ack packets preparing

        ack.body.ack_packet.block_number=packet.body.data_packet.block_number; //copy block number
        //send ack, ack packets conatins only opcode and block number
        sendto(sockfd,&ack,sizeof(uint16_t)*2,0,(struct sockaddr *)&client_addr,client_len);
        if(data_len<512) //check eof
        {
            break;
        }
    }
    fclose(fptr);
    printf("Netascii file received successfully\n");
}