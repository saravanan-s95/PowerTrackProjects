# TCP/IP Chat Room

## 📌 Project Overview

The **TCP/IP Chat Room** is a client-server based chat application developed in **C** using **TCP/IP socket programming**. It allows multiple clients to connect to a central server and communicate with each other in real time.

The server manages client connections and broadcasts messages received from one client to all other connected clients.

## 🚀 Features

* TCP-based reliable communication
* Multiple client support
* Client-server architecture
* Real-time message broadcasting
* Concurrent client handling
* Client connection and disconnection handling
* Unique username support
* Graceful socket termination
* Command-line based interface

## 🏗️ Architecture

```text
              +----------------+
              |     Server     |
              |   TCP Socket   |
              +-------+--------+
                      |
          +-----------+-----------+
          |           |           |
          v           v           v
     +---------+ +---------+ +---------+
     | Client1 | | Client2 | | Client3 |
     +---------+ +---------+ +---------+
          |           |           |
          +-----------+-----------+
                Chat Messages
```

## 🛠️ Technologies Used

* **Language:** C
* **Protocol:** TCP/IP
* **Socket API:** BSD/POSIX Sockets
* **Operating System:** Linux
* **Concepts:** Client-Server Architecture, Socket Programming, Process/Thread-based Concurrency, Inter-Process Communication

## 📂 Project Structure

```text
TCP-IP-Chat-Room/
│
├── server.c
├── client.c
├── README.md
└── Makefile
```

## ⚙️ How It Works

### Server

1. Creates a TCP socket using `socket()`.
2. Binds the socket to an IP address and port using `bind()`.
3. Starts listening for incoming connections using `listen()`.
4. Accepts clients using `accept()`.
5. Handles communication with connected clients.
6. Broadcasts received messages to other clients.
7. Removes clients when they disconnect.

### Client

1. Creates a TCP socket.
2. Connects to the server using `connect()`.
3. Sends messages to the server.
4. Receives messages from other clients.
5. Continues communication until the client exits.
6. Closes the socket gracefully.

## 🔧 Compilation

Clone the repository:

```bash
git clone <YOUR_GITHUB_REPOSITORY_URL>
cd TCP-IP-Chat-Room
```

Compile the server:

```bash
gcc server.c -o server
```

Compile the client:

```bash
gcc client.c -o client
```

## ▶️ Running the Project

### Start the Server

```bash
./server
```

The server starts listening on the configured port.

Example:

```text
Server started...
Waiting for clients...
```

### Start the Client

Open another terminal:

```bash
./client
```

For multiple clients, open multiple terminals and run the client program in each terminal.

Example:

```text
Client 1:
Enter username: Ramanesh
Connected to server.

Client 2:
Enter username: User2
Connected to server.
```

## 💬 Example Chat

```text
[Ramanesh]: Hello everyone!

[User2]: Hi Ramanesh!

[Ramanesh]: How are you?

[User2]: I am fine.
```

Messages received by the server can be broadcast to all connected clients.

## 🔌 Important Socket Functions

| Function    | Purpose                        |
| ----------- | ------------------------------ |
| `socket()`  | Creates a socket               |
| `bind()`    | Assigns IP address and port    |
| `listen()`  | Waits for incoming connections |
| `accept()`  | Accepts a client connection    |
| `connect()` | Connects client to server      |
| `send()`    | Sends data                     |
| `recv()`    | Receives data                  |
| `close()`   | Closes socket                  |

## 🌐 TCP/IP Concepts Used

### TCP

TCP provides:

* Reliable communication
* Connection-oriented communication
* Ordered data delivery
* Error detection and retransmission
* Flow and congestion control

### IP

IP is responsible for addressing and routing packets between hosts.

The application uses:

```text
Application
     ↓
   TCP
     ↓
    IP
     ↓
 Ethernet / Wi-Fi
```

## 🧠 Key Concepts Demonstrated

* Socket programming
* TCP/IP networking
* Client-server architecture
* Port numbers and IP addresses
* Network byte order
* `struct sockaddr_in`
* `select()` / `poll()` or multithreading
* Concurrent client handling
* Message broadcasting
* Error handling
* File descriptors

## ⚠️ Error Handling

The application handles common socket errors such as:

* Socket creation failure
* Bind failure
* Listen failure
* Connection failure
* Send/receive errors
* Client disconnection
* Invalid server address

## 🔮 Future Enhancements

* Private messaging between clients
* Group chat/rooms
* User authentication
* Message timestamps
* Chat history
* File transfer
* Graphical user interface
* Encryption using TLS
* Online/offline user status

## 🎯 Learning Outcomes

Through this project, the following concepts are practically demonstrated:

* Understanding TCP/IP communication
* Creating TCP client and server applications
* Working with Linux socket APIs
* Handling multiple clients
* Implementing real-time message communication
* Understanding ports, IP addresses and sockets
* Applying network programming concepts using C

## 👨‍💻 Author

**Ramanesh M**


This project is developed for educational and learning purposes.

