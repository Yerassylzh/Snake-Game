#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SFML/Graphics.hpp>
#include "HelpTools.hpp"

#include <stdexcept>
#include <set>
#include <deque>
#include <utility>

class Snake
{
public:
    // Enums
    enum Direction
    {
        UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3,
        UNDEF = 4
    };

    enum SnakeShape
    {
        RECTANGLE = 0,
        CIRCLE
    };

    void moveOneStep();
    void setWidth(int newWidth);
    void setHeight(int newHeight);

    Snake();
    Snake(int width, int height, int snakeW=20, int snakeH=20);

    int getScore();
    int getWidth();
    int getHeight();

    bool setSnakeDir(Direction dir);
    bool isGameOver();

    sf::Vector2i getApplePosition();

protected:
    std::deque<sf::Vector2i> m_snakePositions;
    std::set<sf::Vector2i, Vector2Compare<sf::Vector2i>> m_snakePositionsSet;
    std::set<sf::Vector2i, Vector2Compare<sf::Vector2i>> m_notAllowed;
    sf::Vector2i m_applePosition;
    Direction m_snakeDir;
    int m_score;
    int m_width;
    int m_height;
    bool m_gameOver;
    int m_snakeW, m_snakeH;
    SnakeShape m_snakeShape;

    void drawField(sf::RenderWindow &window);
    sf::Vector2i genApplePosition();

    sf::Sprite m_brickSprite;
    sf::Texture m_brickTexture;
};

#endif // snake.hpp
