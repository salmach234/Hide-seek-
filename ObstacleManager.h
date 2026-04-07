#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "GroundObstacle.h"
#include "AirObstacle.h"
#include "Missile.h"
#include "WarningIndicator.h"
#include "HealthPickup.h"
#include "MissilePattern.h"
#include "Player.h"

class ObstacleManager
{
public:
    explicit ObstacleManager(sf::Font& font);

    void update(float dt, float speed, float playerY,
        int jumps, int ducks, int phase,
        Player::Health health);
    void draw(sf::RenderWindow& window);
    void reset();
    void clear();
    void stopSpawning();

    std::vector<std::unique_ptr<Obstacle>>& obstacles();
    std::vector<std::unique_ptr<HealthPickup>>& pickups();

private:
    sf::Font& m_font;

    std::vector<std::unique_ptr<Obstacle>>        m_obstacles;
    std::vector<std::unique_ptr<WarningIndicator>> m_warnings;
    std::vector<std::unique_ptr<HealthPickup>>     m_pickups;

    struct Pending
    {
        Missile::MHeight height;
        float            speed;
        float            delay;
        float            timer;
    };
    std::vector<Pending> m_pending;

    bool  m_active;
    float m_spawnTimer;
    float m_interval;
    float m_warnDur;

    int   m_lastType;
    int   m_consecutive;

    bool  m_seqRunning;
    int   m_seqStep;
    float m_seqTimer;
    float m_seqInterval;
    int   m_seqPattern;

    Player::Health m_prevHealth;
    bool  m_healthPending;
    float m_healthTimer;
    float m_healthDelay;

    std::vector<MissilePattern> m_patterns;

    void buildPatterns();
    void spawnNext(float speed, float playerY, int jumps, int ducks, int phase);
    void spawnGround(float speed);
    void spawnAir(float speed, int jumps, int ducks);
    void runMissilePattern(int idx, float speed, int jumps, int ducks, int phase);
    void queueMissile(Missile::MHeight h, float speed, float extraDelay = 0.f);
    bool validPair(Missile::MHeight a, Missile::MHeight b) const;
    Missile::MHeight pickMissileHeight(int jumps, int ducks, int phase) const;
    float minInterval(int phase) const;

    static const int T_GROUND = 0;
    static const int T_AIR = 1;
    static const int T_MISSILE = 2;
};