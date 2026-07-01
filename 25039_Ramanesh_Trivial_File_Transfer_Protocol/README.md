# Trivial File Transfer Protocol (TFTP)

## Overview

The Trivial File Transfer Protocol (TFTP) is a simple file transfer protocol that operates over UDP. It is commonly used for transferring files between a client and a server in environments where simplicity and low overhead are important. This project implements a TFTP-based file transfer system in C using UDP socket programming and follows the concepts defined in RFC 1350.

The project supports file upload and download operations, acknowledgements, retransmissions, error handling, and multiple transfer modes.

---

## Features

- UDP-based communication
- Read Request (RRQ)
- Write Request (WRQ)
- DATA packet transfer
- ACK packet handling
- ERROR packet handling
- File upload from client to server
- File download from server to client
- Timeout-based retransmission
- Normal transfer mode
- Octet transfer mode
- Netascii transfer mode
- Menu-driven client application
- Multi-packet file transfer

---

## Project Structure

```text
TFTP/
├── tftp.h
├── tftp_client.h
├── tftp.c
├── tftp_client.c
├── tftp_server.c
├── Makefile
└── README.md
```

## Protocol Operations

### Read Request (RRQ)

The client sends an RRQ packet to the server requesting a file.

1. Client sends RRQ.
2. Server checks whether the file exists.
3. Server sends DATA packets.
4. Client sends ACK packets for each DATA block.
5. Transfer ends when the final DATA packet contains fewer than 512 bytes.

### Write Request (WRQ)

The client sends a WRQ packet to upload a file.

1. Client sends WRQ.
2. Server creates the destination file.
3. Server sends ACK block 0.
4. Client starts sending DATA packets.
5. Server acknowledges each DATA block.
6. Transfer ends after the last block is received.

---

## TFTP Packet Types

### RRQ Packet

| Field | Size |
|---------|---------|
| Opcode | 2 Bytes |
| Filename | Variable |

Opcode Value: 1

### WRQ Packet

| Field | Size |
|---------|---------|
| Opcode | 2 Bytes |
| Filename | Variable |

Opcode Value: 2

### DATA Packet

| Field | Size |
|---------|---------|
| Opcode | 2 Bytes |
| Block Number | 2 Bytes |
| Data | 0-512 Bytes |

Opcode Value: 3

### ACK Packet

| Field | Size |
|---------|---------|
| Opcode | 2 Bytes |
| Block Number | 2 Bytes |

Opcode Value: 4

### ERROR Packet

| Field | Size |
|---------|---------|
| Opcode | 2 Bytes |
| Error Code | 2 Bytes |
| Error Message | Variable |

Opcode Value: 5

---

## Error Codes

| Error Code | Description |
|------------|-------------|
| 0 | Undefined Error |
| 1 | File Not Found |
| 2 | Access Violation |
| 3 | Disk Full |
| 4 | Illegal Operation |
| 5 | Unknown Transfer ID |
| 6 | File Already Exists |

---

## Transfer Modes

### Normal Mode

- Transfers 512 bytes per packet.
- Default transfer mode.
- Faster transfer compared to the other modes.

### Octet Mode

- Transfers one byte at a time.
- Useful for understanding packet-level transmission.

### Netascii Mode

- Text transfer mode.
- Inserts carriage return before newline characters.
- Similar to the standard TFTP text mode.

---

## Retransmission Mechanism

The project supports timeout-based retransmission to improve reliability over UDP.

```c
#define TIMEOUT_SEC 5
#define MAX_RETRIES 3
```

If an acknowledgement is not received within the timeout period:

1. The packet is retransmitted.
2. Retry count is incremented.
3. Transfer is terminated after the maximum retry count is reached.

---

## Compilation

### Using Makefile

Build both client and server:

```bash
make
```

Generated executables:

```text
tftp_client
tftp_server
```

### Using GCC

Compile the client:

```bash
gcc -Wall tftp_client.c tftp.c -o tftp_client
```

Compile the server:

```bash
gcc -Wall tftp_server.c tftp.c -o tftp_server
```

---

## Execution

### Start the Server

```bash
./tftp_server
```

Expected Output:

```text
========================================
TFTP Server Listening On Port 3020
========================================
```

### Start the Client

Open another terminal and run:

```bash
./tftp_client
```

Expected Output:

```text
========================================
              TFTP CLIENT
========================================
1. Connect To Server
2. Put File
3. Get File
4. Change Mode
5. Exit
========================================
```

---

## Client Operations

### Connect to Server

Select:

```text
1
```

Enter the server IP address:

```text
127.0.0.1
```

### Upload a File

Select:

```text
2
```

Enter the filename to upload.

### Download a File

Select:

```text
3
```

Enter the filename to download.

### Change Transfer Mode

Select:

```text
4
```

Available modes:

```text
1. Normal
2. Octet
3. Netascii
```

### Exit Client

Select:

```text
5
```

---

## Concepts Covered

- UDP Socket Programming
- Client-Server Architecture
- RFC 1350 TFTP Protocol
- File Handling in C
- Packet-Based Communication
- Timeout Handling
- Retransmission Mechanism
- Network Byte Order Conversion
- Error Handling
- File Upload and Download
- Transfer Modes
- Reliable Communication over UDP

---

## Learning Outcomes

This project helps understand:

- Working of the TFTP protocol
- UDP-based communication
- File transfer mechanisms
- Packet design and processing
- Reliability implementation over UDP
- Network programming concepts
- Timeout and retransmission handling
- Socket programming in Linux

---

## References

- RFC 1350 – Trivial File Transfer Protocol
- UNIX Network Programming
- Beej's Guide to Network Programming
- TCP/IP Illustrated Volume 1

---

## Author

Ramanesh M

## License

This project is developed for educational and learning purposes.

