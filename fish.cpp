#include "fish.h"


Fish::Fish(const sf::Vector2f& pos, FishType type) {
    pos_ = pos;
    type_ = type;
    scale_ = type_scale[type];
    LoadSprite();
}

void Fish::LoadSprite() {
    switch(type_)
    {
        case FishType::L_1:
            if (!tex_.loadFromFile("L_1.png"))
                exit(-1);
            break;
        case FishType::L_2:
            if (!tex_.loadFromFile("L_2.png"))
                exit(-1);
            break;
        case FishType::L_3:
            if (!tex_.loadFromFile("L_3.png"))
                exit(-1);
            break;
        case FishType::L_4:
            if (!tex_.loadFromFile("L_4.png"))
                exit(-1);
            break;

    }
}



sf::Sprite Fish::GetSprite() const {
    return fish_;
}


sf::Vector2f Fish::GetPosition() const {
    return pos_;
}

FishType Fish::GetType() const {
    return type_;
}

AutomaticFish::AutomaticFish(const sf::Vector2f& pos, FishType type,const float& time): Fish(pos, type), time_created_(time) {
    speed_ = type_speed[type];
}



//return true if fish is outside window
bool AutomaticFish::Draw(const float &time, sf::RenderWindow& window) {
    pos_.x -= speed_.x * (time - time_created_);
    //усложнить траекторию!
    speed_.y = sin(5 * time) / ((5 + rand() % 5) + (10 + rand() % 10) * (time - time_created_));
    pos_.y += speed_.y * (time - time_created_);
    fish_.setTexture(tex_);
    fish_.setScale(scale_);
    fish_.scale(1.0f,-1.0f);
    fish_.setPosition(pos_.x, pos_.y);
    window.draw(fish_);
    return (pos_.x < 0);
}

ControlledFish::ControlledFish(const sf::Vector2f& pos, FishType type) : Fish(pos, type) {}

void ControlledFish::Rotate(sf::RenderWindow& window, sf::Vector2f d) {
    const double Pi = 3.14159f;
    double angle = 180 + atan2f(d.y, d.x) * 180.0 / Pi;
    fish_.setRotation(angle);
}

void ControlledFish::Move(sf::Vector2u TextureSize) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        pos_.y -= speed_;
        if (!IsInsideWindow(TextureSize, fish_.getPosition())) {
            pos_.y += speed_;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        pos_.y += speed_;
        if (!IsInsideWindow(TextureSize, fish_.getPosition())) {
            pos_.y -= speed_;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        pos_.x += speed_;
        if (!IsInsideWindow(TextureSize, fish_.getPosition())) {
            pos_.x -= speed_;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        pos_.x -= speed_;
        if (!IsInsideWindow(TextureSize, fish_.getPosition())) {
            pos_.y += speed_;
        }
    }

}

void ControlledFish::Laser(sf::RenderWindow& window, sf::Vector2f center, sf::Vector2f worldPos) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {        //laser
        sf::Vertex line[] =
                {
                        sf::Vertex(center),
                        //sf::Vertex(sf::Vector2f(MousePos.x, MousePos.y))
                        sf::Vertex(sf::Vector2f(worldPos.x, worldPos.y))
                };

        line->color = (sf::Color::Red);
        window.draw(line, 2, sf::Lines);
    }
}

void ControlledFish::Control(sf::Vector2u textureSize, sf::RenderWindow& window) {

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    sf::Vector2u fishSize = fish_.getTexture()->getSize();
    fish_.setOrigin(fishSize.x / 2, fishSize.y / 2);
    sf::Vector2f center = fish_.getPosition();
    sf::Vector2f d = sf::Vector2f(worldPos.x, worldPos.y) - center;

    Move(textureSize);
    Rotate(window,d);
    Laser(window, center, worldPos);
}

bool ControlledFish::isTouched(const AutomaticFish& autoFish) {
    return fish_.getGlobalBounds().intersects(autoFish.GetSprite().getGlobalBounds());
}


void ControlledFish::Eat(std::vector<AutomaticFish>& autoFish, std::vector<AutomaticFish>::iterator it_del) {
    points_ += type_points[(*it_del).GetType()];
    std::cout << "current points = " << points_ << std::endl;
    autoFish.erase(it_del);
    if (points_ >= type_limit_points[type_])
        ChangeType();
}

void ControlledFish::ChangeType() {
    int type = static_cast<int> (type_);
    type += 1;
    if (type > 4) {
        std::cout << "You WIN!!!" << std::endl;
        return;
    }
    type_ = static_cast<FishType> (type);
    scale_ = type_scale[type_];
    LoadSprite();
}


//return true if you died
bool ControlledFish::DetectFish(std::vector<AutomaticFish>& autoFish) {
    bool imDied = false;
    auto it = autoFish.begin();
    for (AutomaticFish& fish : autoFish) {
        if (isTouched(fish)) {
            if (type_ >= fish.GetType()) {
                Eat(autoFish, it);
            }
            else {
                imDied = true;
                break;
            }
        }
        it++;
    }
    return imDied;
}

void ControlledFish::Draw(sf::RenderWindow &window) {
    fish_.setTexture(tex_);
    fish_.setScale(scale_);
    fish_.setPosition(pos_.x, pos_.y);
    window.draw(fish_);
}




FishGeneration::FishGeneration() {}

FishType FishGeneration::GenerateType() {
    //чтобы было распределение "чем выше рыба по уровню, тем реже появляется"
    int genType = rand() % 100;
    FishType type;
    if (genType < 40)
        type = FishType::L_1;
    if (genType >=40 && genType < 70)
        type = FishType::L_2;
    if (genType >=70 && genType < 90)
        type = FishType::L_3;
    if (genType >=90 && genType < 100)
        type = FishType::L_4;
    return type;
}


void FishGeneration::Draw(const float &time, sf::RenderWindow& window) {

    for (int i = 0; i < autoFish.size(); ++i) {
        if (autoFish[i].Draw(time, window)) {
            autoFish.erase(autoFish.begin() + i);
        }
    }
}



void FishGeneration::GenerateFish(const float &time, sf::RenderWindow& window) {
    if (time > fish_creation_time) {
        float x = window.getSize().x + 700;
        float y = 10 + rand() % (window.getSize().y - 400);//?????
        FishType type = GenerateType();
        autoFish.push_back(AutomaticFish({x, y}, type, time));
        float dt = rand() % 3;
        fish_creation_time += dt;

    }
}

