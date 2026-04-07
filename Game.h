#pragma once
#include <SFML/Graphics.hpp>
#include "GameConfig.h"
#include "Menu.h"
#include "Player.h"
#include "ObstacleManager.h"
#include "HUD.h"
#include "Cinematic.h"
#include "GameModeManager.h"
#include "Settings.h"

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void resetGame();
    void drawLab();
    int  getPhase() const;
    void initUI();
    void centerText(sf::Text& text, float cx, float cy);

    sf::RenderWindow m_window;
    sf::Font         m_font;
    sf::Clock        m_clock;

    Menu            m_menu;
    Player          m_player;
    HUD             m_hud;
    Cinematic       m_cinematic;
    ObstacleManager m_obs;
    GameModeManager m_modeManager;
    Settings        m_settings;

    GameState m_state;
    bool      m_endless;
    float     m_distance;
    float     m_speed;
    float     m_warnTimer;
    float     m_warnBlink;

    // WinSequence — bunker fixe, joueur se déplace
    static const float BUNKER_FIXED_X;
    static const float BUNKER_FIXED_Y;
    float m_doorH;
    bool  m_doorClosing;

    float    m_aboutScrollY;
    sf::View m_aboutView;

    sf::RectangleShape m_bunker;
    sf::RectangleShape m_bunkerTop;
    sf::RectangleShape m_door;
    sf::Text           m_bunkerLbl;

    sf::Text m_txtTitle;
    sf::Text m_txtSubtitle;
    sf::Text m_txtAbout;
    sf::Text m_txtGameOver;
    sf::Text m_txtEndlessOver;
};