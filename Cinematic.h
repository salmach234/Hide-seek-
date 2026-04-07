#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Cinematic
{
public:
    explicit Cinematic(sf::Font& font);

    void updateOpening(float dt);
    void drawOpening(sf::RenderWindow& window);
    bool isOpeningDone() const;
    void resetOpening();

    void updateWin(float dt);
    void drawWin(sf::RenderWindow& window);
    bool isWinDone() const;
    void resetWin();

private:
    sf::Font& m_font;

    float m_openTimer;
    int   m_openPhase;
    float m_shake;

    struct WallPiece
    {
        sf::RectangleShape shape;
        sf::Vector2f       vel = { 0.f, 0.f };
        float              rot = 0.f;
    };
    std::vector<WallPiece> m_wall;

    sf::RectangleShape m_kaiBody;
    sf::RectangleShape m_kaiHead;
    float              m_kaiX;

    sf::Text m_txtClassified;
    sf::Text m_txtSector;
    sf::Text m_txtZeroHour;
    sf::Text m_txtRun;

    float m_winTimer;
    int   m_winPhase;

    sf::Text m_txtDestroyed;
    sf::Text m_txtSurvived;
    sf::Text m_txtContinue;

    struct Explosion
    {
        sf::CircleShape shape;
        float           timer = 0.f;
        float           maxR = 0.f;
    };
    std::vector<Explosion> m_explosions;

    void buildWall();
    void buildExplosions();
    void drawLabWorld(sf::RenderWindow& window);
};