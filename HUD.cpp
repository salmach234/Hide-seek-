#include "HUD.h"
#include "GameConfig.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>

HUD::HUD(sf::Font& font, float totalTime)
    : m_font(font),
    m_timeLeft(totalTime),
    m_totalTime(totalTime),
    m_blinkTimer(0.f),
    m_blinkOn(true),
    m_endless(false),
    m_bestScore(0),
    m_timerTxt(font, "03:00", 34),
    m_phaseTxt(font, "PHASE 1", 16),
    m_distTxt(font, "Bunker: 0%", 18),
    m_healthTxt(font, "ALIVE", 18),
    m_endlessDistTxt(font, "Distance: 0 m", 26),
    m_endlessBestTxt(font, "Best: 0 m", 20)
{
    m_timerTxt.setFillColor(sf::Color::White);
    m_timerTxt.setPosition({ 14.f, 8.f });

    m_phaseTxt.setFillColor(sf::Color(160, 160, 200));
    m_phaseTxt.setPosition({ 888.f, 10.f });

    m_barBg.setSize({ 560.f, 20.f });
    m_barBg.setFillColor(sf::Color(60, 60, 70));
    m_barBg.setOutlineColor(sf::Color(120, 120, 140));
    m_barBg.setOutlineThickness(2.f);
    m_barBg.setPosition({ 220.f, 18.f });

    m_barFill.setSize({ 0.f, 20.f });
    m_barFill.setFillColor(sf::Color(0, 200, 80));
    m_barFill.setPosition({ 220.f, 18.f });

    m_bunkerIcon.setSize({ 26.f, 26.f });
    m_bunkerIcon.setFillColor(sf::Color(80, 80, 100));
    m_bunkerIcon.setOutlineColor(sf::Color::White);
    m_bunkerIcon.setOutlineThickness(2.f);
    m_bunkerIcon.setPosition({ 790.f, 9.f });

    m_heart1.setSize({ 18.f, 18.f });
    m_heart1.setFillColor(sf::Color(220, 50, 50));
    m_heart1.setPosition({ 828.f, 9.f });

    m_heart2.setSize({ 18.f, 18.f });
    m_heart2.setFillColor(sf::Color(220, 50, 50));
    m_heart2.setPosition({ 854.f, 9.f });

    m_distTxt.setFillColor(sf::Color(200, 200, 100));
    m_distTxt.setPosition({ 220.f, 42.f });

    m_healthTxt.setFillColor(sf::Color(0, 220, 80));
    m_healthTxt.setPosition({ 828.f, 38.f });

    m_endlessDistTxt.setFillColor(sf::Color::White);
    m_endlessDistTxt.setPosition({ 14.f, 510.f });

    m_endlessBestTxt.setFillColor(sf::Color(0, 200, 255));
    m_endlessBestTxt.setPosition({ 14.f, 542.f });
}

std::string HUD::formatTime(float sec) const
{
    int t = (int)std::ceil(sec);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << t / 60
        << ":" << std::setfill('0') << std::setw(2) << t % 60;
    return oss.str();
}

void HUD::centerText(sf::Text& text, float cx, float cy)
{
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({ b.position.x + b.size.x / 2.f,
                     b.position.y + b.size.y / 2.f });
    text.setPosition({ cx, cy });
}

