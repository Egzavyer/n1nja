#include "../include/peer.h"

int main()
{
    Peer peer;
    int choice;
    std::cout << "1. Server\n2. Client\n3. Exit\n";
    std::cin >> choice;
    peer.begin(choice);
}