#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Snake.hpp"
#include "InputBox.hpp"
#include "Button.hpp"

#include <iostream>
#include <cassert>

class Application : public Snake
{
public:
    // Constructors
    Application(int width, int height, int snakeW=20, int snakeH=20, int fpsLimit=14);


    // Logic functions
    void playGame();
    void gameWindow(sf::RenderWindow &window);
    void exitWindow(sf::RenderWindow &window, const std::string &fontPath="arial.ttf");
    void initWindow(sf::RenderWindow &window, const std::string &fontPath="arial.ttf");
    void settingsWindow(sf::RenderWindow &window, const std::string &fontPath="arial.ttf");

private:
    // logic functions
    bool loadInfoFromInitWindow(sf::RenderWindow &widnow, const std::vector<InputBox> &inputBoxes);

    // Variables
    int m_fpsLimit;
    std::string m_userName = "Unknown";
};

#endif // application.hpp
