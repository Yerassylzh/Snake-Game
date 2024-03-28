#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include "Application.hpp"

#include <iostream>
#include <fstream>

void Application::playGame()
{
    sf::RenderWindow window(sf::VideoMode(m_width * m_snakeW, m_height * m_snakeH), "Snake Game");

    window.setFramerateLimit(m_fpsLimit);

    this->initWindow(window);
}

void Application::gameWindow(sf::RenderWindow &window)
{
    m_snakePositions.clear();
    m_snakePositionsSet.clear();
    m_snakeDir = Direction::UP;
    m_score = 0;

    m_snakePositions.push_back({m_width / 2, m_height / 2});
    m_snakePositionsSet.insert(m_snakePositions.front());
    this->m_gameOver = false;

    Direction dir = Direction::UP;
    while (window.isOpen() && !this->isGameOver())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Key::W)
                    dir = Direction::UP;
                else if (event.key.code == sf::Keyboard::Key::D)
                    dir = Direction::RIGHT;
                else if (event.key.code == sf::Keyboard::Key::S)
                    dir = Direction::DOWN;
                else if (event.key.code == sf::Keyboard::Key::A)
                    dir = Direction::LEFT;
                else if (event.key.code == sf::Keyboard::Key::Escape)
                {
                    // Go to init window
                    return;
                }
            }
            if (event.type == sf::Event::Closed)
                exit(EXIT_SUCCESS);
        }
        window.clear();

        this->setSnakeDir(dir);

        this->moveOneStep();

        this->drawField(window);

        window.display();
    }
    this->exitWindow(window);
    exit(EXIT_SUCCESS);
}

Application::Application(int width, int height, int snakeW, int snakeH, int fpsLimit)
    : Snake(width, height, snakeW, snakeH), m_fpsLimit(fpsLimit)
{}

void Application::exitWindow(sf::RenderWindow &window, const std::string &fontPath)
{
    // Setting the background color
    const sf::Color backgroundColor(255, 255, 255);
    sf::Image image;
    image.create(m_width * m_snakeW, m_height * m_snakeH, backgroundColor);
    sf::Texture backgroundTexture;
    assert(backgroundTexture.loadFromImage(image));
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Loading information
    std::map<int, std::set<std::string>> scores;
    std::ifstream readDevice("Scores.txt", std::ifstream::in);
    if (!readDevice.is_open())
        throw std::invalid_argument("Cannot find <Scores.txt> in directory!");

    std::string name;
    int score;
    while (true)
    {
        readDevice >> name;
        readDevice >> score;
        if (readDevice.fail())
            break;
        scores[score].insert(name);
    }
    scores[m_score].insert(m_userName);
    readDevice.close();

    std::ofstream writeDevice("Scores.txt", std::ios::app);
    if (!scores.empty())
        for (const auto &[score, names] : scores)
        {
            if (names.empty())
                continue;
            for (const auto &name : names)
                writeDevice << name << " " << score << "\n";
        }
    writeDevice.close();

    TextHint titleHint;
    titleHint.setSize(
        sf::Vector2f(
            0.5 * m_width * m_snakeW, 0.07 * m_height * m_snakeH
        )
    );
    titleHint.setCharSize(getAppropriateCharSize("arial.ttf", titleHint.getSize().y, titleHint.getSize().x, 10, "Below you can see the scores"));
    titleHint.setPosition(
        sf::Vector2f(
            (m_width * m_snakeW - titleHint.getSize().x) / 2.f,
            10
        )
    );
    titleHint.setString("Below you can see the scores");

    // Setting up nesessary information
    std::vector<TextHint> records;
    for (auto I = scores.rbegin(); I != scores.rend() && records.size() < 3; ++I)
    {
        std::string msg = std::to_string(records.size() + 1) + ". " + *I->second.begin();

        for (auto it = ++(I->second.begin()); it != I->second.end(); ++it)
            for (char c : std::string(", " + *it))
                msg.push_back(c);
        msg += ": " + std::to_string(I->first);

        TextHint temp("arial.ttf", sf::Vector2f(0.5 * m_width * m_snakeW, 0.07 * m_height * m_snakeH));
        temp.setString(msg);
        temp.setCharSize(getAppropriateCharSize("arial.ttf", temp.getSize().y, temp.getSize().x, 40, msg));
        
        if (records.size() == 0)
        {
            temp.setPosition(
                sf::Vector2f(
                    (m_width * m_snakeW - temp.getSize().x) / 2.f,
                    0.25 * m_height * m_snakeH
                )
            );
        }     
        else
        {
            temp.setCharSize(getAppropriateCharSize("arial.ttf", records.front().getSize().y, temp.getSize().x, 40, records.front().getString()));
            temp.setPosition(
                sf::Vector2f(records.back().getPosition().x, records.back().getPosition().y + records.back().getSize().y)
            );
        }     
        temp.setFillColor(sf::Color::Black);

        records.push_back(temp);     
    }

    Button okButton(Button::Shape::RECTANGLE, Button::DataType::IMAGE);
    sf::Image okButtonImage;
    if (!okButtonImage.loadFromFile("OKButton.png"))
        throw std::invalid_argument("Cannot find <OKButton.png> in directory!");
    okButton.setImage(okButtonImage);
    okButton.setRectSize(sf::Vector2f(records.back().getSize().x / 3.f, records.back().getSize().y * 0.6f));
    okButton.setPosition(
        sf::Vector2f(
            (m_width * m_snakeW - okButton.getSize().x) / 2.f,
            records.back().getPosition().y + records.back().getSize().y
        )
    );

    sf::Event event;
    while (window.isOpen())
    {
        bool wasMouseClick = false; 
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    exit(EXIT_SUCCESS);
            }
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !wasMouseClick)
            {
                sf::Vector2f cursorPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (isInsideOfRect(cursorPos, okButton.getRect()))
                {
                    buttonPressedAnimation(window, sf::Cursor::Arrow);
                    // Go back to the init window
                    this->initWindow(window);
                    exit(EXIT_SUCCESS);
                }
                wasMouseClick = true;
            }
        }

        window.clear();

        window.draw(backgroundSprite);
        
        titleHint.draw(window);
        
        for (auto &hint : records)
            hint.draw(window);

        okButton.draw(window);

        window.display(); 
    }
    exit(EXIT_SUCCESS);
}

