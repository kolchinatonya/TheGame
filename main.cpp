#include "Fish.h"

#include <SFML/Network.hpp>
#include <iostream>
using namespace std;


void WaitClients(std::vector<sf::TcpSocket*>& clients) {
    sf::TcpListener listener;
    listener.listen(55001);
    sf::SocketSelector selector;
    selector.add(listener);
    while (clients.size() != 2) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                sf::TcpSocket *client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) {
                    clients.push_back(client);
                    sf::Packet packet;
                    packet << (int)clients.size();
                    for (auto cl : clients)
                        (*cl).send(packet);
                    cout << "clients count " << clients.size() << endl;
                    selector.add(*client);
                } else
                    delete client;
            }
        }
    }
}



void GetSendControlledFish(std::vector<sf::TcpSocket*>& clients) {
    vector<sf::Packet> packets = vector<sf::Packet>(2);
    for (int i = 0; i < 2; ++i) {
        sf::Vector2f pos;
        int packet_type = 0;
        int type;
        float angle;
        int directionType;
        float speed;
        sf::Packet packet;
        (*clients[i]).receive(packet);
        packet >> packet_type;
        if (packet_type == 1) {
            if (packet >> pos.x >> pos.y >> type >> angle >> directionType >> speed) {
                //std::cout << "got fish " <<  pos.x << " " << pos.y << std::endl;
                (*clients[!i]).send(packet);
            }
        }
    }
}




void SendFishGeneration(std::vector<sf::TcpSocket*>& clients)  {

}

int main() {
    std::vector<sf::TcpSocket*> clients;
    WaitClients(clients);
    FishGeneration gen;
    bool ok = true;
    while (ok) {
        gen.GenerateFish(time, fish.GetPosition());			//сюда передаем фон, чтобы с его обновлением рисовалось корректно
        gen.Draw(time, window);
        GetSendControlledFish(clients);
        //SendFishGeneration();
    }



    return 0;
}