# TCP/IP Chat Room

## Project Brief

TCP/IP based chat room application developed in C using socket programming. Multiple clients can connect to the server, login and communicate with each other.

## Technologies Used

* C
* TCP/IP Socket Programming
* Linux
* POSIX Threads (pthread)
* Shared Memory (mmap)
* File Handling
* fork()

## Features

* User Registration
* User Login
* Multiple Client Chat
* Message Broadcasting
* Online Users (`/who`)
* Help Command (`/help`)
* Logout (`/logout`)
* Client Disconnect Handling

## Project Structure

```text
25045F_001_Swaraj_TCP_IP_CHAT_ROOM/
├── README.md
├── client/
│   └── client.c
└── server/
    ├── server.c
    └── users.txt
```

## How to Run

### Server

```bash
cd server
gcc server.c -lpthread
./a.out
```

### Client

```bash
cd client
gcc client.c
./a.out
```

Run the client in multiple terminals to connect multiple users.

## Commands

```text
/who
/logout
/help
```

## Learning

* TCP client-server communication
* Socket programming
* POSIX threads
* Shared memory
* Mutex synchronization
* File handling
* Multi-client communication

