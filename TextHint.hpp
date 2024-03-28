#ifndef TEXT_HINT_HPP
#define TEXT_HINT_HPP

#include <SFML/Graphics.hpp>

class TextHint
{
public:
    // Constructor(s)
    TextHint();
    TextHint(const std::string &fontPath, const sf::Vector2f &boxSize=sf::Vector2f(90, 15));

    // Setters
    void setCharSize(int newCharSize);
    void setFont(const std::string &fontPath);
    void setPosition(const sf::Vector2f &pos);
    void setString(const std::string &textData);
    void setSize(const sf::Vector2f &newSize);
    void setFillColor(const sf::Color &color);

    // Getters
    const std::string &getString() const;
    const sf::Vector2f &getSize() const;
    const sf::Vector2f &getPosition() const;
    const sf::RectangleShape &getRect();

    // Logical functions
    virtual void draw(sf::RenderWindow &window);
protected:
    // Variables
    sf::RectangleShape m_textRect;
    std::string m_textData;
    sf::Text m_text;
    sf::Font m_font;
    sf::Color m_fillColor;
    int m_charSize;
    int m_oulineW;
    sf::Vector2f m_pos;
};

#endif // TextHint.hpp
