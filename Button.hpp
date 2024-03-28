#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

#include "HelpTools.hpp"

#include <cassert>

class Button
{
public:
    // Enums 
    enum Shape
    {
        ONLY_DATA = 0,
        RECTANGLE,
        OVAL
    };
    enum DataType
    {
        TEXT = 0,
        IMAGE
    };

    // Constructors
    Button(Shape buttonShape, DataType dataType);

    // Setters
    void setString(const std::string &textData);
    void setFocusedTextColor(const sf::Color &newColor);
    void setDefaultTextColor(sf::Color newColor);
    void setCharSize(int newSize);
    void setFont(const std::string &fontPath);
    void setImage(const sf::Image &image);
    void setPosition(const sf::Vector2f &pos);
    void setRectSize(const sf::Vector2f &newSize);
    void setButtonColor(const sf::Color &color);
    void setOutlineW(int w);

    // Getters
    sf::Vector2f getSize() const;
    const sf::Vector2f &getPosition() const;
    const sf::Text &getText() const;
    const sf::Sprite &getSprite() const;
    const sf::RectangleShape &getRect() const;

    // Logic functions
    void draw(sf::RenderWindow &window, bool selected=false);
    bool isSelected(sf::RenderWindow &window) const;

private:
    sf::Text m_text;
    sf::Color m_focusedTextColor = sf::Color::Black;
    sf::Color m_defaultTextColor = sf::Color::Cyan;
    sf::Color m_backgroundColor = sf::Color(200, 200, 200);
    sf::Font m_font;
    sf::RectangleShape m_rect;
    sf::Sprite m_sprite;
    sf::Texture m_texture;

    int m_outlineW = 0;
    Shape m_buttonShape;
    DataType m_dataType;
};

#endif // Button.hpp
