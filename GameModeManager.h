#pragma once
#include <SFML/Graphics.hpp>

class GameModeManager
{
public:
    enum class Mode { None, Story, Endless };

    explicit GameModeManager(sf::Font& font);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    Mode getMode() const;
    void reset();

private:
    sf::Font& m_font;
    Mode      m_mode;
    int       m_hovered;

    sf::RectangleShape m_boxStory;
    sf::RectangleShape m_boxEndless;
    sf::Text           m_title;
    sf::Text           m_lblStory;
    sf::Text           m_lblEndless;
    sf::Text           m_descStory;
    sf::Text           m_descEndless;

    sf::Color m_normal;
    sf::Color m_hover;

    void updateHover(sf::Vector2f mouse);
};