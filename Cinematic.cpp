#include "Cinematic.h"
#include "GameConfig.h"
#include <cstdlib>
#include <cmath>

Cinematic::Cinematic(sf::Font& font)
    : m_font(font),
    m_openTimer(0.f), m_openPhase(0), m_shake(0.f),
    m_kaiX(-80.f),
    m_winTimer(0.f), m_winPhase(0),
    m_txtClassified(font, "CLASSIFIED FACILITY", 40),
    m_txtSector(font, "SECTOR 7 - SELF DESTRUCT INITIATED", 24),
    m_txtZeroHour(font, "ZERO HOUR", 80),
    m_txtRun(font, "RUN!", 90),
    m_txtDestroyed(font, "FACILITY DESTROYED", 44),
    m_txtSurvived(font, "KAI SURVIVED", 70),
    m_txtContinue(font, "Press Enter to continue", 26)
{
    m_txtClassified.setFillColor(sf::Color::Red);
    m_txtClassified.setPosition({ 200.f, 180.f });

    m_txtSector.setFillColor(sf::Color(200, 200, 200));
    m_txtSector.setPosition({ 120.f, 240.f });

    m_txtZeroHour.setFillColor(sf::Color(255, 255, 0, 0));
    sf::FloatRect zb = m_txtZeroHour.getLocalBounds();
    m_txtZeroHour.setOrigin({ zb.size.x / 2.f, zb.size.y / 2.f });
    m_txtZeroHour.setPosition({ 512.f, 200.f });

    m_txtRun.setFillColor(sf::Color::Red);
    sf::FloatRect rb = m_txtRun.getLocalBounds();
    m_txtRun.setOrigin({ rb.size.x / 2.f, rb.size.y / 2.f });
    m_txtRun.setPosition({ 512.f, 260.f });

    m_kaiBody.setSize({ 40.f, 40.f });
    m_kaiBody.setFillColor(sf::Color(60, 120, 220));
    m_kaiBody.setOutlineColor(sf::Color(30, 60, 140));
    m_kaiBody.setOutlineThickness(2.f);

    m_kaiHead.setSize({ 30.f, 30.f });
    m_kaiHead.setFillColor(sf::Color(255, 200, 150));
    m_kaiHead.setOutlineColor(sf::Color(180, 120, 80));
    m_kaiHead.setOutlineThickness(2.f);

    m_txtDestroyed.setFillColor(sf::Color::Red);
    sf::FloatRect db = m_txtDestroyed.getLocalBounds();
    m_txtDestroyed.setOrigin({ db.size.x / 2.f, db.size.y / 2.f });
    m_txtDestroyed.setPosition({ 512.f, 160.f });

    m_txtSurvived.setFillColor(sf::Color::Green);
    sf::FloatRect sb = m_txtSurvived.getLocalBounds();
    m_txtSurvived.setOrigin({ sb.size.x / 2.f, sb.size.y / 2.f });
    m_txtSurvived.setPosition({ 512.f, 270.f });

    m_txtContinue.setFillColor(sf::Color::White);
    sf::FloatRect cb = m_txtContinue.getLocalBounds();
    m_txtContinue.setOrigin({ cb.size.x / 2.f, 0.f });
    m_txtContinue.setPosition({ 512.f, 400.f });

    buildWall();
    buildExplosions();
}

void Cinematic::buildWall()
{
    m_wall.clear();
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 6; c++)
        {
            WallPiece p;
            p.shape.setSize({ 58.f, 58.f });
            p.shape.setFillColor(sf::Color(100 + r * 10, 80 + c * 5, 80));
            p.shape.setOutlineColor(sf::Color(50, 30, 30));
            p.shape.setOutlineThickness(2.f);
            p.shape.setPosition({ 302.f + c * 60.f, 152.f + r * 60.f });
            float a = (float)(std::rand() % 360);
            float sp = 200.f + (float)(std::rand() % 300);
            p.vel = { std::cos(a * 3.14159f / 180.f) * sp,
                      std::sin(a * 3.14159f / 180.f) * sp - 100.f };
            p.rot = (float)(std::rand() % 200 - 100);
            m_wall.push_back(p);
        }
}