void Application::initWindow(sf::RenderWindow &window, const std::string &fontPath)
{
    // loading background image and setting option buttons
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("InitWindow.png"))
        throw std::invalid_argument("Cannot find 'InitWindow.png' in directory!");
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(m_width) * m_snakeW / backgroundTexture.getSize().x,
        static_cast<float>(m_height) * m_snakeH / backgroundTexture.getSize().y
    );

    int buttonsDist = 30;
    int focus = 1;

    Button playButton(Button::Shape::ONLY_DATA, Button::DataType::TEXT);
    playButton.setString("Start game");
    playButton.setFont("arial.ttf");
    const float playButtonCharSize = getAppropriateCharSize("arial.ttf", m_width * m_snakeW * 0.08, 
        m_height * m_snakeH * 0.12, 5, "Start game");
    playButton.setCharSize(playButtonCharSize);
    float playButtonW = playButton.getSize().x;
    float playButtonH = playButton.getSize().y;
    playButton.setPosition(
        sf::Vector2f(
            m_width * m_snakeW / 2.f - playButtonW / 2.f,
            m_height * m_snakeH / 2.f - buttonsDist / 2.f - playButtonH
        )
    );

    Button settingsButton(Button::Shape::ONLY_DATA, Button::DataType::TEXT);
    settingsButton.setString("Settings");
    settingsButton.setFont("arial.ttf");
    settingsButton.setCharSize(playButtonCharSize);
    float settingsButtonW = settingsButton.getSize().x;
    float settingsButtonH = settingsButton.getSize().y;
    settingsButton.setPosition(
        sf::Vector2f(
            m_width * this->m_snakeW / 2.0 - settingsButtonW / 2.0,
            m_height * this->m_snakeH / 2.0 + buttonsDist / 2.0
        )
    );
    
    // Interaction
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                exit(EXIT_SUCCESS);
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Key::Up)
                    focus = std::max(focus - 1, 1);
                else if (event.key.code == sf::Keyboard::Key::Down)
                    focus = std::min(focus + 1, 2);
                else if (event.key.code == sf::Keyboard::Key::Enter)
                {
                    if (focus == 1)
                        this->gameWindow(window);
                    else
                        this->settingsWindow(window);
                }
            }
        }
        window.clear();

        window.draw(backgroundSprite);

        // drawing button(if it's focused right now)
        playButton.draw(window, focus == 1);

        settingsButton.draw(window, focus == 2);

        window.display();
    }
    exit(EXIT_SUCCESS);
}

