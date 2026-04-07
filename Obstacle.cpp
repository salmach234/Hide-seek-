#include "Obstacle.h"

Obstacle::Obstacle(Type type, float speed)
    : m_type(type), m_speed(speed)
{
}

void Obstacle::update(float dt, float /*playerY*/)
{
    m_shape.move({ -m_speed * dt, 0.f });
}

void Obstacle::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

sf::FloatRect  Obstacle::getBounds()   const { return m_shape.getGlobalBounds(); }
bool           Obstacle::isOffScreen() const { return m_shape.getPosition().x < -120.f; }
Obstacle::Type Obstacle::getType()     const { return m_type; }