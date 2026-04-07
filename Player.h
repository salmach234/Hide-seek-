#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
    enum class State { Running, Jumping, Ducking };
    enum class Health { Alive, Injured, Dead };

    Player();

    void handleInput(float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void moveRight(float speed, float dt);
    void takeHit();
    void heal();

    sf::FloatRect getBounds()    const;
    State         getState()     const;
    Health        getHealth()    const;
    bool          isAlive()      const;
    float         getX()         const;
    float         getY()         const;
    int           getJumpCount() const;
    int           getDuckCount() const;

private:
    sf::RectangleShape m_head;
    sf::RectangleShape m_body;

    sf::Vector2f m_velocity;
    State        m_state;
    Health       m_health;
    bool         m_alive;
    float        m_x;
    float        m_y;

    bool  m_jumpHeld;
    float m_jumpHeldTime;
    bool  m_jumpStarted;
    bool  m_wasDucking;

    float m_invTimer;
    bool  m_invincible;
    float m_blinkTimer;
    bool  m_visible;

    int   m_jumpCount;
    int   m_duckCount;

    static const float GRAVITY;
    static const float JUMP_INITIAL;
    static const float JUMP_HOLD_FORCE;
    static const float JUMP_HOLD_MAX;
    static const float INV_DURATION;

    void applyGravity(float dt);
    void clampToGround();
    void syncShapes();
};