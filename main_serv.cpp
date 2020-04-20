#include "Fish.h"

#include <SFML/Network.hpp>
#include <iostream>
#include <algorithm>
using namespace std;

vector<float> pos_x = {0.f, 0.f};

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


void GetSendControlledFish(std::vector<sf::TcpSocket*>& clients, FishGeneration& gen) {
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
                pos_x[i] = pos.x;
                //std::cout << "got fish " <<  pos.x << " " << pos.y << std::endl;
                (*clients[!i]).send(packet);
            }
        }
        if (packet_type == 3)
        {
            int ind;
            packet >> ind;
            gen.DeleteFish(ind);
            (*clients[!i]).send(packet);
        }
    }
}






int main() {
    std::vector<sf::TcpSocket*> clients;
    WaitClients(clients);
    FishGeneration gen;
    bool ok = true;
    sf::Clock clock;
    while (ok) {
        sf::Time time = clock.getElapsedTime();
        gen.GenerateFish(time.asSeconds(), max(pos_x[0], pos_x[1]), clients);			//??
        GetSendControlledFish(clients, gen);
    }



    return 0;
}
