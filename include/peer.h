#ifndef PEER_H
#define PEER_H

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <unordered_map>
#include <atomic>
#include <errno.h>

struct socketInfo
{
    int fd;
    struct sockaddr_in address;
    socklen_t addrlen;
};

class Peer
{
private:
    static constexpr int PORT = 8080;
    static constexpr int BUFFER_SIZE = 1024;
    static constexpr int MAX_CONN = 3;
    std::atomic<bool> running{true};
    inline static std::unordered_map<int, std::thread> connections; // maybe put in constructor

    static void handleConnection(int connID);
    static socketInfo createServerSocket();

public:
    void runServer();
    void stop();
    void runClient(const std::string &serverIP);
};

#endif /* PEER_H */