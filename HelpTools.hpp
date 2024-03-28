#ifndef HELP_TOOLS_HPP
#define HELP_TOOLS_HPP

#include <SFML/Graphics.hpp>

#include <string>

bool isInsideOfRect(const sf::Vector2f &pos, const sf::RectangleShape &rect);
int getAppropriateCharSize(const std::string &fontPath, int maxHeight, int maxWidth, int outlineW, const std::string &textData="A");
void buttonPressedAnimation(sf::RenderWindow &window, const sf::Cursor::Type &initState);

template<typename T>
struct Vector2Compare
{
    bool operator()(const T &a, const T &b) const
    {
        return (a.x < b.x || (a.x == b.x && a.y < b.y));
    }
};

#endif // HelpTools.hpp
