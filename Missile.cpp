#include "Missile.h"
#include "GameConfig.h"
#include <cmath>

const float Missile::W = 55.f;
const float Missile::H = 20.f;

float Missile::yForHeight(MHeight h)
{
    // Low  → hits running player body → needs jump to avoid
    // Mid  → hits running player head → needs duck to avoid
    // High → passes above player → no action needed
    if (h == MHeight::Low)  return GROUND_Y - 36.f;
    if (h == MHeight::Mid)  return GROUND_Y - PLAYER_H;
    return                         180.f;
}

Missile::Missile(float speed, MHeight h)
    : Obstacle(Type::Missile, speed),
    m_mheight(h),
    m_animTimer(0.f)
{
    m_shape.setSize({ W, H });
    m_shape.setFillColor(sf::Color(180, 60, 0));
    m_shape.setOutlineColor(sf::Color(255, 120, 0));
    m_shape.setOutlineThickness(2.f);
    m_shape.setPosition({ (float)WIN_W, yForHeight(h) });
}

void Missile::update(float dt, float /*playerY*/)
{
    m_shape.move({ -m_speed * dt, 0.f });
    m_animTimer += dt;
}

void Missile::draw(sf::RenderWindow& window)
{
    window.draw(m_shape);

    float x = m_shape.getPosition().x;
    float y = m_shape.getPosition().y;

    sf::RectangleShape nose({ 18.f, H });
    nose.setFillColor(sf::Color(220, 80, 0));
    nose.setOutlineColor(sf::Color(255, 140, 0));
    nose.setOutlineThickness(1.f);
    nose.setPosition({ x - 16.f, y });
    window.draw(nose);

    sf::RectangleShape finT({ 8.f, 10.f });
    finT.setFillColor(sf::Color(140, 40, 0));
    finT.setPosition({ x + W - 14.f, y - 8.f });
    window.draw(finT);

    sf::RectangleShape finB({ 8.f, 10.f });
    finB.setFillColor(sf::Color(140, 40, 0));
    finB.setPosition({ x + W - 14.f, y + H - 2.f });
    window.draw(finB);

    float flicker = (std::sin(m_animTimer * 20.f) + 1.f) / 2.f;
    float r = 8.f + flicker * 4.f;
    sf::CircleShape flame(r);
    flame.setFillColor(sf::Color(255, (uint8_t)(180 * flicker), 0, 200));
    flame.setPosition({ x + W - 4.f, y + H / 2.f - r });
    window.draw(flame);
}

Missile::MHeight Missile::getMHeight() const { return m_mheight; }