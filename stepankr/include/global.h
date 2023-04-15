#ifndef GLOBAL_H_
#define GLOBAL_H_

#define HOSTNAME_LEN 128
#define PATH_LEN 256
struct blocked{
    char ip[16];
};
struct client {
    char name[50];
    char ip[22];
    int port;
    int fdsocket;
    int messagesSent;
    int messagesReceived;
    int loggedIn;
    //int numBlocked;

    //int bufferedAmt;
    //char BufferedMessages[100][256];
    //struct blocked blocked_clients[20];
};

#endif
