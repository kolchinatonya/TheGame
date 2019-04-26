//#include "pch.h"

#include "Fish.h"

Fish::Fish(const sf::Vector2f& pos, FishType type) {
    pos_ = pos;
    type_ = type;
    scale_ = type_scale[type];
    LoadSprite();

}

void Fish::LoadSprite() {
    switch (type_)
    {
        case FishType::L_1:
            if (!tex_.loadFromFile(L1_file))
                exit(-1);
            break;
        case FishType::L_2:
            if (!tex_.loadFromFile(L2_file))
                exit(-1);
            break;
        case FishType::L_3:
            if (!tex_.loadFromFile(L3_file))
                exit(-1);
            break;
        case FishType::L_4:
            if (!tex_.loadFromFile(L4_file))
                exit(-1);
            break;
            //for boosting shrimp
        case FishType::SHRIMP:
            if (!tex_.loadFromFile(shrimp_file))
                exit(-1);
            break;
        case FishType::OYSTER:
            if (!tex_.loadFromFile(oyster_file))
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

AutomaticFish::AutomaticFish(const sf::Vector2f& pos, FishType type, const float& time) :
        Fish(pos, type), time_created_(time) {
    speed_ = type_speed[type];
}



//return true if fish is outside window
bool AutomaticFish::Draw(const float &time, sf::RenderWindow& window) {
    pos_.x -= speed_.x * (time - time_created_);
    //óñëîæíèòü òðàåêòîðèþ!
    speed_.y = sin(5 * time) / ((5 + rand() % 5) + (10 + rand() % 10) * (time - time_created_));
    pos_.y += speed_.y * (time - time_created_);
    fish_.setTexture(tex_);
    fish_.setScale(scale_);
    fish_.scale(1.0f, -1.0f);
    fish_.setPosition(pos_.x, pos_.y);
    window.draw(fish_);
    return (pos_.x < 0);
}

//additionally setting size of font used to display points added
ControlledFish::ControlledFish(const sf::Vector2f& pos, FishType type) : Fish(pos, type), add_points_(20) {
    LoadSounds();
}

void ControlledFish::LoadSounds() {
    if (!collect_point_sound_.loadFromFile("collect_point.wav"))
        exit(-1);
}

void ControlledFish::PlaySound(sf::SoundBuffer buffer) {
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
}



bool ControlledFish::CheckSharpRotate(DirectionType newDirectionType) {
    return (((previous_direction == DirectionType::UP) && (newDirectionType == DirectionType::DOWN)) ||
     ((previous_direction == DirectionType::DOWN) && (newDirectionType == DirectionType::UP)) ||
     ((previous_direction == DirectionType::LEFT) && (newDirectionType == DirectionType::RIGHT)) ||
     ((previous_direction == DirectionType::RIGHT) && (newDirectionType == DirectionType::LEFT)));
}

void ControlledFish::GradualRotate(DirectionType newDirectionType) {
    float end_angle = directionType_angle[newDirectionType];
    float sign = end_angle - angle_;
    if (angle_ != end_angle) {
        float d_angle;
        if (((angle_ >= 0) && (angle_ < 90)) && (end_angle == 270))
            angle_ = angle_ + 360 - 1;
        else
            if ((((angle_ < 360) && (angle_ > 270))) && (end_angle == 270))
                angle_ -= 1;
            else
                if ((((angle_ < 360) && (angle_ >= 270))) && (end_angle == 0))
                angle_ += 1;
        else {
            d_angle = (sign > 0) ? 1 : -1;
            angle_ += d_angle;
        }
    }
}

void ControlledFish::Rotate(DirectionType newDirectionType) {
    if (CheckSharpRotate(newDirectionType)) {
        angle_ = directionType_angle[newDirectionType];
    }
    GradualRotate(newDirectionType);
    previous_direction = newDirectionType;
}

void ControlledFish::Move(sf::Vector2u& TextureSize, Background& background, const float& current_time) {
    float dt = (current_time - previous_control_time);
    previous_control_time = current_time;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        Rotate(DirectionType::UP);
        pos_.y -= speed_ * dt;
        if (!IsInsideWindow(TextureSize, pos_)) {
            pos_.y += speed_ * dt;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        Rotate(DirectionType::DOWN);
        pos_.y += speed_ * dt;
        if (!IsInsideWindow(TextureSize, pos_)) {
            pos_.y -= speed_ * dt;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        Rotate(DirectionType::RIGHT);
        pos_.x += speed_ * dt;

        if (TextureSize.x < pos_.x + 500) {
            std::cout << "Background added" << std::endl;
            background.AddBackground();
            TextureSize = background.GetBackgroundTextureSize();		//äîáàâëÿåò î÷åðåäíîé êóñîê ôîíà
        }

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        Rotate(DirectionType::LEFT);
        pos_.x -= speed_ * dt;
        if (!IsInsideWindow(TextureSize, pos_)) {
            pos_.x += speed_ * dt;
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

void ControlledFish::Control(sf::Vector2u& textureSize, sf::RenderWindow& window, Background& background, const float& time) {

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    sf::Vector2u fishSize = fish_.getTexture()->getSize();
    fish_.setOrigin(10, fishSize.y / 2);
    sf::Vector2f center = fish_.getPosition();
    sf::Vector2f d = sf::Vector2f(worldPos.x, worldPos.y) - center;

    //ñþäà äîáàâëÿþ ïåðåäà÷ó ôîíà, ÷òîáû âîâðåìÿ åãî ïðîäëåâàòü
    Move(textureSize, background, time);
    Laser(window, center, worldPos);
}


//additionally getting time for "adding points animation"
void ControlledFish::Eat(std::vector<AutomaticFish>& autoFish,
                         std::vector<AutomaticFish>::iterator it_del, const float& time) {

    delta_pts_ = type_points[(*it_del).GetType()];

    points_ += delta_pts_;
    plus_pts_string = "+ " + to_str(delta_pts_);

    std::cout << "current points = " << points_ << std::endl;

    time_fish_eaten_ = time;

    autoFish.erase(it_del);//seems OK
    PlaySound(collect_point_sound_);

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
//template<typename T>
bool ControlledFish::DetectFish(std::vector<AutomaticFish>& autoFish, const float& time) {
    //it is here for now, need to find another place (probably)
    CheckBoost(time);

    bool imDied = false;
    auto it = autoFish.begin();

    //fixing simultaneous iteration and deleting from one vector
    std::vector<AutomaticFish>::iterator to_eat;
    bool poedanie = false;

    while (it != autoFish.end()) {
        if (isTouched(*it)) {
            /*if ((*it).GetType() == FishType::SHRIMP) {
                to_eat = it;
                poedanie = true;
                break;
            }*/
            if (type_ >= (*it).GetType()) {
                to_eat = it;
                poedanie = true;
            }
            else {
                imDied = true;
                break;
            }
        }
        it++;
    }

    if (poedanie) {
        Eat(autoFish, to_eat, time);
    }
    /*for (AutomaticFish& fish : autoFish) {
        if (isTouched(fish)) {
            if (type_ >= fish.GetType()) {
                Eat(autoFish, it);
            }
            else {
                imDied = true;
                break;
            }
        }
        //it++;
    }*/
    return imDied;
}

void ControlledFish::Draw(sf::RenderWindow &window, const float& time) {
    fish_.setTexture(tex_);
    fish_.setScale(scale_);
    fish_.setPosition(pos_.x, pos_.y);
    fish_.setRotation(angle_);
    if (((angle_ >= 0) && (angle_ < 90)) || ((angle_ > 270) && (angle_ < 360)))
        fish_.scale(1.0f, -1.0f);
    window.draw(fish_);

    if (time - time_fish_eaten_ < 0.75f)
        PointsAnimation(window, time);
}


void ControlledFish::PointsAnimation(sf::RenderWindow& window, const double& time) {
    sf::Vector2f delta_pts_pos;
    delta_pts_pos.x = pos_.x;
    delta_pts_pos.y = pos_.y - tex_.getSize().y * scale_.y / 2 - 75 * (time - time_fish_eaten_);	//going up effect

    add_points_.Display(window, delta_pts_pos, plus_pts_string);
}


int ControlledFish::GetScore() {
    return points_;
}





FishGeneration::FishGeneration() {}

FishType FishGeneration::GenerateType() {
    //÷òîáû áûëî ðàñïðåäåëåíèå "÷åì âûøå ðûáà ïî óðîâíþ, òåì ðåæå ïîÿâëÿåòñÿ"
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

    if (genType > 100)
        type = FishType::SHRIMP;

    return type;
}

/*
void FishGeneration::Draw(const float &time, sf::RenderWindow& window) {

	for (int i = 0; i < autoFish.size(); ++i) {
		if (autoFish[i].Draw(time, window)) {
			autoFish.erase(autoFish.begin() + i);
		}
	}
}
*/

//ñþäà òåïåðü ïåðåäàåì ñàìîãî ïåðñà, ÷òîáû íà ðàññòîÿíèè îò íåãî ãåíåðèëèñü ðûáû
//à íå íà êîíöå îêíà(èíà÷å áåäà)

void FishGeneration::GenerateFish(const float &time, const sf::Vector2f& current_fish) {
    if (time > last_creation_time) {
        float x = current_fish.x + 600.f + rand() % 400;
        float y = 100 + rand() % 400;			//?????

        FishType type = GenerateType();

        autoCreature.push_back(AutomaticFish({ x, y }, type, time));

        float dt = rand() % 3;
        last_creation_time += dt;
    }
}