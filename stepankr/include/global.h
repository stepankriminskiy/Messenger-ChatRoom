#ifndef GLOBAL_H_
#define GLOBAL_H_

#define HOSTNAME_LEN 128
#define PATH_LEN 256
struct client {
    char name[50];
    char ip[22];
    int port;
    int fdsocket;
};

#endif
