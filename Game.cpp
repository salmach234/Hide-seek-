#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <stdexcept>

const float Game::BUNKER_FIXED_X = 840.f;
const float Game::BUNKER_FIXED_Y = GROUND_Y - 80.f;

Game::Game()
    : m_window(sf::VideoMode({ WIN_W, WIN_H }), "ZERO HOUR", sf::Style::Close),
    m_menu(m_font),
    m_player(),
    m_hud(m_font, TOTAL_TIME),
    m_cinematic(m_font),
    m_obs(m_font),
    m_modeManager(m_font),
    m_settings(m_font),
    m_state(GameState::MainMenu),
    m_endless(false),
    m_distance(0.f),
    m_speed(SPD_PHASE1),
    m_warnTimer(0.f),
    m_warnBlink(0.f),
    m_doorH(80.f),
    m_doorClosing(false),
    m_aboutScrollY(0.f),
    m_aboutView(sf::FloatRect({ 0.f, 0.f }, { (float)WIN_W, (float)WIN_H })),
    m_bunkerLbl(m_font, "[ BUNKER ]", 16),
    m_txtTitle(m_font, "ZERO HOUR", 72),
    m_txtSubtitle(m_font, "Escape before it's too late...", 22),
    m_txtAbout(m_font, "", 19),
    m_txtGameOver(m_font, "", 34),
    m_txtEndlessOver(m_font, "", 30)
{
    std::srand((unsigned)std::time(nullptr));
    m_window.setFramerateLimit(FPS);

    if (!m_font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        throw std::runtime_error("Font not found");

    m_aboutView = m_window.getDefaultView();
    initUI();
}

void Game::centerText(sf::Text& text, float cx, float cy)
{
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({ b.position.x + b.size.x / 2.f,
                     b.position.y + b.size.y / 2.f });
    text.setPosition({ cx, cy });
}

void Game::initUI()
{
    m_txtTitle.setFillColor(sf::Color::Yellow);
    centerText(m_txtTitle, WIN_W / 2.f, 80.f);

    m_txtSubtitle.setFillColor(sf::Color(160, 160, 160));
    centerText(m_txtSubtitle, WIN_W / 2.f, 138.f);

    m_txtAbout.setString(
        "ZERO HOUR\n"
        "==============================\n\n"
        "YEAR 2031.\n\n"
        "NEXUS runs a secret underground facility,\n"
        "experimenting on humans to create super-soldiers.\n\n"
        "A trapped scientist sent one message to agent KAI:\n"
        "  'There is a child here. Save him.' - X-07\n\n"
        "==============================\n\n"
        "KAI infiltrated the facility, retrieved the files,\n"
        "and secured the child's escape.\n\n"
        "But NEXUS was watching.\n\n"
        "They sealed every door and activated:\n"
        "  PROTOCOL ZERO HOUR - 3 MINUTES TO DESTRUCTION\n\n"
        "==============================\n\n"
        "One corridor. One bunker. 180 seconds.\n"
        "Mines on the ground. Missiles in the air.\n\n"
        "Run, KAI. Run.\n\n"
        "==============================\n\n"
        "CONTROLS:\n"
        "  Space / Up (hold)  ->  High jump\n"
        "  Space / Up (tap)   ->  Low jump\n"
        "  Down Arrow         ->  Duck\n"
        "  Escape             ->  Menu\n\n\n");
    m_txtAbout.setFillColor(sf::Color::White);
    m_txtAbout.setPosition({ 80.f, 20.f });

    m_txtGameOver.setString(
        "MISSION FAILED\n\n"
        "KAI did not make it...\n\n"
        "Press Enter to retry\n"
        "Press Escape for menu");
    m_txtGameOver.setFillColor(sf::Color::Red);
    centerText(m_txtGameOver, WIN_W / 2.f, WIN_H / 2.f);

    // Bunker (fixe)
    m_bunker.setSize({ 110.f, 80.f });
    m_bunker.setFillColor(sf::Color(45, 45, 60));
    m_bunker.setOutlineColor(sf::Color(100, 100, 130));
    m_bunker.setOutlineThickness(3.f);
    m_bunker.setPosition({ BUNKER_FIXED_X, BUNKER_FIXED_Y });

    m_bunkerTop.setSize({ 120.f, 10.f });
    m_bunkerTop.setFillColor(sf::Color(70, 70, 90));
    m_bunkerTop.setPosition({ BUNKER_FIXED_X - 5.f, BUNKER_FIXED_Y - 10.f });

    m_door.setFillColor(sf::Color(0, 180, 60));
    m_door.setSize({ 50.f, 80.f });
    m_door.setPosition({ BUNKER_FIXED_X + 28.f, BUNKER_FIXED_Y });

    m_bunkerLbl.setFillColor(sf::Color(0, 255, 80));
    m_bunkerLbl.setPosition({ BUNKER_FIXED_X + 5.f, BUNKER_FIXED_Y - 28.f });
}

void Game::resetGame()
{
    m_player = Player();
    m_speed = SPD_PHASE1;
    m_distance = 0.f;
    m_doorH = 80.f;
    m_doorClosing = false;
    m_warnTimer = 0.f;
    m_aboutScrollY = 0.f;
    m_aboutView = m_window.getDefaultView();

    m_door.setSize({ 50.f, 80.f });
    m_door.setPosition({ BUNKER_FIXED_X + 28.f, BUNKER_FIXED_Y });

    m_obs.reset();
    m_hud.reset(TOTAL_TIME);
    m_cinematic.resetOpening();
    m_state = GameState::Opening;
}

int Game::getPhase() const
{
    if (m_endless) return 2;
    float t = m_hud.getTimeLeft();
    if (t > 120.f) return 1;
    if (t > 60.f)  return 2;
    return                3;
}

void Game::drawLab()
{
    m_window.clear(C_LAB_BG);

    sf::RectangleShape ceil({ (float)WIN_W, 68.f });
    ceil.setFillColor(C_LAB_CEIL);
    ceil.setPosition({ 0.f, 0.f });
    m_window.draw(ceil);

    sf::RectangleShape ceilEdge({ (float)WIN_W, 5.f });
    ceilEdge.setFillColor(sf::Color(70, 70, 90));
    ceilEdge.setPosition({ 0.f, 68.f });
    m_window.draw(ceilEdge);

    for (int i = 0; i < 6; i++)
    {
        sf::RectangleShape pipe({ 20.f, 38.f });
        pipe.setFillColor(sf::Color(50, 52, 68));
        pipe.setOutlineColor(sf::Color(65, 68, 88));
        pipe.setOutlineThickness(1.5f);
        pipe.setPosition({ 75.f + i * 170.f, 30.f });
        m_window.draw(pipe);
    }

    uint8_t bA = (uint8_t)(m_warnBlink * 240.f);
    for (int i = 0; i < 5; i++)
    {
        sf::CircleShape glow(18.f);
        glow.setFillColor(sf::Color(180, 0, 0, (uint8_t)(bA / 3)));
        glow.setPosition({ 84.f + i * 210.f - 4.f, 28.f });
        m_window.draw(glow);

        sf::CircleShape light(10.f);
        light.setFillColor(sf::Color(220, 0, 0, bA));
        light.setOutlineColor(sf::Color(80, 0, 0));
        light.setOutlineThickness(2.f);
        light.setPosition({ 90.f + i * 210.f, 35.f });
        m_window.draw(light);
    }

    for (int i = 0; i < 4; i++)
    {
        sf::RectangleShape panel({ 185.f, 290.f });
        panel.setFillColor(sf::Color(18, 18, 30));
        panel.setOutlineColor(sf::Color(38, 38, 58));
        panel.setOutlineThickness(2.f);
        panel.setPosition({ 55.f + i * 240.f, 73.f });
        m_window.draw(panel);

        for (int j = 0; j < 4; j++)
        {
            sf::RectangleShape det({ 155.f, 2.f });
            det.setFillColor(sf::Color(32, 32, 50));
            det.setPosition({ 70.f + i * 240.f, 110.f + j * 55.f });
            m_window.draw(det);
        }

        sf::RectangleShape corner({ 12.f, 12.f });
        corner.setFillColor(sf::Color(55, 55, 75));
        corner.setPosition({ 55.f + i * 240.f, 73.f });
        m_window.draw(corner);
    }

    sf::RectangleShape floor({ (float)WIN_W, 120.f });
    floor.setFillColor(C_LAB_FLOOR);
    floor.setPosition({ 0.f, GROUND_Y });
    m_window.draw(floor);

    sf::RectangleShape floorEdge({ (float)WIN_W, 6.f });
    floorEdge.setFillColor(sf::Color(72, 72, 92));
    floorEdge.setPosition({ 0.f, GROUND_Y });
    m_window.draw(floorEdge);

    for (int i = 1; i < 8; i++)
    {
        sf::RectangleShape grid({ 2.f, 120.f });
        grid.setFillColor(sf::Color(52, 52, 65));
        grid.setPosition({ (float)(i * 128), GROUND_Y });
        m_window.draw(grid);
    }

    for (int i = 0; i < 11; i++)
    {
        sf::RectangleShape stripe({ 45.f, 10.f });
        stripe.setFillColor(i % 2 == 0 ? sf::Color(170, 130, 0)
            : sf::Color(28, 28, 42));
        stripe.setPosition({ 5.f + i * 95.f, GROUND_Y + 4.f });
        m_window.draw(stripe);
    }
}

void Game::processEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (const auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::Escape)
            {
                if (m_state == GameState::About ||
                    m_state == GameState::Settings)
                {
                    m_aboutScrollY = 0.f;
                    m_aboutView = m_window.getDefaultView();
                    m_state = GameState::MainMenu;
                    return;
                }
                if (m_state == GameState::Playing ||
                    m_state == GameState::GameOver ||
                    m_state == GameState::ModeSelect)
                {
                    m_obs.clear();
                    m_state = GameState::MainMenu;
                    return;
                }
            }

            if (key->code == sf::Keyboard::Key::Enter)
            {
                if (m_state == GameState::GameOver) resetGame();
                if (m_state == GameState::Win)      m_state = GameState::MainMenu;
                if (m_state == GameState::Opening)
                {
                    m_state = GameState::Playing;
                    m_obs.reset();
                }
            }

            if (m_state == GameState::About)
            {
                if (key->code == sf::Keyboard::Key::Down) m_aboutScrollY += 40.f;
                if (key->code == sf::Keyboard::Key::Up)   m_aboutScrollY -= 40.f;
            }
        }

        if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>())
            if (m_state == GameState::About)
                m_aboutScrollY -= wheel->delta * 35.f;

        if (m_state == GameState::MainMenu)
            m_menu.handleEvent(*event, m_window);

        if (m_state == GameState::ModeSelect)
            m_modeManager.handleEvent(*event, m_window);

        if (m_state == GameState::Settings)
            m_settings.handleEvent(*event, m_window);
    }
}

