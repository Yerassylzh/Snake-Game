#include "Button.hpp"

// Constructors
Button::Button(Button::Shape buttonShape, Button::DataType dataType) :
    m_buttonShape(buttonShape), m_dataType(dataType), m_font(sf::Font()), m_rect(sf::RectangleShape()),
    m_sprite(sf::Sprite()), m_text(sf::Text()), m_texture(sf::Texture())
{}

// Setters
void Button::setString(const std::string &textData)
{
    m_text.setString(textData);
}

void Button::setFocusedTextColor(const sf::Color &newColor)
{
    m_focusedTextColor = newColor;
}

void Button::setDefaultTextColor(sf::Color newColor)
{
    m_defaultTextColor = newColor;
}

void Button::setCharSize(int newSize)
{
    m_text.setCharacterSize(newSize);
}

void Button::setFont(const std::string &fontPath)
{
    if (m_dataType == DataType::IMAGE)
        throw std::logic_error("Calling 'setFont(const std::string &) for the button, consisting only image!");
    if (!m_font.loadFromFile(fontPath))
        throw std::invalid_argument("Cannot find <" + fontPath + "> in directory");
    m_text.setFont(m_font);
}

void Button::setImage(const sf::Image &image)
{
    assert(m_texture.loadFromImage(image));
    m_sprite.setTexture(m_texture);
}

void Button::setPosition(const sf::Vector2f &pos)
{
    if (m_buttonShape == Shape::ONLY_DATA)
        m_text.setPosition(pos);
    else 
        m_rect.setPosition(pos);
}

void Button::setRectSize(const sf::Vector2f &newSize)
{
    if (m_buttonShape == Shape::ONLY_DATA) 
        throw std::logic_error("Cannot set rectangle size for button, which contains only text or image!");
    m_rect.setSize(newSize);
}

void Button::setButtonColor(const sf::Color &color)
{
    if (m_buttonShape == Shape::ONLY_DATA) 
        throw std::logic_error("Cannot set button fill color for button, which shape is 'only data'!");
    m_rect.setFillColor(color);
}

void Button::setOutlineW(int w)
{
    m_outlineW = w;
}

// Getters
sf::Vector2f Button::getSize() const 
{
    if (m_buttonShape == Shape::ONLY_DATA)
        return sf::Vector2f(m_text.getLocalBounds().width, m_text.getLocalBounds().height);
    return m_rect.getSize();
}

const sf::Vector2f &Button::getPosition() const
{
    if (m_buttonShape == Shape::ONLY_DATA)
        return m_text.getPosition();
    return m_rect.getPosition();
}

const sf::Text &Button::getText() const 
{
    if (m_dataType == DataType::IMAGE)
        throw std::logic_error("Cannot get the text for button, which contains only image!");
    return m_text;
}

const sf::Sprite &Button::getSprite() const 
{
    if (m_dataType == DataType::TEXT)
        throw std::logic_error("Cannot get sprite from button, which contains only text!");
    return m_sprite;
}

const sf::RectangleShape &Button::getRect() const
{
    return m_rect;
}

// Logic functions
void Button::draw(sf::RenderWindow &window, bool selected)
{
    switch (m_buttonShape)
    {
        case Shape::RECTANGLE:
            window.draw(m_rect);
            break;
        case Shape::OVAL:
            throw std::logic_error("oval button still doesn't implemented!");
    }

    switch (m_dataType)
    {
        case DataType::TEXT:
            m_text.setFillColor(selected ? m_focusedTextColor : m_defaultTextColor);
            if (m_buttonShape != Shape::ONLY_DATA)
            {
                m_text.setPosition(
                    m_rect.getPosition().x + (m_rect.getSize().x - m_text.getLocalBounds().width - m_outlineW) / 2.f, 
                    m_text.getPosition().y + (m_rect.getSize().y - m_text.getLocalBounds().height - m_outlineW) / 2.f
                );
            }
            window.draw(m_text);
            break;
        case DataType::IMAGE:
            int w = m_rect.getSize().x - m_outlineW;
            int h = m_rect.getSize().y - m_outlineW;
            m_sprite.setScale(
                w / m_sprite.getLocalBounds().width,
                h / m_sprite.getLocalBounds().height
            );
            m_sprite.setPosition(m_rect.getPosition());
            window.draw(m_sprite);
            break;
    }

    if (selected && m_buttonShape != Shape::ONLY_DATA)
        buttonPressedAnimation(window, sf::Cursor::Arrow);
}

bool Button::isSelected(sf::RenderWindow &window) const 
{
    if (m_buttonShape == Shape::ONLY_DATA)  
        throw std::logic_error("Calling isSelected(sf::RenderWindow&) for the button without particular shape!");
    const sf::Vector2f &cursorPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return isInsideOfRect(cursorPos, m_rect);
}