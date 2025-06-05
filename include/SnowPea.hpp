#ifndef SNOWPEA_HPP
#define SNOWPEA_HPP

#include "Plant.hpp"
#include "Game_Define.h"
#include "Bullet.hpp"

class SnowPea : public Plant
{
public:
    SnowPea(int grid_x, int grid_y, int mouse_x, int mouse_y, float health);
    void Update() override;

private:
    void RecycleBullet();

    float m_last_shoot_time;
    std::vector<std::unique_ptr<Bullet>> m_bullets;
};

#endif //SNOWPEA_HPP
