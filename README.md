# Modern Networking Concepts Project: Chat Application

## Introduction
This project is a part of the Modern Networking Concepts (CSE 489) course, focused on implementing a chat application in C to explore network programming basics. It facilitates functionalities such as message broadcasting, direct messaging, and dynamic user connection management. The application consists of two main components: a client and a server, each offering various commands for interactive communication.

## Installation

### Prerequisites
- GCC or any standard C compiler
- Basic knowledge of terminal or command prompt commands
- Network programming libraries included with the standard C library

### Compiling with Makefile
To compile both the client and server, navigate to the project directory and use the provided makefile:

## Usage
Starting the Server
To start the server, execute:

```bash
./assignment1 s <port>
Replace <port> with the port number you wish the server to listen on.
```

### Connecting the Client
To connect a client to the server, use:
 ```bash
./assignment1 c <port>
Replace <port> with the client's listening port.
```

After starting the client, connect to the server using the LOGIN command followed by the server's IP address and port:

```bash
LOGIN <server-ip> <server-port>
```
server-ip: The IP address of the server.

server-port: The port number the server is listening on.

### Available Commands

**AUTHOR:** Confirms my adherence to academic integrity.

**IP:** Displays the IP address of the client/server.

**PORT:** Shows the port number the client/server is running on.

**LIST:** Lists all currently connected users.

**STATISTICS:** (Server only) Displays messaging statistics for each user.

**BROADCAST:** Sends a message to all connected users.

**SEND:** Sends a message to a specific user.

**REFRESH:** Requests an updated list of connected users from the server.

**LOGOUT:** Logs the client out of the server.

**EXIT:** Closes the client or server application.

## Demonstrating Functionality

### Direct Messaging
2 Users Connected to the server:

![image](https://github.com/stepankriminskiy/Messenger-ChatRoom/assets/98437298/43ffc827-7e8f-4cd3-b8c6-cc3d2cf3aa14)

Client with port 4056 sending a message directly to the other client with port 7001

![image](https://github.com/stepankriminskiy/Messenger-ChatRoom/assets/98437298/98a4a55a-9c8a-4ad3-8520-74228c8f802c)

The other client with port 7001 receiving the message

![image](https://github.com/stepankriminskiy/Messenger-ChatRoom/assets/98437298/3290fc73-cd9f-4f68-a28c-454433bc8c37)

### Broadcast
A user broadcasting to the server a message:

![image](https://github.com/stepankriminskiy/Messenger-ChatRoom/assets/98437298/f03f2bc2-e4c7-4a30-af5f-8526a731f016)

2 Different clients that are already connected to the server, receiving the message:

![image](https://github.com/stepankriminskiy/Messenger-ChatRoom/assets/98437298/1d197475-267e-4497-bf1e-434d4526fb08)

### Statistics

![image](https://github.com/stepankriminskiy/Messenger-ChatRoom/assets/98437298/b7a1ec36-511f-4bcd-bb47-eb27de8148b1)

After a user logs out example

![image](https://github.com/stepankriminskiy/Messenger-ChatRoom/assets/98437298/a056a9fc-29e3-4348-aaf5-c879826931d4)



