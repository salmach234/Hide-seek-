#pragma once
#include <SFML/Graphics.hpp>
#include "Missile.h"

class WarningIndicator
{
public:
    WarningIndicator(sf::Font& font, Missile::MHeight height, float duration);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isDone() const;

private:
    sf::Text           m_exclaim;
    sf::RectangleShape m_arrow;
    float              m_timer;
    float              m_duration;
    float              m_blinkTimer;
    bool               m_visible;
};