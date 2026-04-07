#include "ObstacleManager.h"
#include "GameConfig.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

ObstacleManager::ObstacleManager(sf::Font& font)
    : m_font(font),
    m_active(true),
    m_spawnTimer(0.f),
    m_interval(3.0f),
    m_warnDur(0.9f),
    m_lastType(-1),
    m_consecutive(0),
    m_seqRunning(false),
    m_seqStep(0),
    m_seqTimer(0.f),
    m_seqInterval(1.1f),
    m_seqPattern(-1),
    m_prevHealth(Player::Health::Alive),
    m_healthPending(false),
    m_healthTimer(0.f),
    m_healthDelay(0.f)
{
    buildPatterns();
}

void ObstacleManager::buildPatterns()
{
    using MH = Missile::MHeight;
    using MK = MissilePattern::Kind;

    m_patterns.push_back({ MK::Single,     { {MH::Low,  0.f} },                         1 });
    m_patterns.push_back({ MK::Single,     { {MH::Mid,  0.f} },                         1 });
    m_patterns.push_back({ MK::Single,     { {MH::High, 0.f} },                         1 });
    m_patterns.push_back({ MK::Double,     { {MH::Low,  0.f}, {MH::High, 0.f} },        2 });
    m_patterns.push_back({ MK::Double,     { {MH::High, 0.f}, {MH::Mid,  0.f} },        2 });
    m_patterns.push_back({ MK::Double,     { {MH::Mid,  0.f}, {MH::Low,  0.f} },        2 });
    m_patterns.push_back({ MK::Sequential, { {MH::High, 0.f}, {MH::Mid, 1.1f}, {MH::Low, 1.1f} }, 2 });
    m_patterns.push_back({ MK::Sequential, { {MH::Low,  0.f}, {MH::Mid, 1.0f}, {MH::High,1.0f} }, 3 });
}

bool ObstacleManager::validPair(Missile::MHeight a, Missile::MHeight b) const
{
    return std::abs(Missile::yForHeight(a) - Missile::yForHeight(b)) >= 90.f;
}

Missile::MHeight ObstacleManager::pickMissileHeight(int jumps, int ducks, int phase) const
{
    using MH = Missile::MHeight;
    if (phase == 1) return (std::rand() % 2 == 0) ? MH::Low : MH::High;
    if (jumps > ducks + 5) return MH::Mid;
    if (ducks > jumps + 5) return MH::Low;
    int r = std::rand() % 3;
    if (r == 0) return MH::Low;
    if (r == 1) return MH::Mid;
    return              MH::High;
}

float ObstacleManager::minInterval(int phase) const
{
    if (phase == 1) return 2.2f;
    if (phase == 2) return 1.5f;
    return                  1.1f;
}

void ObstacleManager::queueMissile(Missile::MHeight h, float speed, float extraDelay)
{
    m_warnings.push_back(
        std::make_unique<WarningIndicator>(m_font, h, m_warnDur));
    m_pending.push_back({ h, speed, m_warnDur + extraDelay, 0.f });
}

void ObstacleManager::runMissilePattern(int idx, float speed,
    int jumps, int ducks, int phase)
{
    const MissilePattern& p = m_patterns[idx];
    float ms = speed * 1.2f;

    if (p.kind == MissilePattern::Kind::Single)
    {
        queueMissile(p.entries[0].height, ms);
    }
    else if (p.kind == MissilePattern::Kind::Double)
    {
        if (p.entries.size() >= 2 &&
            validPair(p.entries[0].height, p.entries[1].height))
        {
            queueMissile(p.entries[0].height, ms);
            queueMissile(p.entries[1].height, ms);
        }
        else
        {
            queueMissile(Missile::MHeight::Low, ms);
        }
    }
    else
    {
        m_seqRunning = true;
        m_seqStep = 1;
        m_seqTimer = 0.f;
        m_seqInterval = (phase == 3) ? 0.9f : 1.1f;
        m_seqPattern = idx;
        queueMissile(p.entries[0].height, ms);
    }
}

void ObstacleManager::spawnGround(float speed)
{
    m_obstacles.push_back(std::make_unique<GroundObstacle>(speed));
}

void ObstacleManager::spawnAir(float speed, int jumps, int ducks)
{
    AirObstacle::Height h;
    if (jumps > ducks + 5) h = AirObstacle::Height::Mid;
    else if (ducks > jumps + 5) h = AirObstacle::Height::Low;
    else
    {
        int r = std::rand() % 3;
        h = (r == 0) ? AirObstacle::Height::Low
            : (r == 1) ? AirObstacle::Height::Mid
            : AirObstacle::Height::High;
    }
    m_obstacles.push_back(std::make_unique<AirObstacle>(speed, h));
}

