#include "WarningIndicator.h"

WarningIndicator::WarningIndicator(sf::Font& font,
    Missile::MHeight height,
    float duration)
    : m_timer(0.f),
    m_duration(duration),
    m_blinkTimer(0.f),
    m_visible(true),
    m_exclaim(font, "!", 28)
{
    float posY = Missile::yForHeight(height);

    m_arrow.setSize({ 28.f, 12.f });
    m_arrow.setFillColor(sf::Color(255, 80, 0));
    m_arrow.setPosition({ 982.f, posY + 4.f });

    m_exclaim.setFillColor(sf::Color::Yellow);
    m_exclaim.setPosition({ 960.f, posY - 4.f });
}

void WarningIndicator::update(float dt)
{
    m_timer += dt;
    m_blinkTimer += dt;
    if (m_blinkTimer >= 0.12f) { m_blinkTimer = 0.f; m_visible = !m_visible; }
}

void WarningIndicator::draw(sf::RenderWindow& window)
{
    if (!m_visible) return;
    window.draw(m_arrow);
    window.draw(m_exclaim);
}

bool WarningIndicator::isDone() const { return m_timer >= m_duration; }