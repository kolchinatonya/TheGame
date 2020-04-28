//#include "pch.h"

#include "Network.h"

Network::Network() {
    ip = "10.55.131.135";
}

//void Network::CreateConnection() {
//    std::cout << "Enter (s) for server, enter (c) for client" << std::endl;
//    cin >> connectionType;
//
//    if (connectionType == 's') {
//        sf::TcpListener listener;
//        std::cout << "Waiting for another player..." << std::endl;
//        listener.listen(2000);
//        listener.accept(socket);
//        socket.setBlocking(false);
//        std::cout << "Player found!" << std::endl;
//    }
//    else {
//        socket.connect(ip, 2000);
//        socket.setBlocking(false);
//        std::cout << "Connection!" << std::endl;
//    }
//}

void Network::CreateConnection() {
    socket.connect(ip, 55001);
    socket.setBlocking(false);
    int players_count = 0;
    sf::Packet packet;
    while (players_count == 0) {
        socket.receive(packet);
        packet >> players_count;
    }
    switch (players_count) {
        case 0:
            std::cout << "Server doesn't work!" << std::endl;
            break;
        case 1:
            std::cout << "Waiting for another player..." << std::endl;
            while (players_count != 2) {
                socket.receive(packet);
                packet >> players_count;
            }
            std::cout << "Start the game!" << std::endl;
            break;
        case 2:
            std::cout << "Start the game!" << std::endl;
            break;
    }
}


void Network::GetAnotherFish(AnotherPlayerFish& anotherFish) {
    sf::Packet packet;
    sf::Vector2f pos;
    int type;
    if (socket.receive(packet) == sf::Socket::NotReady) {
        return;
    }
    if (packet >> pos.x >> pos.y >> type) {
        anotherFish = AnotherPlayerFish(pos, static_cast<FishType> (type));
        //testing
        std::cout << "anotherFish recieved! " << std::endl;
        std::cout << "Position" << anotherFish.GetPosition().x << ":" << anotherFish.GetPosition().y << "\nLevel: " << static_cast<int> (anotherFish.GetType()) <<std::endl;
    }
}

void Network::SendMyFish(const ControlledFish& myFish) {
    sf::Packet packet;
    packet << myFish.GetPosition().x << myFish.GetPosition().y << static_cast<int> (myFish.GetType());
    socket.send(packet);
}