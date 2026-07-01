#include "tftp.h" // Common TFTP definitions

void send_file(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *filename,int mode)
{
    FILE *fp; // File pointer
    char buffer[BUFFER_SIZE]; // Data packet
    char ack_buf[BUFFER_SIZE]; // ACK buffer
    char temp[DATA_SIZE]; // Temporary buffer
    uint16_t block = 1; // Block number
    uint16_t opcode; // Opcode
    uint16_t ack_block; // ACK block
    size_t bytes_read; // Bytes read
    int retries; // Retry counter
    int n; // Received bytes
    int i; // Loop variable
    int j; // Netascii index

    fp = fopen(filename,"rb"); // Open file

    if(fp == NULL)
    {
        perror("fopen"); // File open failed
        return;
    }

    printf("Starting File Transfer : %s\n",filename); // Status

    while(1)
    {
        memset(buffer,0,sizeof(buffer)); // Clear packet

        *(uint16_t *)buffer = htons(DATA); // DATA opcode

        *(uint16_t *)(buffer + 2) = htons(block); // Block number

        if(mode == MODE_OCTET)
        {
            bytes_read = fread(buffer + 4,1,1,fp); // Read 1 byte
        }
        else if(mode == MODE_NETASCII)
        {
            bytes_read = fread(temp,1,DATA_SIZE,fp); // Read block

            j = 0; // Reset index

            for(i = 0 ; i < bytes_read ; i++)
            {
                if(temp[i] == '\n')
                {
                    buffer[4 + j++] = '\r'; // Add carriage return
                }

                buffer[4 + j++] = temp[i]; // Copy character
            }

            bytes_read = j; // Updated length
        }
        else
        {
            bytes_read = fread(buffer + 4,1,DATA_SIZE,fp); // Read 512 bytes
        }

        retries = 0; // Reset retries

        while(retries < MAX_RETRIES)
        {
            sendto(sockfd,buffer,bytes_read + 4,0,(struct sockaddr *)&client_addr,client_len); // Send DATA

            printf("Sent Block %u\n",block); // Status

            n = recvfrom(sockfd,ack_buf,sizeof(ack_buf),0,(struct sockaddr *)&client_addr,&client_len); // Receive ACK

            if(n >= 4)
            {
                opcode = ntohs(*(uint16_t *)ack_buf); // Extract opcode

                ack_block = ntohs(*(uint16_t *)(ack_buf + 2)); // Extract block

                if(opcode == ACK && ack_block == block)
                {
                    printf("Received ACK %u\n",block); // ACK received

                    break;
                }
            }

            retries++; // Increment retry

            printf("Retry %d For Block %u\n",retries,block); // Status
        }

        if(retries == MAX_RETRIES)
        {
            printf("Transfer Failed : Maximum Retries Reached\n"); // Failure

            fclose(fp); // Close file

            return;
        }

        if(bytes_read < DATA_SIZE)
        {
            break; // Last packet
        }

        block++; // Next block
    }

    printf("File Transfer Completed\n"); // Success

    fclose(fp); // Close file
}

void receive_file(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *filename,int mode)
{
    FILE *fp; // File pointer
    char buffer[BUFFER_SIZE]; // Receive buffer
    char ack[4]; // ACK packet
    uint16_t opcode; // Opcode
    uint16_t block; // Block number
    uint16_t expected_block = 1; // Expected block
    int retries = 0; // Retry counter
    int n; // Received bytes

    fp = fopen(filename,"wb"); // Create file

    if(fp == NULL)
    {
        perror("fopen"); // File creation failed
        return;
    }

    printf("Receiving File : %s\n",filename); // Status

    while(1)
    {
        n = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client_addr,&client_len); // Receive DATA

        if(n < 4)
        {
            retries++; // Increment retry

            if(retries == MAX_RETRIES)
            {
                printf("Receive Failed : Maximum Retries Reached\n"); // Failure

                fclose(fp); // Close file

                return;
            }

            continue;
        }

        opcode = ntohs(*(uint16_t *)buffer); // Extract opcode

        block = ntohs(*(uint16_t *)(buffer + 2)); // Extract block

        if(opcode == DATA && block == expected_block)
        {
            fwrite(buffer + 4,1,n - 4,fp); // Write data

            *(uint16_t *)ack = htons(ACK); // ACK opcode

            *(uint16_t *)(ack + 2) = htons(block); // ACK block

            sendto(sockfd,ack,4,0,(struct sockaddr *)&client_addr,client_len); // Send ACK

            printf("Received Block %u\n",block); // Status

            if((n - 4) < DATA_SIZE)
            {
                break; // Last packet
            }

            expected_block++; // Next block

            retries = 0; // Reset retries
        }
    }

    printf("File Received Successfully\n"); // Success

    fclose(fp); // Close file
}