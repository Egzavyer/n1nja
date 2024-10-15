#include "../include/peer.h"

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;
constexpr int MAX_CONN = 3;
// std::unordered_map<pthread_t, int> connections;

struct arg_struct
{
    pthread_t tid;
    int connID;
};

void Peer::begin(int choice)
{
    pthread_t ptid;
    if (choice == 1)
    {
        pthread_create(&ptid, NULL, &server, NULL);
        // pthread_join(ptid, NULL);
        std::cout << "Exiting...\n";
        pthread_exit(NULL);
    }
    else if (choice == 2)
    {
        pthread_create(&ptid, NULL, &client, NULL);
        // pthread_join(ptid, NULL);
        std::cout << "Exiting...\n";
        pthread_exit(NULL);
    }

    std::cout << "Exiting...\n";
}

void *Peer::server(void *arg)
{
    pthread_detach(pthread_self());
    int serverFD, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket: FAILED\n";
        exit(EXIT_FAILURE);
    }
    // std::cout << "socket: SUCCESS\n";

    if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "setsockopt: FAILED\n";
        exit(EXIT_FAILURE);
    }
    // std::cout << "setsockopt: SUCCESS\n";

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind: FAILED\n";
        exit(EXIT_FAILURE);
    }

    // std::cout << "bind: SUCCESS\n";

    while (true)
    {
        if (listen(serverFD, MAX_CONN) < 0)
        {
            std::cerr << "listen: FAILED\n";
            exit(EXIT_FAILURE);
        }

        // std::cout << "listen: SUCCESS\n";
        std::cout << "Server Listening On Port " << PORT << "\n";

        if ((newSocket = accept(serverFD, (struct sockaddr *)&address, &addrlen)) < 0)
        {
            std::cerr << "accept: FAILED\n";
            exit(EXIT_FAILURE);
        }

        std::cout << "accept " << newSocket << ": SUCCESS\n";

        // pthread_t tid;
        //  connections[newSocket] = tid;
        // struct arg_struct args;
        // args.connID = newSocket;
        //  args.tid = tid;

        connections[newSocket] = std::thread(&Peer::handleConnection, newSocket);
        // pthread_create(&tid, NULL, &connectionHandler, (void *)&args);
        for (auto &conn : connections)
        {
            std::cout << conn.first << " : " << conn.second.get_id() << '\n';
            if (conn.second.joinable())
            {
                conn.second.join();
            }
        }
    }

    close(serverFD);
    pthread_exit(NULL);
}

void *Peer::client(void *arg)
{
    pthread_detach(pthread_self());

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
    // sleep(10);
    close(sock);
    pthread_exit(NULL);
}

/*void *Peer::connectionHandler(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valread = read(args->connID, buffer, BUFFER_SIZE);
    std::cout << "Received: " << buffer << "\n";
    send(args->connID, buffer, valread, 0);
    std::cout << "Echo Sent\n";
    close(args->connID);
    // connections.erase(args->connID);
    pthread_exit(NULL);
}*/

void Peer::handleConnection(int connID)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t valRead = read(connID, buffer, BUFFER_SIZE);
    if (valRead > 0)
    {
        std::cout << "Received: " << buffer << "\n";
        send(connID, buffer, valRead, 0);
        std::cout << "Echo Sent\n";
    }
    close(connID);
    // connections.erase(connID);
}