void Game::update(float dt)
{
    // ---- MainMenu ----
    if (m_state == GameState::MainMenu)
    {
        m_menu.update(m_window);
        auto choice = m_menu.getChoice();
        if (choice == Menu::Choice::Play)
        {
            m_state = GameState::ModeSelect;
            m_modeManager.reset();
            m_menu.resetChoice();
        }
        else if (choice == Menu::Choice::About)
        {
            m_aboutScrollY = 0.f;
            m_aboutView = m_window.getDefaultView();
            m_state = GameState::About;
            m_menu.resetChoice();
        }
        else if (choice == Menu::Choice::Settings)
        {
            m_settings.resetBack();
            m_state = GameState::Settings;
            m_menu.resetChoice();
        }
        else if (choice == Menu::Choice::Quit)
            m_window.close();
    }

    // ---- About scroll ----
    if (m_state == GameState::About)
    {
        float maxScroll = 700.f;
        if (m_aboutScrollY < 0.f)       m_aboutScrollY = 0.f;
        if (m_aboutScrollY > maxScroll) m_aboutScrollY = maxScroll;
        m_aboutView.setCenter({ WIN_W / 2.f, WIN_H / 2.f + m_aboutScrollY });
    }

    // ---- Settings ----
    if (m_state == GameState::Settings)
    {
        m_settings.update(m_window);
        if (m_settings.wantsBack())
        {
            m_settings.resetBack();
            m_state = GameState::MainMenu;
        }
    }

    // ---- ModeSelect ----
    if (m_state == GameState::ModeSelect)
    {
        m_modeManager.update(m_window);
        auto mode = m_modeManager.getMode();
        if (mode == GameModeManager::Mode::Story)
        {
            m_endless = false;
            resetGame();
            m_modeManager.reset();
        }
        else if (mode == GameModeManager::Mode::Endless)
        {
            m_endless = true;
            resetGame();
            m_modeManager.reset();
        }
    }

    // ---- Opening ----
    if (m_state == GameState::Opening)
    {
        m_cinematic.updateOpening(dt);
        if (m_cinematic.isOpeningDone())
        {
            m_state = GameState::Playing;
            m_obs.reset();
        }
    }

    // ---- HUD + blink (Playing + WinSequence) ----
    if (m_state == GameState::Playing || m_state == GameState::WinSequence)
    {
        m_hud.update(dt, m_player.getHealth(),
            m_distance, MAX_DISTANCE, m_endless);

        m_warnTimer += dt;
        float bs = 2.f;
        if (!m_endless && m_hud.getTimeLeft() < 60.f) bs = 5.f;
        if (!m_endless && m_hud.getTimeLeft() < 20.f) bs = 10.f;
        if (m_state == GameState::WinSequence)         bs = 14.f;
        m_warnBlink = (std::sin(m_warnTimer * bs) + 1.f) / 2.f;
    }

    // ---- Playing ----
    if (m_state == GameState::Playing)
    {
        m_player.handleInput(dt);
        m_player.update(dt);

        float targetSpeed = (getPhase() == 1) ? SPD_PHASE1
            : (getPhase() == 2) ? SPD_PHASE2 : SPD_PHASE3;
        m_speed += (targetSpeed - m_speed) * 0.01f;
        if (m_speed > SPD_MAX) m_speed = SPD_MAX;

        m_distance += m_speed * dt;

        m_obs.update(dt, m_speed, m_player.getY(),
            m_player.getJumpCount(), m_player.getDuckCount(),
            getPhase(), m_player.getHealth());

        // Collision obstacles
        for (auto& obs : m_obs.obstacles())
        {
            sf::FloatRect pb = m_player.getBounds();
            pb.position.x += 6.f;  pb.size.x -= 12.f;
            pb.position.y += 6.f;  pb.size.y -= 12.f;

            if (pb.findIntersection(obs->getBounds()))
            {
                m_player.takeHit();
                if (!m_player.isAlive())
                {
                    if (m_endless)
                    {
                        int meters = (int)(m_distance / 50.f);
                        m_txtEndlessOver.setString(
                            "GAME OVER\n\nDistance: " +
                            std::to_string(meters) + " m\n\n"
                            "Press Enter to retry\n"
                            "Press Escape for menu");
                        m_txtEndlessOver.setFillColor(sf::Color::Cyan);
                        centerText(m_txtEndlessOver, WIN_W / 2.f, WIN_H / 2.f);
                    }
                    m_state = GameState::GameOver;
                }
                break;
            }
        }

        // Collision health pickups (Story + Endless)
        for (auto& pu : m_obs.pickups())
        {
            if (!pu->isCollected() &&
                m_player.getHealth() == Player::Health::Injured)
            {
                if (m_player.getBounds().findIntersection(pu->getBounds()))
                {
                    pu->collect();
                    m_player.heal();
                }
            }
        }

        if (!m_endless)
        {
            if (m_hud.isTimeUp())
            {
                m_state = GameState::GameOver;
            }
            else if (m_distance >= WIN_DISTANCE)
            {
                m_obs.stopSpawning();
                if (m_obs.obstacles().empty())
                {
                    m_doorH = 80.f;
                    m_doorClosing = false;
                    m_door.setSize({ 50.f, 80.f });
                    m_door.setPosition({ BUNKER_FIXED_X + 28.f, BUNKER_FIXED_Y });
                    m_state = GameState::WinSequence;
                }
            }
        }
    }

    // ---- WinSequence — bunker fixe, joueur avance ----
    if (m_state == GameState::WinSequence)
    {
        float doorX = BUNKER_FIXED_X + 28.f;

        if (!m_doorClosing)
        {
            // Le joueur avance vers le bunker fixe
            m_player.moveRight(180.f, dt);

            if (m_player.getX() >= doorX - 10.f)
                m_doorClosing = true;
        }

        if (m_doorClosing)
        {
            m_doorH -= 70.f * dt;
            if (m_doorH < 0.f) m_doorH = 0.f;

            float closed = 80.f - m_doorH;
            m_door.setSize({ 50.f, m_doorH });
            m_door.setPosition({ doorX, BUNKER_FIXED_Y + closed });

            if (m_doorH <= 0.f)
            {
                m_cinematic.resetWin();
                m_state = GameState::WinCinematic;
            }
        }
    }

    // ---- WinCinematic ----
    if (m_state == GameState::WinCinematic)
    {
        m_cinematic.updateWin(dt);
        if (m_cinematic.isWinDone())
            m_state = GameState::Win;
    }
}