void Application::settingsWindow(sf::RenderWindow &window, const std::string &fontPath)
{
    const float W = window.getSize().x, H = window.getSize().y;
    const float titleW = W, titleH = H * 0.2f;
    const float backButtonW = titleH * 0.5f, backButtonH = backButtonW;
    const sf::Vector2f inputBoxSize = sf::Vector2f(0.64 * W, 0.08 * H);
    const sf::Color backgroundColor(189, 195, 199);
    const int titleCharSize = getAppropriateCharSize("arial.ttf", titleH, titleW, 40, "SETTINGS");
    const int inputCharSize = getAppropriateCharSize("arial.ttf", inputBoxSize.y, inputBoxSize.x, 40, "Snake shape");
    const int applyButtonOutlineW = 10;
    const int backButtonOutlineW = 7;
    sf::Image backButtonImage;
    if (!backButtonImage.loadFromFile("BackButton.png"))
        throw std::invalid_argument("Cannot find <BackButton.png> in directory!");
    sf::Image applyButtonImage;
    if (!applyButtonImage.loadFromFile("ApplyButton.png"))
        throw std::invalid_argument("Cannot find <ApplyButton.png> in directory!");

    // Setting back button
    Button backButton(Button::Shape::RECTANGLE, Button::DataType::IMAGE);
    backButton.setRectSize(sf::Vector2f(backButtonW, backButtonH));
    backButton.setButtonColor(backgroundColor);
    backButton.setImage(backButtonImage);
    backButton.setOutlineW(backButtonOutlineW);
    backButton.setPosition(sf::Vector2f(0, 0));

    // Setting up background
    sf::Image im;
    im.create(W, H, backgroundColor);
    sf::Texture backgroundTexture;
    assert(backgroundTexture.loadFromImage(im));
    sf::Sprite backgroundSprite(backgroundTexture);

    // Setting up title
    TextHint titleHint("arial.ttf", sf::Vector2f(0.54f * W, H));
    titleHint.setCharSize(titleCharSize);
    titleHint.setPosition(sf::Vector2f(0.23f * W, 0));
    titleHint.setFillColor(sf::Color::Black);
    titleHint.setString("SETTINGS");

    // Setting up input hints
    std::vector<TextHint> inputHints(3);
    for (int i = 0; i < inputHints.size(); i++)
    {
        inputHints[i].setCharSize(inputCharSize);
        inputHints[i].setFont("arial.ttf");
        inputHints[i].setPosition(sf::Vector2f(0.18 * W, 0.28f * H + static_cast<float>(i) * 0.19f * H));
        inputHints[i].setSize(inputBoxSize);
        inputHints[i].setFillColor(sf::Color::Black);
    }
    inputHints[0].setString("Snake shape");
    inputHints[1].setString("User name");
    inputHints[2].setString("Maximal FPS");

    // Setting up input boxes
    std::vector<InputBox> inputBoxes(3);
    for (int i = 0; i < inputBoxes.size(); i++)
    {
        inputBoxes[i].setCharSize(inputCharSize);
        inputBoxes[i].setFont("arial.ttf");
        inputBoxes[i].setPosition(sf::Vector2f(inputHints[i].getPosition().x, inputHints[i].getPosition().y + 0.08 * H));
        inputBoxes[i].setSize(inputBoxSize);
        inputBoxes[i].setFillColor(sf::Color::Black);
    }

    // Setting up apply button
    Button applyButton(Button::Shape::RECTANGLE, Button::DataType::IMAGE);
    applyButton.setRectSize(
        sf::Vector2f(
            inputBoxes.front().getSize().x / 2.f, 
            inputBoxes.front().getSize().y
        )  
    );
    applyButton.setOutlineW(applyButtonOutlineW);
    applyButton.setButtonColor(backgroundColor);
    applyButton.setPosition(
        sf::Vector2f(
            (W - applyButton.getSize().x) / 2.f, 
            inputBoxes.back().getPosition().y + inputBoxes.back().getSize().y * 2    
        )
    );
    applyButton.setImage(applyButtonImage);

    // Main loop
    sf::Event event;
    int curSelected = -1;
    bool applyButtonPressed = false, goBack = false;
    while (window.isOpen())
    {
        window.clear();

        if (goBack)
            return;

        // Drawing background color
        window.draw(backgroundSprite);

        // Drawing back button
        backButton.draw(window);

        // Drawing title
        titleHint.draw(window);

        // Draw input hints;
        for (int i = 0; i < inputHints.size(); i++)
            inputHints[i].draw(window);

        // Adding character or unselect current input box
        if (curSelected != -1 && !inputBoxes[curSelected].addChar(window))
        {
            inputBoxes[curSelected].setNotSelected();
            curSelected = -1;
        }

        // Draw input boxes
        for (int i = 0; i < inputBoxes.size(); i++)
            inputBoxes[i].draw(window);

        bool wasMouseClick = false;
        // Events
        while (window.pollEvent(event))
        {
            // If some input box is selected
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !wasMouseClick)
            {
                // Checking if input box has selected
                for (int i = 0; i < inputBoxes.size(); i++)
                {
                    if (inputBoxes[i].isSelected(window))
                    {
                        curSelected = i;
                        if (curSelected != -1)
                            inputBoxes[curSelected].setNotSelected();
                        inputBoxes[curSelected].setSelected();
                        break;
                    }
                }

                // May be user pressed a button
                if (backButton.isSelected(window))
                {
                    backButton.draw(window, true);
                    goBack = true;
                }
                else if (applyButton.isSelected(window))
                    applyButtonPressed = true;
                wasMouseClick = true;
            }

            if (event.type == sf::Event::Closed)
                exit(EXIT_SUCCESS);
        }

        applyButton.draw(window);
        if (applyButtonPressed)
        {
            sf::Font font;
            assert(font.loadFromFile("arial.ttf"));
            sf::Text msg;
            msg.setFont(font);
            msg.setCharacterSize(15);
            if (loadInfoFromInitWindow(window, inputBoxes))
            {   
                msg.setFillColor(sf::Color::Blue);
                msg.setString("Successfully saved!");
            }
            else 
            {
                msg.setFillColor(sf::Color::Red);
                msg.setString("Try to enter again!");
            }
            msg.setPosition(applyButton.getPosition().x + 30, applyButton.getPosition().y - 20);
            window.draw(msg);
            applyButtonPressed = false;
            window.display();
            sf::sleep(sf::milliseconds(1500));
            continue;
        }
        window.display();
    }
    exit(EXIT_SUCCESS);
}

bool Application::loadInfoFromInitWindow(sf::RenderWindow &window, const std::vector<InputBox> &inputBoxes)
{
    std::vector<std::string> texts(inputBoxes.size());
    for (int i = 0; i < inputBoxes.size(); i++)
        texts[i] = std::string(inputBoxes[i].getString());

    if (texts.front() == "rectangle")
        m_snakeShape = SnakeShape::RECTANGLE;
    else if (texts.front() == "circle")
        m_snakeShape = SnakeShape::CIRCLE;
    else
        return false;

    m_userName = texts.at(1);

    int fpsLimit = 0;
    for (char c : texts.back())
    {
        if (!('0' <= c && c <= '9'))
            return false;
        fpsLimit = fpsLimit * 10 + (int)c - '0';
    }

    m_fpsLimit = fpsLimit;
    window.setFramerateLimit(fpsLimit);
    return true;
}

#endif // APPLICATION_CPP
