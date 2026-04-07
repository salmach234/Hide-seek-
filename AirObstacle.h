#pragma once
#include "Obstacle.h"

class AirObstacle : public Obstacle
{
public:
    enum class Height { Low, Mid, High };

    AirObstacle(float speed, Height h = Height::Low);
    void draw(sf::RenderWindow& window) override;
    Height getHeight() const;

private:
    Height m_height;

    static const float W;
    static const float H;
    static float yForHeight(Height h);
};