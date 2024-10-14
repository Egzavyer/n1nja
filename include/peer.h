#ifndef PEER_H
#define PEER_H

#include <iostream>
#include <string>
#include <memory>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <unordered_map>

class Peer
{
public:
    static void *server(void *arg);
    static void *client(void *arg);
    static void *connectionHandler(void *arg);
    static void begin(int choice);
};

#endif /* PEER_H */