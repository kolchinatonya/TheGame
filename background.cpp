#include "background.h"

//сюда добавить передачу текущего размера рыбы, чтобы она не заплывала плавником за границу аквариума.
bool IsInsideWindow(sf::Vector2u WSize, sf::Vector2f Position) {
    if (Position.x >= 0 && Position.y >= 0 &&
        Position.x <= WSize.x && Position.y <= WSize.y) {
        return true;
    } else {
        return false;
    }
}

//maybe написать единую (шаблонную?) функцию для проверки взаимного расположения рыба-окно, рыба-крючок, рыба-другая рыба

bool IsOnTheHook(const sf::Sprite &sprite, const sf::Vector2f &hook_pos) {
    if (abs(sprite.getPosition().x - hook_pos.x) < sprite.getTexture()->getSize().x * sprite.getScale().x &&
        abs(sprite.getPosition().y - hook_pos.y) < sprite.getTexture()->getSize().y * sprite.getScale().y) {
        return true;
    }
    return false;
}


Bubble::Bubble(const sf::Vector2f &pos, const float &t) {
    coordinates = pos;
    time_created = t;
    speed.y = (float) -0.05;
    //speed.x = 0.1* (-10 + rand() % 20);
}

sf::Vector2f Bubble::GetPos(const float &current_time) {
    float dt = current_time - time_created;

    //making bubbles go like sin(t)
    speed.x = sin(5 * current_time) / (5 + 10 * dt);

    coordinates = coordinates + speed;
    return coordinates;
}

void Bubble::Draw(sf::RenderWindow &wind, const float &current_time) {
    coordinates = GetPos(current_time);

    sf::CircleShape bub(5.f);
    bub.setFillColor(sf::Color(255, 255, 255, 90));
    bub.setPosition(coordinates.x, coordinates.y);
    wind.draw(bub);
}


Boat::Boat(const float &x, const float &v) {
    x_coordinate = x;
    speed = v;
    SetSprite();
    //std::cout << "boat created" << std::endl;
}

void Boat::SetSprite() {
    if (!tex.loadFromFile("speed-boat.png")) {
        std::cout << "boat" << std::endl;
        exit(-1);
    }
    //boat_.setTexture(tex);
}

void Boat::draw(const float &t, sf::RenderWindow &wind) {
    x_coordinate += speed * t;

    /*sf::RectangleShape boat;
    boat.setSize({ 100.f, 20.f });
    boat.setPosition(x_coordinate, 20.f);
    boat.setFillColor(sf::Color(128, 128, 0));*/

    //sf::Sprite boat(boat_figure);

    boat_.setTexture(tex);
    boat_.setScale(0.2f, 0.2f);
    boat_.setPosition(x_coordinate, -100.f);
    wind.draw(boat_);
}


FisherBoat::FisherBoat(const float &x, const float &v) : Boat(x, v) {}

bool FisherBoat::IsAttacking() {
    if (depth >= 20) {
        return true;
    }
    return false;
}

void FisherBoat::InitAttack() {        //setting params to start attack
    depth = 20;
    hook = true;
}

sf::Vector2f FisherBoat::Attack(sf::RenderWindow &wind) {
    sf::Vertex line[] =                        //леска
            {
                    sf::Vertex(sf::Vector2f(x_coordinate, 20.f)),
                    sf::Vertex(sf::Vector2f(x_coordinate, (float) depth))
            };
    if (hook) {
        depth += (float) 0.5;
    } else {
        depth -= (float) 0.5;
    }

    wind.draw(line, 2, sf::Lines);
    if (depth == 300) {
        hook = false;
    }
    return sf::Vector2f(x_coordinate, depth);
}

Background::Background(sf::RenderWindow &window) {
    if (!BackgroundTexture.loadFromFile("panorama.jpg")) {
        std::cout << "background" << std::endl;
        exit(-1);
    } else {
        TextureSize = BackgroundTexture.getSize(); //Get size of texture.
        WindowSize = window.getSize();

        //можно слегка растянуть фон :)
        //float ScaleX = (float)WindowSize.x / TextureSize.x;

        background.setTexture(BackgroundTexture);
        //background.setScale(ScaleX, ScaleY);      //Set scale.
    }
}

void Background::Bubbles(const float &time, sf::RenderWindow &window) {
    //initializing bubbles vector
    float make_x = 200 + rand() % 201;
    sf::Vector2f pos(make_x, 500.0);

    //making bubbles at random interval
    if (time > bubble_creation_time) {
        bubbles.push_back(Bubble(pos, time));
        float dt = 1.5 + 0.1 * (rand() % 11);
        bubble_creation_time += dt;
    }

    //drawing bubbles and erasing unneeded ones
    for (int i = 0; i < bubbles.size(); i++) {
        bubbles[i].Draw(window, time);
        if (!IsInsideWindow(window.getSize(), bubbles[i].GetPos(time))) {
            bubbles.erase(bubbles.begin() + i);
        }
    }

}

void Background::draw(sf::RenderWindow &window) {
    window.draw(background);
}

sf::Vector2u Background::GetBackgroundTextureSize() const {
    return TextureSize;
}