void ObstacleManager::spawnNext(float speed, float playerY,
    int jumps, int ducks, int phase)
{
    int roll = std::rand() % 100;
    int type;

    if (phase == 1) type = (roll < 50) ? T_GROUND : (roll < 75 ? T_AIR : T_MISSILE);
    else if (phase == 2) type = (roll < 25) ? T_GROUND : (roll < 40 ? T_AIR : T_MISSILE);
    else                 type = (roll < 15) ? T_GROUND : (roll < 25 ? T_AIR : T_MISSILE);

    if (type == m_lastType && m_consecutive >= 2)
        type = (type == T_GROUND) ? T_MISSILE : T_GROUND;

    if (type == m_lastType) m_consecutive++;
    else                    m_consecutive = 1;
    m_lastType = type;

    if (type == T_GROUND)
    {
        spawnGround(speed);
    }
    else if (type == T_AIR)
    {
        spawnAir(speed, jumps, ducks);
    }
    else
    {
        int pr = std::rand() % 100;
        int kind = (pr < 40) ? 0 : (pr < 80 ? 1 : 2);

        std::vector<int> eligible;
        for (int i = 0; i < (int)m_patterns.size(); i++)
        {
            if (m_patterns[i].minPhase > phase) continue;
            int k = (m_patterns[i].kind == MissilePattern::Kind::Single) ? 0
                : (m_patterns[i].kind == MissilePattern::Kind::Double) ? 1 : 2;
            if (k == kind) eligible.push_back(i);
        }

        if (eligible.empty())
            for (int i = 0; i < (int)m_patterns.size(); i++)
                if (m_patterns[i].minPhase <= phase) eligible.push_back(i);

        if (!eligible.empty())
            runMissilePattern(eligible[std::rand() % eligible.size()],
                speed, jumps, ducks, phase);
    }
}

void ObstacleManager::update(float dt, float speed, float playerY,
    int jumps, int ducks, int phase,
    Player::Health health)
{
    if (m_prevHealth == Player::Health::Alive &&
        health == Player::Health::Injured)
    {
        m_healthPending = true;
        m_healthTimer = 0.f;
        m_healthDelay = 3.f + (float)(std::rand() % 4);
    }
    m_prevHealth = health;

    for (auto& o : m_obstacles) o->update(dt, playerY);
    m_obstacles.erase(
        std::remove_if(m_obstacles.begin(), m_obstacles.end(),
            [](const std::unique_ptr<Obstacle>& o) { return o->isOffScreen(); }),
        m_obstacles.end());

    for (auto& w : m_warnings) w->update(dt);
    m_warnings.erase(
        std::remove_if(m_warnings.begin(), m_warnings.end(),
            [](const std::unique_ptr<WarningIndicator>& w) { return w->isDone(); }),
        m_warnings.end());

    for (auto& pm : m_pending) pm.timer += dt;
    for (auto& pm : m_pending)
        if (pm.timer >= pm.delay)
            m_obstacles.push_back(std::make_unique<Missile>(pm.speed, pm.height));
    m_pending.erase(
        std::remove_if(m_pending.begin(), m_pending.end(),
            [](const Pending& pm) { return pm.timer >= pm.delay; }),
        m_pending.end());

    for (auto& pu : m_pickups) pu->update(dt, speed);
    m_pickups.erase(
        std::remove_if(m_pickups.begin(), m_pickups.end(),
            [](const std::unique_ptr<HealthPickup>& p) {
                return p->isCollected() || p->isOffScreen(); }),
                m_pickups.end());

    if (m_healthPending && health == Player::Health::Injured)
    {
        m_healthTimer += dt;
        if (m_healthTimer >= m_healthDelay && m_pickups.empty())
        {
            if (std::rand() % 2 == 0)
                m_pickups.push_back(
                    std::make_unique<HealthPickup>(1040.f, GROUND_Y - 95.f));
            m_healthPending = false;
            m_healthTimer = 0.f;
        }
    }

    if (health != Player::Health::Injured)
        m_healthPending = false;

    if (m_seqRunning)
    {
        m_seqTimer += dt;
        if (m_seqTimer >= m_seqInterval &&
            m_seqStep < (int)m_patterns[m_seqPattern].entries.size())
        {
            queueMissile(m_patterns[m_seqPattern].entries[m_seqStep].height,
                speed * 1.2f);
            m_seqStep++;
            m_seqTimer = 0.f;
            if (m_seqStep >= (int)m_patterns[m_seqPattern].entries.size())
                m_seqRunning = false;
        }
    }

    if (!m_active || m_seqRunning) return;

    float safe = 2.f * 700.f / 1800.f + 0.4f;
    if (m_interval < safe) m_interval = safe;

    m_spawnTimer += dt;
    if (m_spawnTimer >= m_interval)
    {
        m_spawnTimer = 0.f;
        spawnNext(speed, playerY, jumps, ducks, phase);
        float mn = minInterval(phase);
        if (m_interval > mn)   m_interval -= 0.025f;
        if (m_warnDur > 0.4f)  m_warnDur -= 0.003f;
    }
}

void ObstacleManager::draw(sf::RenderWindow& window)
{
    for (auto& o : m_obstacles) o->draw(window);
    for (auto& w : m_warnings)  w->draw(window);
    for (auto& pu : m_pickups)   pu->draw(window);
}

void ObstacleManager::clear()
{
    m_obstacles.clear();
    m_warnings.clear();
    m_pending.clear();
    m_pickups.clear();
    m_seqRunning = false;
}

void ObstacleManager::reset()
{
    clear();
    m_active = true;
    m_spawnTimer = 0.f;
    m_interval = 3.0f;
    m_warnDur = 0.9f;
    m_lastType = -1;
    m_consecutive = 0;
    m_seqRunning = false;
    m_seqStep = 0;
    m_seqTimer = 0.f;
    m_healthPending = false;
    m_healthTimer = 0.f;
    m_prevHealth = Player::Health::Alive;
}

void ObstacleManager::stopSpawning() { m_active = false; }

std::vector<std::unique_ptr<Obstacle>>& ObstacleManager::obstacles() { return m_obstacles; }
std::vector<std::unique_ptr<HealthPickup>>& ObstacleManager::pickups() { return m_pickups; }