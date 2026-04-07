#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu
{
public:
    enum class Choice { None, Play, About, Settings, Quit };

    explicit Menu(sf::Font& font);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    Choice getChoice() const;
    void   resetChoice();

private:
    sf::Font& m_font;
    Choice    m_choice;
    int       m_hovered;

    std::vector<sf::RectangleShape> m_boxes;
    std::vector<sf::Text>           m_labels;

    sf::Color m_normal;
    sf::Color m_hover;

    void build();
    void updateHover(sf::Vector2f mouse);
    void centerText(sf::Text& text, float cx, float cy);
};