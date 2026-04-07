#include "HealthPickup.h"
#include "GameConfig.h"
#include <cmath>

const float HealthPickup::SIZE = 30.f;

HealthPickup::HealthPickup(float x, float y)
    : m_x(x), m_y(y), m_animTimer(0.f), m_collected(false)
{
    m_bg.setSize({ SIZE, SIZE });
    m_bg.setFillColor(sf::Color(25, 70, 25));
    m_bg.setOutlineColor(sf::Color(0, 210, 70));
    m_bg.setOutlineThickness(2.f);

    m_crossH.setSize({ SIZE, 10.f });
    m_crossH.setFillColor(sf::Color(0, 240, 90));
    m_crossH.setOrigin({ SIZE / 2.f, 5.f });

    m_crossV.setSize({ 10.f, SIZE });
    m_crossV.setFillColor(sf::Color(0, 240, 90));
    m_crossV.setOrigin({ 5.f, SIZE / 2.f });

    m_glow.setRadius(SIZE * 0.9f);
    m_glow.setFillColor(sf::Color(0, 255, 80, 35));
    m_glow.setOrigin({ SIZE * 0.9f, SIZE * 0.9f });
}

void HealthPickup::update(float dt, float gameSpeed)
{
    m_x -= gameSpeed * dt;
    m_animTimer += dt;

    float offset = std::sin(m_animTimer * 3.f) * 4.f;
    float dy = m_y + offset;

    m_bg.setPosition({ m_x, dy });
    m_crossH.setPosition({ m_x + SIZE / 2.f, dy + SIZE / 2.f });
    m_crossV.setPosition({ m_x + SIZE / 2.f, dy + SIZE / 2.f });
    m_glow.setPosition({ m_x + SIZE / 2.f, dy + SIZE / 2.f });

    uint8_t alpha = (uint8_t)(35 + 25 * std::sin(m_animTimer * 4.f));
    m_glow.setFillColor(sf::Color(0, 255, 80, alpha));
}

void HealthPickup::draw(sf::RenderWindow& window)
{
    if (m_collected) return;
    window.draw(m_glow);
    window.draw(m_bg);
    window.draw(m_crossH);
    window.draw(m_crossV);
}

sf::FloatRect HealthPickup::getBounds()   const { return m_bg.getGlobalBounds(); }
bool          HealthPickup::isCollected() const { return m_collected; }
bool          HealthPickup::isOffScreen() const { return m_x < -80.f; }
void          HealthPickup::collect() { m_collected = true; }