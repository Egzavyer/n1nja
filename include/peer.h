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
#include <thread>
#include <unordered_map>

class Peer
{
private:
    static constexpr int PORT = 8080;
    static constexpr int BUFFER_SIZE = 1024;
    static constexpr int MAX_CONN = 3;
    inline static std::unordered_map<int, std::thread> connections;

    static void handleConnection(int connID);

public:
    static void *server(void *arg);
    static void *client(void *arg);
    // static void *connectionHandler(void *arg);
    static void begin(int choice);
};

#endif /* PEER_H */