void Cinematic::buildExplosions()
{
    m_explosions.clear();
    for (int i = 0; i < 10; i++)
    {
        Explosion e;
        e.maxR = 30.f + (float)(std::rand() % 60);
        e.timer = (float)(std::rand() % 100) / 100.f;
        e.shape.setRadius(5.f);
        e.shape.setFillColor(sf::Color(220, 120, 0));
        e.shape.setPosition({ (float)(80 + std::rand() % 860),
                              (float)(80 + std::rand() % 300) });
        m_explosions.push_back(e);
    }
}

void Cinematic::drawLabWorld(sf::RenderWindow& window)
{
    window.clear(C_LAB_BG);

    sf::RectangleShape ceil({ (float)WIN_W, 68.f });
    ceil.setFillColor(C_LAB_CEIL);
    ceil.setPosition({ 0.f, 0.f });
    window.draw(ceil);

    sf::RectangleShape ceilEdge({ (float)WIN_W, 5.f });
    ceilEdge.setFillColor(sf::Color(70, 70, 90));
    ceilEdge.setPosition({ 0.f, 68.f });
    window.draw(ceilEdge);

    for (int i = 0; i < 6; i++)
    {
        sf::RectangleShape pipe({ 20.f, 38.f });
        pipe.setFillColor(sf::Color(50, 52, 68));
        pipe.setOutlineColor(sf::Color(65, 68, 88));
        pipe.setOutlineThickness(1.5f);
        pipe.setPosition({ 75.f + i * 170.f, 30.f });
        window.draw(pipe);
    }

    for (int i = 0; i < 4; i++)
    {
        sf::RectangleShape panel({ 185.f, 290.f });
        panel.setFillColor(sf::Color(18, 18, 30));
        panel.setOutlineColor(sf::Color(38, 38, 58));
        panel.setOutlineThickness(2.f);
        panel.setPosition({ 55.f + i * 240.f, 73.f });
        window.draw(panel);
    }

    sf::RectangleShape floor({ (float)WIN_W, 120.f });
    floor.setFillColor(C_LAB_FLOOR);
    floor.setPosition({ 0.f, GROUND_Y });
    window.draw(floor);

    sf::RectangleShape floorEdge({ (float)WIN_W, 6.f });
    floorEdge.setFillColor(sf::Color(72, 72, 92));
    floorEdge.setPosition({ 0.f, GROUND_Y });
    window.draw(floorEdge);
}

void Cinematic::updateOpening(float dt)
{
    m_openTimer += dt;

    if (m_openTimer < 2.f)
    {
        m_openPhase = 0;
        m_shake = 0.f;
    }
    else if (m_openTimer < 3.5f)
    {
        m_openPhase = 1;
        m_shake = 8.f;
        for (auto& p : m_wall)
        {
            p.vel.y += 600.f * dt;
            p.shape.move(p.vel * dt);
            p.shape.rotate(sf::degrees(p.rot * dt));
        }
    }
    else if (m_openTimer < 6.f)
    {
        m_openPhase = 2;
        m_shake = 0.f;
        float prog = (m_openTimer - 3.5f) / 2.5f;
        if (prog > 1.f) prog = 1.f;
        m_kaiX = -80.f + 180.f * prog;
        m_kaiBody.setPosition({ m_kaiX, GROUND_Y - 40.f });
        m_kaiHead.setPosition({ m_kaiX + 5.f, GROUND_Y - 70.f });
        if (prog > 0.5f)
        {
            float a = std::min((prog - 0.5f) / 0.5f, 1.f);
            m_txtZeroHour.setFillColor(sf::Color(255, 255, 0, (uint8_t)(255 * a)));
        }
    }
    else
    {
        m_openPhase = 3;
    }
}

