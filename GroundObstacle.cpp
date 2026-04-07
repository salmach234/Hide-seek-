#include "GroundObstacle.h"
#include "GameConfig.h"

const float GroundObstacle::W = 38.f;
const float GroundObstacle::H = 48.f;

GroundObstacle::GroundObstacle(float speed)
    : Obstacle(Type::Ground, speed)
{
    m_shape.setSize({ W, H });
    m_shape.setFillColor(sf::Color(20, 20, 30));
    m_shape.setOutlineColor(sf::Color(200, 0, 0));
    m_shape.setOutlineThickness(3.f);
    m_shape.setPosition({ (float)WIN_W, GROUND_Y - H });
}

void GroundObstacle::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);

    float x = m_shape.getPosition().x;
    float y = m_shape.getPosition().y;

    sf::RectangleShape base({ W + 10.f, 8.f });
    base.setFillColor(sf::Color(40, 40, 55));
    base.setOutlineColor(sf::Color(70, 70, 90));
    base.setOutlineThickness(1.f);
    base.setPosition({ x - 5.f, y + H - 4.f });
    window.draw(base);

    sf::CircleShape light(7.f);
    light.setFillColor(sf::Color(255, 30, 30));
    light.setPosition({ x + W / 2.f - 7.f, y + 8.f });
    window.draw(light);

    sf::RectangleShape excV({ 4.f, 16.f });
    excV.setFillColor(sf::Color::White);
    excV.setPosition({ x + W / 2.f - 2.f, y + 18.f });
    window.draw(excV);

    sf::RectangleShape excD({ 4.f, 4.f });
    excD.setFillColor(sf::Color::White);
    excD.setPosition({ x + W / 2.f - 2.f, y + 37.f });
    window.draw(excD);
}