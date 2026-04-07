#pragma once
#include <SFML/Graphics.hpp>

class HealthPickup
{
public:
    HealthPickup(float x, float y);

    void update(float dt, float gameSpeed);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds()   const;
    bool          isCollected() const;
    bool          isOffScreen() const;
    void          collect();

private:
    float m_x;
    float m_y;
    float m_animTimer;
    bool  m_collected;

    sf::RectangleShape m_bg;
    sf::RectangleShape m_crossH;
    sf::RectangleShape m_crossV;
    sf::CircleShape    m_glow;

    static const float SIZE;
};