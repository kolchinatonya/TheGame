#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "BackGround.h"
#include "Network.h"

#include "SFML/Graphics.hpp"

//const std::string font_file = "C:/Users/User/MIPT/TheGame/varsity_regular.ttf";
const std::string button_file = "ButtonOne.png";


std::string to_str(int a);


class Button {
public:
    Button(const std::string& name, const float& y);		//íàçâàíèå êíîïêè è ãäå îíà ïî âûñîòå
    Button(const std::string& file_name, const float& x, const float& y);		//constructor for simple buttons without text
    void draw(sf::RenderWindow& window);					//îòðèñîâêà êíîïêè â îêíå
    void dynamicDraw(sf::RenderWindow& window, const sf::Vector2f& pos);
    bool IsClicked(const sf::Vector2f& MousePos);
    //void ResetTexture(const std::string& new_file);

private:
    void SetVectors();		//çàäàåò âåêòîðû, ñîäåðæàùèå îðèåíòàöèþ êíîïêè íà ýêðàíå

    sf::Vector2f Left_Top;			//ïî ýòèì äâóì âåêòîðàì îäíîçíà÷íî ïîíèìàåì ïîëîæåíèå êíîïêè è åå ðàçìåð
    sf::Vector2f Right_Bottom;		//íóæíû, ÷òîáû áûñòðî îïðåäåëÿòü, íàæàòà ëè êíîïêà

    sf::Texture button_texture;		//"ôîí" êíîïêè
    sf::Sprite button_sprite;

    WindowText button_text;			//text on a button made in a new way

    const std::string button_name;

    float y_position;				//ãäå ïî âûñîòå â ñòîëáèêå êíîïîê (ðàçíîñòü ëó÷øå äåëàòü îêîëî 100)
};

bool ShowMenu(sf::RenderWindow& window, Network& net, bool EntryMenu, bool& multiplayer_mode, const int& score);
//returns true if another restart cycle
//bool multiplayer_mode added to draw the second fish only in two players mode

//void ShowNetworkMenu(Network& net, sf::RenderWindow& window);