#pragma once

#include "SFML/Graphics.hpp"
#include <SFML/Network.hpp>
#include "SFML/Audio.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>

#include <iterator>

//#include "BackGround.h"


std::string to_str(int a);

enum class FishType {
    L_1 = 1, L_2 = 2, L_3 = 3, L_4 = 4
};

const float life_time = 30.f;





enum class DirectionType {
    UP, DOWN, LEFT, RIGHT, NOWHERE
};


class Fish {
public:
    Fish();
    Fish(const sf::Vector2f& pos, FishType type);

    sf::Vector2f GetPosition() const;
    FishType GetType() const;

    virtual ~Fish() = default;
protected:
    sf::Vector2f pos_;
    FishType type_;
};

class AutomaticFish : public Fish {
public:
    AutomaticFish(const sf::Vector2f& pos, FishType type, const float& time);

private:
    float time_created_;
    float time_ = 0.f;
};

class AnotherPlayerFish : public Fish {
public:
    //AnotherPlayerFish();
    AnotherPlayerFish(const sf::Vector2f& pos, FishType type, float angle, DirectionType directionType, float speed);
    void Draw(sf::RenderWindow& window);
    void NetUpdate(const sf::Vector2f& pos, FishType type, float angle, DirectionType directionType, float speed);
    void UpdatePosition(const float& time);
    DirectionType GetDirectionType() const;

    ~AnotherPlayerFish() = default;
private:
    DirectionType current_direction = DirectionType::RIGHT;
    float time_last_updated = 0.f;
};


class FishGeneration {
public:
    FishGeneration();
    FishType GenerateType();
    void GenerateFish(const float &time, const float& max_pos_x, std::vector<sf::TcpSocket*>& clients);
    void SendFishGeneration(std::vector<sf::TcpSocket*>& clients, AutomaticFish);
    //void Draw(const float &time, sf::RenderWindow& window);
    //std::vector<AutomaticFish> autoFish;
    std::vector<AutomaticFish> autoCreature;
private:
    float last_creation_time = 0;
};