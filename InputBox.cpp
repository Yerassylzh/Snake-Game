#include "InputBox.hpp"
#include "HelpTools.hpp"

#include <stdexcept>
#include <cassert>
#include <iostream>

InputBox::InputBox() : InputBox("arial.ttf")
{}

InputBox::InputBox(const std::string &fontPath, const sf::Vector2f &boxSize) : TextHint(fontPath, boxSize),
      m_selected(false)
{}

// Unfocus if mouse button or esc pressed. Returns false if unfocused
bool InputBox::addChar(sf::RenderWindow &window)
{
    assert(m_selected);
    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    return false;
                else if (event.key.code == sf::Keyboard::Key::BackSpace)
                {
                    if (m_textData.size() > 0)
                        m_textData.pop_back();
                    return true;
                }
                int keyCode = event.key.code;
                if (sf::Keyboard::Key::A <= keyCode && keyCode <= sf::Keyboard::Key::Z)
                    m_textData.push_back(static_cast<int>('a') + keyCode - sf::Keyboard::Key::A);
                else if (sf::Keyboard::Key::Num0 <= keyCode && keyCode <= sf::Keyboard::Key::Num9)
                    m_textData.push_back(std::to_string(keyCode - sf::Keyboard::Key::Num0)[0]);
                else if (keyCode == sf::Keyboard::Key::Space)
                    m_textData.push_back(' ');
                else
                    m_textData.push_back('?');

                return true;
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                return false;
            else if (event.type == sf::Event::Closed)
                exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_SUCCESS);
}

void InputBox::setSelected()
{
    m_selected = true;
}

void InputBox::setNotSelected()
{
    m_selected = false;
}

void InputBox::draw(sf::RenderWindow &window)
{
    if (m_selected)
        m_text.setString(m_textData + "_");
    else
        m_text.setString(m_textData);

    if (m_text.getLocalBounds().width > m_textRect.getSize().x
          || m_text.getLocalBounds().height > m_textRect.getSize().y)
        throw std::invalid_argument("Box size is too small for text");

    m_text.setPosition(m_pos.x + (m_textRect.getSize().x - m_text.getLocalBounds().width) / 2.f,
          m_pos.y + (m_textRect.getSize().y - m_text.getLocalBounds().height) / 2.f);
    m_text.setFillColor(m_fillColor);
    m_textRect.setPosition(m_pos);
    m_textRect.setFillColor(sf::Color::White);

    window.draw(m_textRect);
    window.draw(m_text);
}

// Make sure that mouse left button has pressed
bool InputBox::isSelected(sf::RenderWindow &window)
{
    sf::Vector2f cursorPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return isInsideOfRect(cursorPos, m_textRect);
}