void HUD::update(float dt, Player::Health health,
    float distance, float maxDist, bool endless)
{
    m_endless = endless;

    if (!endless)
    {
        m_timeLeft -= dt;
        if (m_timeLeft < 0.f) m_timeLeft = 0.f;

        m_timerTxt.setString(formatTime(m_timeLeft));

        if (m_timeLeft > 60.f)
        {
            m_timerTxt.setFillColor(sf::Color::White);
            m_phaseTxt.setString("PHASE 1");
            m_phaseTxt.setFillColor(sf::Color(100, 200, 100));
        }
        else if (m_timeLeft > 20.f)
        {
            m_timerTxt.setFillColor(sf::Color::Yellow);
            m_phaseTxt.setString("PHASE 2");
            m_phaseTxt.setFillColor(sf::Color(220, 200, 0));
        }
        else
        {
            m_blinkTimer += dt;
            if (m_blinkTimer >= 0.25f) { m_blinkTimer = 0.f; m_blinkOn = !m_blinkOn; }
            m_timerTxt.setFillColor(m_blinkOn ? sf::Color::Red : sf::Color(100, 0, 0));
            m_phaseTxt.setString("PHASE 3");
            m_phaseTxt.setFillColor(sf::Color(255, 60, 60));
        }

        // شريط التقدم مرتبط بالمسافة فقط
        float ratio = std::min(distance / maxDist, 1.f);
        m_barFill.setSize({ 560.f * ratio, 20.f });

        if (ratio < 0.4f)  m_barFill.setFillColor(sf::Color(0, 200, 80));
        else if (ratio < 0.75f) m_barFill.setFillColor(sf::Color(220, 180, 0));
        else                    m_barFill.setFillColor(sf::Color(220, 60, 0));

        m_distTxt.setString("Bunker: " + std::to_string((int)(ratio * 100)) + "%");
    }
    else
    {
        int meters = (int)(distance / 50.f);
        m_timerTxt.setString("ENDLESS");
        m_timerTxt.setFillColor(sf::Color::Cyan);
        m_endlessDistTxt.setString("Distance: " + std::to_string(meters) + " m");
        if (meters > m_bestScore)
        {
            m_bestScore = meters;
            m_endlessBestTxt.setString("Best: " + std::to_string(m_bestScore) + " m");
        }
    }

    if (health == Player::Health::Alive)
    {
        m_heart1.setFillColor(sf::Color(220, 50, 50));
        m_heart2.setFillColor(sf::Color(220, 50, 50));
        m_healthTxt.setString("ALIVE");
        m_healthTxt.setFillColor(sf::Color(0, 220, 80));
    }
    else if (health == Player::Health::Injured)
    {
        m_heart1.setFillColor(sf::Color(220, 50, 50));
        m_heart2.setFillColor(sf::Color(55, 55, 55));
        m_healthTxt.setString("INJURED");
        m_healthTxt.setFillColor(sf::Color(255, 160, 0));
    }
}

void HUD::draw(sf::RenderWindow& window)
{
    window.draw(m_timerTxt);
    window.draw(m_heart1);
    window.draw(m_heart2);
    window.draw(m_healthTxt);

    if (!m_endless)
    {
        window.draw(m_barBg);
        window.draw(m_barFill);
        window.draw(m_distTxt);
        window.draw(m_bunkerIcon);
        window.draw(m_phaseTxt);
    }
    else
    {
        window.draw(m_endlessDistTxt);
        window.draw(m_endlessBestTxt);
    }
}

void HUD::reset(float totalTime)
{
    m_timeLeft = totalTime;
    m_totalTime = totalTime;
    m_blinkTimer = 0.f;
    m_blinkOn = true;
    m_endless = false;
    m_barFill.setSize({ 0.f, 20.f });
    m_barFill.setFillColor(sf::Color(0, 200, 80));
    m_timerTxt.setString(formatTime(totalTime));
    m_timerTxt.setFillColor(sf::Color::White);
    m_distTxt.setString("Bunker: 0%");
    m_heart1.setFillColor(sf::Color(220, 50, 50));
    m_heart2.setFillColor(sf::Color(220, 50, 50));
    m_healthTxt.setString("ALIVE");
    m_healthTxt.setFillColor(sf::Color(0, 220, 80));
    m_phaseTxt.setString("PHASE 1");
    m_phaseTxt.setFillColor(sf::Color(100, 200, 100));
    m_endlessDistTxt.setString("Distance: 0 m");
}

bool  HUD::isTimeUp()     const { return !m_endless && m_timeLeft <= 0.f; }
float HUD::getTimeLeft()  const { return m_timeLeft; }
int   HUD::getBestScore() const { return m_bestScore; }