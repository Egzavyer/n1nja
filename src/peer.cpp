#include "../include/peer.h"

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;
constexpr int MAX_CONN = 3;

void Peer::begin(int choice)
{
    if (choice == 1)
    {
        server();
    }
    else if (choice == 2)
    {
        client();
    }
    else
    {
        std::cout << "Exiting...\n";
    }
}

void Peer::server()
{
    int serverFD, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket: FAILED\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "socket: SUCCESS\n";

    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "setsockopt: FAILED\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "setsockopt: SUCCESS\n";

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind: FAILED\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "bind: SUCCESS\n";

    if (listen(serverFD, MAX_CONN) < 0)
    {
        std::cerr << "listen: FAILED\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "listen: SUCCESS\n";
    std::cout << "Server Listening On Port " << PORT << "\n";

    if ((newSocket = accept(serverFD, (struct sockaddr *)&address, &addrlen)) < 0)
    {
        std::cerr << "accept: FAILED\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "accept: SUCCESS\n";

    ssize_t valread = read(newSocket, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << "\n";
    send(newSocket, buffer, valread, 0);
    std::cout << "Echo Sent\n";

    close(newSocket);
    close(serverFD);
}

void Peer::client()
{
    int sock = 0;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "socket: FAILED\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "socket: SUCCESS\n";

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address\n";
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "connect: FAILED\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "connect: SUCCESS\n";

    std::string hello = "Hello from Client";
    send(sock, hello.c_str(), hello.size(), 0);
    std::cout << "Hello Message Sent\n";
    read(sock, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << "\n";
    close(sock);
}
