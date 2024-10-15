#include "../include/peer.h"

int main()
{
    Peer peer;
    int choice;
    std::cout << "1. Server\n2. Client\n3. Exit\n";
    std::cin >> choice;

    try
    {
        if (choice == 1)
        {
            std::thread serverThread(&Peer::runServer, &peer);
            std::cout << "Press Enter to stop the server..." << std::endl;
            // std::cin.get();
            // peer.stop();
            serverThread.join();
        }
        else if (choice == 2)
        {
            peer.runClient();
        }
        else
        {
            std::cout << "Exiting...\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << '\n';
        return 1;
    }
}