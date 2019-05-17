#include "Fish.h"

Fish::Fish() {}

Fish::Fish(const sf::Vector2f& pos, FishType type) {
    pos_ = pos;
    type_ = type;
}




sf::Vector2f Fish::GetPosition() const {
    return pos_;
}

FishType Fish::GetType() const {
    return type_;
}





AutomaticFish::AutomaticFish(const sf::Vector2f& pos, FishType type, const float& time) :
        Fish(pos, type), time_created_(time) {
}



DirectionType AnotherPlayerFish::GetDirectionType()  const {
    return current_direction;
}

FishGeneration::FishGeneration() {}

FishType FishGeneration::GenerateType() {
    //чтобы было распределение "чем выше рыба по уровню, тем реже появляется"
    int genType = rand() % 100;
    FishType type;
    if (genType < 40)
        type = FishType::L_1;
    if (genType >= 40 && genType < 70)
        type = FishType::L_2;
    if (genType >= 70 && genType < 90)
        type = FishType::L_3;
    if (genType >= 90 && genType < 100)
        type = FishType::L_4;

    return type;
}


void FishGeneration::SendFishGeneration(std::vector<sf::TcpSocket*>& clients, AutomaticFish new_gen_fish) {
    sf::Packet packet;
    packet << 2 << new_gen_fish.GetPosition().x << new_gen_fish.GetPosition().y << static_cast<int>(new_gen_fish.GetType());
    for (int i = 0; i < 2; ++i) {
        (*clients[i]).send(packet);
    }
}

void FishGeneration::GenerateFish(const float &time, const float& max_pos_x, std::vector<sf::TcpSocket*>& clients) {
    if (time > last_creation_time) {
        float x = max_pos_x + 600.f + rand() % 400;
        float y = 100 + rand() % 400;			//?????

        FishType type = GenerateType();

        autoCreature.push_back(AutomaticFish({ x, y }, type, time));
        SendFishGeneration(clients, AutomaticFish({ x, y }, type, time));
        float dt = rand() % 3;
        last_creation_time += dt;
    }
}

void FishGeneration::DeleteFish(int ind) {

    autoCreature.erase(autoCreature.begin() + ind);
}