#include <iostream>
#include "background.h"
#include "fish.h"

int main()
{
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(1200, 720), "Best game ever!");

    Background background(window);

    sf::Vector2u TextureSize = background.GetBackgroundTextureSize();

    //setting view
    sf::View view_;
    view_.setCenter(sf::Vector2f(300.f, 200.f));
    view_.setSize(sf::Vector2f(600.f, 360.f));
    //end of view

    const float YRatio = (TextureSize.y - view_.getSize().y) / (TextureSize.y);		//магия для того, чтобы не вылетать
    //за текстуру фона

    ControlledFish fish({100, 100}, FishType::L_1);

    //boat
    FisherBoat boat(100.f, 0.001);
    FishGeneration gen;

    while (window.isOpen())
    {
        sf::Time time = clock.getElapsedTime();


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        //moving sprite towards left side of the window
        //sf::Vector2f deltaX(view_.getSize().x / 3, 0.f);
        //sf::Vector2f ratioY(0.f, )

        sf::Vector2f view_Center;


        view_Center.x = fish.GetSprite().getPosition().x + view_.getSize().x/ 3.f;
        view_Center.y = (TextureSize.y / 2.f) + ((fish.GetSprite().getPosition().y - TextureSize.y / 2) * YRatio);

        //view_.setCenter(circle.getPosition() + deltaX);		//making camera follow our sprite

        view_.setCenter(view_Center);
        window.setView(view_);						//!!!Dont forget or view is useless

        background.draw(window);
        background.Bubbles(time.asSeconds(), window);

        //boat - легко сделать вектор лодок, атакующих в разное время
        boat.draw(time.asSeconds(), window);
        fish.Draw(window);
        gen.GenerateFish(time.asSeconds(), window);
        gen.Draw(time.asSeconds(), window);
        if (fish.DetectFish(gen.autoFish)) {
            std::cout << "YOU ARE DIED!" << std::endl;
            // написать какой-то метод EndGame(), который будет все останавливать
        }

        sf::Vector2f hook_pos;
        if (boat.IsAttacking()) {
            hook_pos = boat.Attack(window);
            if (IsOnTheHook(fish.GetSprite(), hook_pos)) {
                std::cout << "You fucked up" << std::endl;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            boat.InitAttack();
        }

        fish.Control(TextureSize, window);


        window.display();
    }

    return 0;
}