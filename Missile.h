#pragma once
#include "Obstacle.h"

class Missile : public Obstacle
{
public:
    enum class MHeight { Low, Mid, High };

    Missile(float speed, MHeight h);

    void update(float dt, float playerY = 0.f) override;
    void draw(sf::RenderWindow& window) override;

    MHeight getMHeight() const;
    static float yForHeight(MHeight h);

private:
    MHeight m_mheight;
    float   m_animTimer;

    static const float W;
    static const float H;
};