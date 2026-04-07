#include "Player.h"
#include "GameConfig.h"
#include <algorithm>
#include <cmath>

const float Player::GRAVITY = 1800.f;
const float Player::JUMP_INITIAL = -620.f;
const float Player::JUMP_HOLD_FORCE = -500.f;
const float Player::JUMP_HOLD_MAX = 0.2f;
const float Player::INV_DURATION = 2.f;

Player::Player()
    : m_velocity(0.f, 0.f),
    m_state(State::Running),
    m_health(Health::Alive),
    m_alive(true),
    m_x(120.f),
    m_y(GROUND_Y - PLAYER_H),
    m_jumpHeld(false),
    m_jumpHeldTime(0.f),
    m_jumpStarted(false),
    m_wasDucking(false),
    m_invTimer(0.f),
    m_invincible(false),
    m_blinkTimer(0.f),
    m_visible(true),
    m_jumpCount(0),
    m_duckCount(0)
{
    m_body.setFillColor(sf::Color(60, 120, 220));
    m_body.setOutlineColor(sf::Color(30, 60, 140));
    m_body.setOutlineThickness(2.f);

    m_head.setSize({ 30.f, 30.f });
    m_head.setFillColor(sf::Color(255, 200, 150));
    m_head.setOutlineColor(sf::Color(180, 120, 80));
    m_head.setOutlineThickness(2.f);

    syncShapes();
}

void Player::takeHit()
{
    if (m_invincible) return;
    if (m_health == Health::Alive)
    {
        m_health = Health::Injured;
        m_invincible = true;
        m_invTimer = 0.f;
        m_body.setFillColor(sf::Color(220, 80, 80));
        m_body.setOutlineColor(sf::Color(140, 30, 30));
    }
    else if (m_health == Health::Injured)
    {
        m_health = Health::Dead;
        m_alive = false;
    }
}

void Player::heal()
{
    if (m_health != Health::Injured) return;
    m_health = Health::Alive;
    m_invincible = true;
    m_invTimer = 0.f;
    m_body.setFillColor(sf::Color(60, 120, 220));
    m_body.setOutlineColor(sf::Color(30, 60, 140));
}

void Player::handleInput(float dt)
{
    if (!m_alive) return;

    bool upHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    bool downHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

    // القفز يبدأ فورًا عند أول ضغطة
    if (upHeld && m_state == State::Running && !m_jumpStarted)
    {
        m_velocity.y = JUMP_INITIAL;
        m_state = State::Jumping;
        m_jumpStarted = true;
        m_jumpHeld = true;
        m_jumpHeldTime = 0.f;
        m_jumpCount++;
    }

    // زيادة القوة أثناء الضغط المستمر
    if (m_jumpHeld && upHeld && m_state == State::Jumping)
    {
        m_jumpHeldTime += dt;
        if (m_jumpHeldTime < JUMP_HOLD_MAX)
            m_velocity.y += JUMP_HOLD_FORCE * dt;
    }

    if (!upHeld)
    {
        m_jumpHeld = false;
        m_jumpStarted = false;
    }

    if (downHeld && m_state == State::Running)
    {
        if (!m_wasDucking) { m_duckCount++; m_wasDucking = true; }
        m_state = State::Ducking;
    }
    else if (!downHeld && m_state == State::Ducking)
    {
        m_state = State::Running;
        m_wasDucking = false;
    }

    if (!downHeld) m_wasDucking = false;
}

void Player::update(float dt)
{
    if (!m_alive) return;

    if (m_invincible)
    {
        m_invTimer += dt;
        m_blinkTimer += dt;
        if (m_blinkTimer >= 0.1f) { m_blinkTimer = 0.f; m_visible = !m_visible; }
        if (m_invTimer >= INV_DURATION) { m_invincible = false; m_visible = true; }
    }

    applyGravity(dt);
    clampToGround();
    syncShapes();
}

void Player::moveRight(float speed, float dt)
{
    m_state = State::Running;
    m_x += speed * dt;
    syncShapes();
}

void Player::applyGravity(float dt)
{
    if (m_state == State::Jumping)
    {
        m_velocity.y += GRAVITY * dt;
        m_y += m_velocity.y * dt;
    }
}

void Player::clampToGround()
{
    float h = (m_state == State::Ducking) ? PLAYER_DUCK_H : PLAYER_H;
    float floor = GROUND_Y - h;
    if (m_y >= floor)
    {
        m_y = floor;
        m_velocity.y = 0.f;
        m_jumpStarted = false;
        if (m_state == State::Jumping) m_state = State::Running;
    }
}

void Player::syncShapes()
{
    if (m_state == State::Ducking)
    {
        m_body.setSize({ PLAYER_W + 10.f, PLAYER_DUCK_H });
        m_body.setPosition({ m_x, GROUND_Y - PLAYER_DUCK_H });
        m_head.setPosition({ m_x + PLAYER_W, GROUND_Y - PLAYER_DUCK_H - 5.f });
    }
    else
    {
        m_body.setSize({ PLAYER_W, PLAYER_H - 35.f });
        m_body.setPosition({ m_x, m_y + 35.f });
        m_head.setPosition({ m_x + 5.f, m_y });
    }
}

void Player::draw(sf::RenderWindow& window)
{
    if (!m_alive || !m_visible) return;
    window.draw(m_body);
    window.draw(m_head);

    if (m_health == Health::Injured)
    {
        sf::CircleShape badge(5.f);
        badge.setFillColor(sf::Color(255, 50, 50));
        badge.setPosition({ m_x + PLAYER_W - 4.f, m_y - 12.f });
        window.draw(badge);
    }
}

sf::FloatRect Player::getBounds() const
{
    sf::FloatRect b = m_body.getGlobalBounds();
    sf::FloatRect h = m_head.getGlobalBounds();
    float top = std::min(b.position.y, h.position.y);
    float left = std::min(b.position.x, h.position.x);
    float right = std::max(b.position.x + b.size.x, h.position.x + h.size.x);
    float bottom = std::max(b.position.y + b.size.y, h.position.y + h.size.y);
    return sf::FloatRect({ left, top }, { right - left, bottom - top });
}

Player::State  Player::getState()     const { return m_state; }
Player::Health Player::getHealth()    const { return m_health; }
bool           Player::isAlive()      const { return m_alive; }
float          Player::getX()         const { return m_x; }
float          Player::getY()         const { return m_y; }
int            Player::getJumpCount() const { return m_jumpCount; }
int            Player::getDuckCount() const { return m_duckCount; }