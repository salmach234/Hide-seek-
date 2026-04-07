#pragma once
#include "Obstacle.h"

class GroundObstacle : public Obstacle
{
public:
    explicit GroundObstacle(float speed);
    void draw(sf::RenderWindow& window) override;

private:
    static const float W;
    static const float H;
};