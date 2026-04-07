#pragma once
#include <SFML/Graphics.hpp>

class Obstacle
{
public:
    enum class Type { Ground, Air, Missile };

    explicit Obstacle(Type type, float speed);
    virtual ~Obstacle() = default;

    virtual void update(float dt, float playerY = 0.f);
    virtual void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds()   const;
    bool          isOffScreen() const;
    Type          getType()     const;

protected:
    sf::RectangleShape m_shape;
    Type               m_type;
    float              m_speed;
};