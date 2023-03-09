#ifndef SERVER_H
#define SERVER_H

int start_server(int argc, char **argv);

struct client {
    char name[50];
    char ip[INET_ADDRSTRLEN];
    int port;
};

#endif