void Game::render()
{
    switch (m_state)
    {
    case GameState::MainMenu:
        m_window.clear(C_MENU_BG);
        m_window.draw(m_txtTitle);
        m_window.draw(m_txtSubtitle);
        m_menu.draw(m_window);
        break;

    case GameState::ModeSelect:
        m_window.clear(C_MENU_BG);
        m_modeManager.draw(m_window);
        break;

    case GameState::Opening:
        m_cinematic.drawOpening(m_window);
        break;

    case GameState::Playing:
        drawLab();
        m_obs.draw(m_window);
        m_player.draw(m_window);
        m_hud.draw(m_window);
        break;

    case GameState::WinSequence:
        drawLab();
        m_obs.draw(m_window);
        m_player.draw(m_window);
        m_window.draw(m_bunkerTop);
        m_window.draw(m_bunker);
        if (m_doorH > 0.f) m_window.draw(m_door);
        m_window.draw(m_bunkerLbl);
        m_hud.draw(m_window);
        break;

    case GameState::About:
    {
        m_window.setView(m_aboutView);
        m_window.clear(C_MENU_BG);
        m_window.draw(m_txtAbout);
        m_window.setView(m_window.getDefaultView());

        // Bouton Back
        sf::RectangleShape btnBack(sf::Vector2f(120.f, 40.f));
        btnBack.setFillColor(sf::Color(50, 50, 80));
        btnBack.setOutlineColor(sf::Color::White);
        btnBack.setOutlineThickness(2.f);
        btnBack.setPosition({ 28.f, 18.f });
        m_window.draw(btnBack);

        sf::Text txtBack(m_font, "<  Back", 20);
        txtBack.setFillColor(sf::Color::White);
        sf::FloatRect bb = txtBack.getLocalBounds();
        txtBack.setOrigin({ bb.position.x, bb.position.y + bb.size.y / 2.f });
        txtBack.setPosition({ 36.f, 18.f + 20.f });
        m_window.draw(txtBack);

        if (m_aboutScrollY < 650.f)
        {
            sf::Text hint(m_font, "v  Scroll to read more  v", 15);
            hint.setFillColor(sf::Color(130, 130, 160));
            sf::FloatRect hb = hint.getLocalBounds();
            hint.setOrigin({ hb.position.x + hb.size.x / 2.f, 0.f });
            hint.setPosition({ WIN_W / 2.f, WIN_H - 28.f });
            m_window.draw(hint);
        }
        break;
    }

    case GameState::Settings:
        m_window.clear(C_MENU_BG);
        m_settings.draw(m_window);
        break;

    case GameState::GameOver:
        m_window.clear(sf::Color::Black);
        if (m_endless) m_window.draw(m_txtEndlessOver);
        else           m_window.draw(m_txtGameOver);
        break;

    case GameState::WinCinematic:
    case GameState::Win:
        m_cinematic.drawWin(m_window);
        break;

    default:
        break;
    }

    m_window.display();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        processEvents();
        update(dt);
        render();
    }
}