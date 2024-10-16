#include "../include/peer.h"

socketInfo Peer::createServerSocket()
{
    struct socketInfo sock;
    int serverFD;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
    }

    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw std::runtime_error("Setsockopt failed: " + std::string(strerror(errno)));
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
    }

    sock.address = address;
    sock.fd = serverFD;
    sock.addrlen = addrlen;
    return sock;
}

void Peer::runServer()
{
    struct socketInfo sock = Peer::createServerSocket();
    int serverFD = sock.fd;
    struct sockaddr_in address = sock.address;
    socklen_t addrlen = sock.addrlen;
    int newSocket;

    if (listen(serverFD, MAX_CONN) < 0)
    {
        throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
    std::cout << "Server listening on " << ip << ":" << PORT << std::endl;

    while (running)
    {

        if ((newSocket = accept(serverFD, (struct sockaddr *)&address, &addrlen)) < 0)
        {
            throw std::runtime_error("accept: FAILED\n");
        }

        std::cout << "accept " << newSocket << ": SUCCESS" << std::endl;

        connections[newSocket] = std::thread(&Peer::handleConnection, newSocket);
    }

    for (auto &conn : connections)
    {
        if (conn.second.joinable())
        {
            std::cout << conn.first << " : " << conn.second.get_id() << '\n';
            conn.second.join();
        }
    }

    close(serverFD);
}

void Peer::runClient(const std::string &serverIP)
{
    int sock = 0;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
    }

    std::cout << "socket: SUCCESS" << std::endl;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid address: " + std::string(strerror(errno)));
    }

    std::cout << "Address: VALID" << std::endl;

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        throw std::runtime_error("Connection failed: " + std::string(strerror(errno)));
    }

    std::cout << "Connected to server: " << serverIP << ":" << PORT << std::endl;

    std::string msg;
    std::cout << "Enter Message: ";
    std::getline(std::cin, msg);
    send(sock, msg.c_str(), msg.length(), 0);
    std::cout << "Message Sent\n";

    ssize_t valRead = read(sock, buffer, BUFFER_SIZE);
    if (valRead > 0)
    {
        std::cout << "Received: " << buffer << std::endl;
    }

    close(sock);
}

void Peer::handleConnection(int connID)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valRead;

    while ((valRead = read(connID, buffer, BUFFER_SIZE)) > 0)
    {
        std::cout << "Received: " << buffer << "\n";
        send(connID, buffer, valRead, 0);
        std::cout << "Echo Sent\n";
        memset(buffer, 0, BUFFER_SIZE);
    }
    if (valRead == 0)
    {
        std::cout << "Client disconnected" << std::endl;
    }
    else if (valRead < 0)
    {
        std::cerr << "Read error: " << strerror(errno) << std::endl;
    }
    close(connID);
}

void Peer::stop()
{
    running = false;
}