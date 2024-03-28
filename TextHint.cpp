#include "TextHint.hpp"

TextHint::TextHint() : TextHint("arial.ttf")
{}

TextHint::TextHint(const std::string &fontPath, const sf::Vector2f &boxSize) : m_textData(""), m_font(),
      m_charSize(14)
{
    this->setFont(fontPath);
    m_textRect.setSize(boxSize);
    m_fillColor = sf::Color::Black;
}

void TextHint::setCharSize(int newCharSize)
{
    m_charSize = newCharSize;
}

void TextHint::setFont(const std::string &fontPath)
{
    if (!m_font.loadFromFile(fontPath))
        throw std::invalid_argument("Cannot find <" + fontPath + "< in directory!");
    m_text.setFont(m_font);
}

void TextHint::setPosition(const sf::Vector2f &pos)
{
    m_pos = pos;
}

void TextHint::setString(const std::string &textData)
{
    m_textData = textData;
}

void TextHint::setSize(const sf::Vector2f &newSize)
{
    m_textRect.setSize(newSize);
}

void TextHint::setFillColor(const sf::Color &color)
{
    m_fillColor = color;
}

const std::string &TextHint::getString() const
{
    return m_textData;
}

const sf::Vector2f &TextHint::getSize() const
{
    return m_textRect.getSize();
}

const sf::Vector2f &TextHint::getPosition() const
{
    return m_pos;
}

const sf::RectangleShape &TextHint::getRect()
{
    return m_textRect;
}

void TextHint::draw(sf::RenderWindow &window)
{
    m_text.setCharacterSize(m_charSize);
    m_text.setFont(m_font);
    m_text.setFillColor(m_fillColor);
    m_text.setPosition(m_pos);
    m_text.setString(m_textData);
    window.draw(m_text);
}