void Cinematic::drawOpening(sf::RenderWindow& window)
{
    sf::View v = window.getDefaultView();
    if (m_shake > 0.f)
    {
        float dx = (float)(std::rand() % (int)(m_shake * 2)) - m_shake;
        float dy = (float)(std::rand() % (int)(m_shake * 2)) - m_shake;
        v.setCenter({ 512.f + dx, 288.f + dy });
    }
    window.setView(v);

    if (m_openPhase == 0)
    {
        window.clear(sf::Color(10, 10, 10));
        sf::RectangleShape line({ 620.f, 3.f });
        line.setFillColor(sf::Color::Red);
        line.setPosition({ 202.f, 172.f });
        window.draw(line);
        window.draw(m_txtClassified);
        window.draw(m_txtSector);
    }
    else if (m_openPhase == 1)
    {
        window.clear(sf::Color(10, 10, 10));
        sf::RectangleShape light({ 200.f, 240.f });
        light.setFillColor(sf::Color(255, 200, 100, 80));
        light.setPosition({ 412.f, 150.f });
        window.draw(light);
        for (auto& p : m_wall) window.draw(p.shape);
    }
    else if (m_openPhase == 2)
    {
        drawLabWorld(window);
        sf::RectangleShape rem({ 15.f, 250.f });
        rem.setFillColor(sf::Color(60, 40, 40));
        rem.setPosition({ 0.f, GROUND_Y - 250.f });
        window.draw(rem);
        window.draw(m_kaiBody);
        window.draw(m_kaiHead);
        window.draw(m_txtZeroHour);
    }
    else
    {
        drawLabWorld(window);
        float scale = 1.f + (m_openTimer - 6.f) * 0.6f;
        m_txtRun.setScale({ scale, scale });
        window.draw(m_txtRun);
    }

    window.setView(window.getDefaultView());
}

bool Cinematic::isOpeningDone() const { return m_openTimer >= 7.f; }

void Cinematic::resetOpening()
{
    m_openTimer = 0.f;
    m_openPhase = 0;
    m_shake = 0.f;
    m_kaiX = -80.f;
    m_txtZeroHour.setFillColor(sf::Color(255, 255, 0, 0));
    buildWall();
}

void Cinematic::updateWin(float dt)
{
    m_winTimer += dt;
    m_winPhase = (m_winTimer < 2.f) ? 0 : (m_winTimer < 4.f) ? 1 : 2;

    if (m_winPhase == 0)
    {
        for (auto& e : m_explosions)
        {
            e.timer += dt;
            if (e.timer > 1.2f) e.timer = 0.f;
            float p = e.timer / 1.2f;
            float r = e.maxR * p;
            e.shape.setRadius(r);
            e.shape.setOrigin({ r, r });
            e.shape.setFillColor(sf::Color(
                (uint8_t)(255 * (1.f - p)),
                (uint8_t)(80 * (1.f - p)),
                0,
                (uint8_t)(200 * (1.f - p))));
        }
    }
}

void Cinematic::drawWin(sf::RenderWindow& window)
{
    if (m_winPhase == 0)
    {
        window.clear(sf::Color(40, 10, 10));
        for (auto& e : m_explosions) window.draw(e.shape);
        sf::Text t(m_font, "The facility collapses...", 30);
        t.setFillColor(sf::Color(255, 150, 0));
        sf::FloatRect tb = t.getLocalBounds();
        t.setOrigin({ tb.size.x / 2.f, 0.f });
        t.setPosition({ 512.f, 60.f });
        window.draw(t);
    }
    else if (m_winPhase == 1)
    {
        window.clear(sf::Color(20, 5, 5));
        window.draw(m_txtDestroyed);
        sf::RectangleShape line({ 500.f, 2.f });
        line.setFillColor(sf::Color::Red);
        line.setPosition({ 262.f, 340.f });
        window.draw(line);
    }
    else
    {
        window.clear(sf::Color(5, 20, 5));
        window.draw(m_txtDestroyed);
        window.draw(m_txtSurvived);
        window.draw(m_txtContinue);
        for (int i = 0; i < 3; i++)
        {
            sf::Text star(m_font, "*", 50);
            star.setFillColor(sf::Color::Yellow);
            star.setPosition({ 380.f + i * 80.f, 355.f });
            window.draw(star);
        }
    }
}

bool Cinematic::isWinDone() const { return m_winTimer >= 4.f && m_winPhase >= 2; }

void Cinematic::resetWin()
{
    m_winTimer = 0.f;
    m_winPhase = 0;
    buildExplosions();
}