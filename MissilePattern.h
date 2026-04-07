#pragma once
#include "Missile.h"
#include <vector>

struct MissileEntry
{
    Missile::MHeight height;
    float            delayAfterPrev;
};

struct MissilePattern
{
    enum class Kind { Single, Double, Sequential };

    Kind                    kind;
    std::vector<MissileEntry> entries;
    int                     minPhase;
};