#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class HUD
{
public:
    HUD(sf::Font& font, float totalTime);

    void update(float dt, Player::Health health,
        float distance, float maxDist,
        bool endless = false);
    void draw(sf::RenderWindow& window);
    void reset(float totalTime);

    bool  isTimeUp()     const;
    float getTimeLeft()  const;
    int   getBestScore() const;

private:
    sf::Font& m_font;
    float     m_timeLeft;
    float     m_totalTime;
    float     m_blinkTimer;
    bool      m_blinkOn;
    bool      m_endless;
    int       m_bestScore;

    sf::RectangleShape m_barBg;
    sf::RectangleShape m_barFill;
    sf::RectangleShape m_bunkerIcon;
    sf::RectangleShape m_heart1;
    sf::RectangleShape m_heart2;

    sf::Text m_timerTxt;
    sf::Text m_phaseTxt;
    sf::Text m_distTxt;
    sf::Text m_healthTxt;
    sf::Text m_endlessDistTxt;
    sf::Text m_endlessBestTxt;

    std::string formatTime(float s) const;
    void centerText(sf::Text& text, float cx, float cy);
};