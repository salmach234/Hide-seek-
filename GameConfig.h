#pragma once
#include <SFML/Graphics.hpp>

const int   WIN_W = 1024;
const int   WIN_H = 576;
const int   FPS = 60;
const float GROUND_Y = WIN_H - 120.f;

const float TOTAL_TIME = 180.f;
const float MAX_DISTANCE = 9000.f;
const float WIN_DISTANCE = 8500.f;

const float SPD_PHASE1 = 250.f;
const float SPD_PHASE2 = 380.f;
const float SPD_PHASE3 = 520.f;
const float SPD_MAX = 700.f;

const float PLAYER_H = 70.f;
const float PLAYER_DUCK_H = 35.f;
const float PLAYER_W = 40.f;

const sf::Color C_MENU_BG(15, 15, 40);
const sf::Color C_LAB_BG(15, 15, 25);
const sf::Color C_LAB_FLOOR(40, 40, 52);
const sf::Color C_LAB_CEIL(35, 35, 50);

enum class GameState
{
    MainMenu,
    ModeSelect,
    Opening,
    Playing,
    WinSequence,
    WinCinematic,
    Win,
    About,
    Settings,
    GameOver
};