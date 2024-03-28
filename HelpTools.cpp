#include "HelpTools.hpp"

#include <cassert>

bool isInsideOfRect(const sf::Vector2f &pos, const sf::RectangleShape &rect)
{
    int rectW = rect.getSize().x, rectH = rect.getSize().y;
    return rect.getPosition().x <= pos.x && pos.x <= rect.getPosition().x + rectW &&
        rect.getPosition().y <= pos.y && pos.y <= rect.getPosition().y + rectH;
}

int getAppropriateCharSize(const std::string &fontPath, int maxHeight, int maxWidth, int outlineW, const std::string &textData)
{
    sf::Font font;
    if (!font.loadFromFile(fontPath))
        throw std::invalid_argument("Cannot find <" + fontPath + "> in directory!");
    sf::Text text;
    text.setFont(font);
    text.setString(textData);

    int maxSize = 200;
    int minSize = 1;

    while (minSize < maxSize - 1)
    {
        int middle = maxSize + minSize >> 1;
        text.setCharacterSize(middle);
        float height = text.getLocalBounds().height;
        float width = text.getLocalBounds().width;
        if (height < maxHeight - outlineW * 4 || width < maxWidth - outlineW * 4)
            minSize = middle;
        else if (height > maxHeight - outlineW * 4 || width > maxWidth - outlineW * 4)
            maxSize = middle - 1;
        else
            return middle;
    }
    text.setCharacterSize(minSize);
    // assert((maxHeight - outlineW * 2) - text.getLocalBounds().height < 5);
    return minSize;
}

void buttonPressedAnimation(sf::RenderWindow &window, const sf::Cursor::Type &initState)
{
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Type::Hand);
    window.setMouseCursor(cursor);
    sf::sleep(sf::milliseconds(300));

    // Return cursor into it's init state
    sf::Cursor initCursor;
    assert(initCursor.loadFromSystem(initState));
    window.setMouseCursor(initCursor);
}
