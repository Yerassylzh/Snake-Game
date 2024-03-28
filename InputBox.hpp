#ifndef INPUT_BOX_HPP
#define INPUT_BOX_HPP

#include "TextHint.hpp"

#include <string>

class InputBox : public TextHint
{
public:
    // Constructors
    InputBox();
    InputBox(const std::string &fontPath, const sf::Vector2f &boxSize=sf::Vector2f(90, 15));

    // Setters
    void setSelected();
    void setNotSelected();

    // Logical function
    bool addChar(sf::RenderWindow &window);
    void draw(sf::RenderWindow &window) override;

    // Others
    bool isSelected(sf::RenderWindow &window);
private:
    bool m_selected;
};

#endif // inputbox.hpp
