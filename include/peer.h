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

class Peer
{
public:
    static void server();
    static void client();
    static void begin(int choice);
};

#endif /* PEER_H */