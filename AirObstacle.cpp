#include "AirObstacle.h"
#include "GameConfig.h"

const float AirObstacle::W = 64.f;
const float AirObstacle::H = 28.f;

float AirObstacle::yForHeight(Height h)
{
    if (h == Height::Low)  return GROUND_Y - 80.f;
    if (h == Height::Mid)  return GROUND_Y - 140.f;
    return                        GROUND_Y - 220.f;
}

AirObstacle::AirObstacle(float speed, Height h)
    : Obstacle(Type::Air, speed), m_height(h)
{
    m_shape.setSize({ W, H });
    m_shape.setFillColor(sf::Color(50, 55, 70));
    m_shape.setOutlineColor(sf::Color(90, 95, 120));
    m_shape.setOutlineThickness(2.f);
    m_shape.setPosition({ (float)WIN_W, yForHeight(h) });
}

void AirObstacle::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);

    float x = m_shape.getPosition().x;
    float y = m_shape.getPosition().y;

    sf::RectangleShape wL({ 18.f, 7.f });
    wL.setFillColor(sf::Color(35, 35, 50));
    wL.setOutlineColor(sf::Color(70, 70, 90));
    wL.setOutlineThickness(1.f);
    wL.setPosition({ x - 15.f, y + 6.f });
    window.draw(wL);

    sf::RectangleShape wR({ 18.f, 7.f });
    wR.setFillColor(sf::Color(35, 35, 50));
    wR.setOutlineColor(sf::Color(70, 70, 90));
    wR.setOutlineThickness(1.f);
    wR.setPosition({ x + W - 3.f, y + 6.f });
    window.draw(wR);

    sf::RectangleShape cam({ 10.f, 14.f });
    cam.setFillColor(sf::Color(20, 20, 30));
    cam.setOutlineColor(sf::Color(60, 60, 80));
    cam.setOutlineThickness(1.f);
    cam.setPosition({ x + W / 2.f - 5.f, y + H - 2.f });
    window.draw(cam);

    sf::CircleShape red(4.f);
    red.setFillColor(sf::Color(255, 0, 0));
    red.setPosition({ x + W - 12.f, y + 4.f });
    window.draw(red);
}

AirObstacle::Height AirObstacle::getHeight() const { return m_height; }