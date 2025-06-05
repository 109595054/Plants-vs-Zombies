//
// Created by user on 25-5-26.
//

#ifndef GAME_DEFINE_H
#define GAME_DEFINE_H

// Game constants
constexpr int LAWN_START_X = -409.0f;
constexpr int LAWN_END_X = 265.0f;
constexpr int LAWN_START_Y = 286.0f;
constexpr int LAWN_END_Y = -328.0f;
constexpr int GRID_COLS = 9;
constexpr int GRID_ROWS = 5;
constexpr int LAWN_WIDTH = 1280;
constexpr int LAWN_HEIGHT = 720;


constexpr float ZOMBIE_SPEED = 20.0f;
constexpr float FLAG_ZOMBIE_SPEED = 15.0f;
constexpr float BUCKETHEAD_ZOMBIE_SPEED = 10.0f;
constexpr float CONEHEAD_ZOMBIE_SPEED = 12.0f;

constexpr float BULLET_SPEED = 120.0f;
constexpr int SUN_VALUE = 25;
constexpr float SUN_SPAWN_INTERVAL = 30000.0f;
constexpr float SUN_SPAWN_INTERVAL_AUTO = 5000.0f;

constexpr float CARD_WIDTH = 64.0f;
constexpr float CARD_HEIGHT = 90.0f;
constexpr float CARD_START_X = -360.0f;
constexpr float CARD_END_X = 51.0f;
constexpr float CARD_START_Y = 353.0f;
constexpr float CARD_END_Y = 281.0f;
constexpr float CARD_SPACING = 5.0f;

constexpr int PEASHOOTER_COST = 100;
constexpr int SUNFLOWER_COST = 50;
constexpr int CHERRYBOMB_COST = 150;
constexpr int WALLNUT_COST = 50;
constexpr int POTATO_MINE_COST = 25;
constexpr int SNOWPEA_COST = 175;
constexpr int CHOMPER_COST = 150;

enum class PlantType
{
    NONE,
    SHOVEL,
    PEASHOOTER,
    SUN_FLOWER,
    CHERRY_BOMB,
    WALL_NUT,
    POTATO_MINE,
    SNOWPEA,
    CHOMPER,
};


enum class ZombieType {
    NORMAL_ZOMBIE,
    FLAG_ZOMBIE,
    BUCKETHEAD_ZOMBIE,
    CONEHEAD_ZOMBIE,
};


enum GAME_STATUS {
    GAME_STATUS_NONE,
    GAME_STATUS_START,
    GAME_STATUS_PLAYING,
    GAME_STATUS_OVER,
    GAME_STATUS_WIN,
};


enum WAVE_STATUS {
    WAVE_SPAWNING,
    WAVE_WAITING,
    WAVE_COMPLETED,
    LEVEL_COMPLETED,
};



#endif //GAME_DEFINE_H
