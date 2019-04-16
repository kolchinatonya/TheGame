#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>
#include "fish.h"

//сюда добавить передачу текущего размера рыбы, чтобы она не заплывала плавником за границу аквариума.
bool IsInsideWindow(sf::Vector2u WSize, sf::Vector2f Position);


//maybe написать единую (шаблонную?) функцию для проверки взаимного расположения рыба-окно, рыба-крючок, рыба-другая рыба
bool IsOnTheHook(const sf::Sprite& sprite, const sf::Vector2f& hook_pos);
class Bubble {
public:
    Bubble(const sf::Vector2f& pos, const float& t);

    sf::Vector2f GetPos(const float& current_time);
    void Draw(sf::RenderWindow& wind, const float& current_time);

private:
    sf::Vector2f coordinates;
    sf::Vector2f speed;
    float time_created;
};

class Boat {
public:
    Boat(const float& x, const float& v);

    void SetSprite();

    void draw(const float& t, sf::RenderWindow& wind);

protected:
    float x_coordinate;
    float speed;
    sf::Sprite boat_;
    sf::Texture tex;
    //сюда добавить текстуру
};

class FisherBoat : public Boat {			//на конец "лески" нужно повесить спрайт крючка
public:
    FisherBoat(const float& x, const float& v);			//так же можно передать максимальную глубину крючка

    bool IsAttacking();

    void InitAttack();

    sf::Vector2f Attack(sf::RenderWindow& wind);

private:
    bool hook = true;
    //float time_attacked;
    float depth = 0;
};

class Background {				//responsible for setting background image and drawing bubbles
public:
    Background(sf::RenderWindow& window);

    void Bubbles(const float& time, sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);
    sf::Vector2u GetBackgroundTextureSize() const;

private:
    //background
    sf::Texture BackgroundTexture;
    sf::Sprite background;

    sf::Vector2u TextureSize;  //Added to store texture size.
    sf::Vector2u WindowSize;   //Added to store window size.

    //bubbles
    std::vector<Bubble> bubbles;
    double bubble_creation_time = 0;
};

