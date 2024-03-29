#include <SFML/Window.hpp>

#include "Snake.hpp"

#include <cassert>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <stdexcept>
#include <memory>

void Snake::drawField(sf::RenderWindow &window)
{
    // drawing the field background
    sf::RectangleShape snakeShapes[2];
    snakeShapes[0].setFillColor({140, 214, 81});
    snakeShapes[1].setFillColor({168, 214, 80});
    snakeShapes[0].setSize(sf::Vector2f(m_snakeW, m_snakeH));
    snakeShapes[1].setSize(sf::Vector2f(m_snakeW, m_snakeH));

    for (int x = 0; x < m_width; ++x)
    {
        int mode = (x % 2);
        for (int y = 0; y < m_height; ++y, mode ^= 1)
        {
            snakeShapes[mode].setPosition(sf::Vector2f(x * m_snakeW, y * m_snakeH));
            window.draw(snakeShapes[mode]);
        }
    }

    // drawing the snake
    std::unique_ptr<sf::Shape> snShape;
    if (m_snakeShape == SnakeShape::CIRCLE)
        snShape.reset(new sf::CircleShape(std::min(m_snakeW / 2.f, m_snakeH / 2.f)));
    else
        snShape.reset(new sf::RectangleShape(sf::Vector2f(m_snakeW, m_snakeH)));
    snShape->setFillColor(sf::Color::Blue);

    for (auto it = m_snakePositions.cbegin(); it != m_snakePositions.cend(); ++it)
    {
        snShape->setPosition(it->x * m_snakeW, it->y * m_snakeH);
        window.draw(*snShape);
    }

    snShape->setFillColor(sf::Color::Red);
    snShape->setPosition(m_applePosition.x * m_snakeW, m_applePosition.y * m_snakeH);

    window.draw(*snShape);

    // drawing the borders(bricks)
    for (const auto &pos : m_notAllowed)
    {
        m_brickSprite.setPosition(pos.x * m_snakeW, pos.y * m_snakeH);
        window.draw(m_brickSprite);
    }
}

Snake::Snake(int width, int height, int snakeW, int snakeH) : m_width(width), m_height(height),
      m_snakeW(snakeW), m_snakeH(snakeH), m_score(0),
      m_snakePositions(std::deque<sf::Vector2i>()), m_snakeDir(Direction::UP),
      m_snakePositionsSet(std::set<sf::Vector2i, Vector2Compare<sf::Vector2i>>()),
      m_notAllowed(std::set<sf::Vector2i, Vector2Compare<sf::Vector2i>>()),
      m_gameOver(false), m_snakeShape(SnakeShape::RECTANGLE)
{
    // Setting up snake and apple position
    srand(static_cast<uint32_t>(time(nullptr)));
    m_snakePositions.push_back({m_width / 2, m_height / 2});
    m_snakePositionsSet.insert({m_width / 2, m_height / 2});
    
    // Setting up brick wall position
    if (width > 70 || height > 70)
        throw std::invalid_argument("Too big game window!");

    for (int x = 0; x < width / 4; ++x)
        m_notAllowed.insert({x, 0}), m_notAllowed.insert({x, height - 1});
    for (int x = width * 3 / 4; x < width; ++x)
        m_notAllowed.insert({x, 0}), m_notAllowed.insert({x, height - 1});
    for (int y = 0; y < height / 4; ++y)
        m_notAllowed.insert({0, y}), m_notAllowed.insert({width - 1, y});
    for (int y = height * 3 / 4; y < height; ++y)
        m_notAllowed.insert({0, y}), m_notAllowed.insert({width - 1, y});

    m_applePosition = genApplePosition();
    
    // Setting up brick sprite
    if (!m_brickTexture.loadFromFile("Brick.png"))
        throw std::invalid_argument("Cannot find <Brick.png> in directory!");

    m_brickSprite.setTexture(m_brickTexture);
    m_brickSprite.setScale(
        static_cast<float>(m_snakeW) / m_brickTexture.getSize().x,
        static_cast<float>(m_snakeH) / m_brickTexture.getSize().y
    );
}

Snake::Snake() : Snake(20, 30)
{}

sf::Vector2i Snake::genApplePosition()
{
    while (true)
    {
        int x = rand() % m_width;
        int y = rand() % m_height;
        if (m_snakePositionsSet.find({x, y}) == m_snakePositionsSet.end()
              && m_notAllowed.find({x, y}) == m_notAllowed.end())
            return sf::Vector2i(x, y);
    }
}

void Snake::moveOneStep()
{
    m_snakePositionsSet.erase(m_snakePositions.back());

    sf::Vector2i old_back_position = m_snakePositions.back();

    for (auto it = (m_snakePositions.end() - 1); it != m_snakePositions.begin(); --it)
        *it = *(it - 1);

    int new_x = m_snakePositions.front().x, new_y = m_snakePositions.front().y;
    if (m_snakeDir == Direction::UP)
        new_y = (new_y == 0 ? m_height - 1 : new_y - 1);
    else if (m_snakeDir == Direction::RIGHT)
        new_x = (new_x == m_width - 1 ? 0 : new_x + 1);
    else if (m_snakeDir == Direction::DOWN)
        new_y = (new_y == m_height - 1 ? 0 : new_y + 1);
    else if (m_snakeDir == Direction::LEFT)
        new_x = (new_x == 0 ? m_width - 1 : new_x - 1);
    else
        throw std::invalid_argument("Snake cannot move to undefinite direction!");

    m_snakePositions.front() = {new_x, new_y};
    m_snakePositionsSet.insert(m_snakePositions.front());

    if (m_notAllowed.count(m_snakePositions.front()) > 0)
    {
        m_gameOver = true;
        return;
    }

    if (m_snakePositions.front() == m_applePosition)
    {
        m_snakePositions.push_back(old_back_position);
        m_snakePositionsSet.insert(old_back_position);
        m_score++;
        m_applePosition = genApplePosition();
    }
    if (m_snakePositionsSet.size() == m_snakePositions.size() - 1)
        m_gameOver = true;
}

bool Snake::isGameOver()
{
    return m_gameOver;
}

int Snake::getScore()
{
    return m_score;
}

int Snake::getWidth()
{
    return m_width;
}

int Snake::getHeight()
{
    return m_height;
}

bool Snake::setSnakeDir(Direction dir)
{
    if (m_snakeDir == Direction::UP && dir == Direction::DOWN
          || m_snakeDir == Direction::DOWN && dir == Direction::UP
          || m_snakeDir == Direction::LEFT && dir == Direction::RIGHT
          || m_snakeDir == Direction::RIGHT && dir == Direction::LEFT)
        return false;

    m_snakeDir = dir;
    return true;
}

sf::Vector2i Snake::getApplePosition()
{
    return m_applePosition;
}

void Snake::setWidth(int newWidth)
{
    this->m_width = newWidth;
}

void Snake::setHeight(int newHeight)
{
    this->m_height = newHeight;
}

