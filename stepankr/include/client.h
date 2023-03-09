#ifndef CLIENT_H
#define CLIENT_H

int start_client(int argc, char **argv);

struct client {
    char name[50];
    char ip[INET_ADDRSTRLEN];
    int port;
};
#endif