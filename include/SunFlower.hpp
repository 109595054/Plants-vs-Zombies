#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "Plant.hpp"
#include "Game_Define.h"


class SunFlower : public Plant
{
public:
    SunFlower(int grid_x, int grid_y, int mouse_x, int mouse_y, float health, int gem_sun_interval);
    void Update() override;

private:
    float m_last_sun_time;
    int m_gen_sun_interval;
};

#endif //SUNFLOWER_H
