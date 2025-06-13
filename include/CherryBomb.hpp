#ifndef CHERRYBOMB_HPP
#define CHERRYBOMB_HPP

#include "Plant.hpp"
#include "Game_Define.h"

static constexpr float EXPLOSION_RADIUS = 200.0f;
static constexpr float EXPLOSION_DAMAGE = 1800.0f;

class CherryBomb : public Plant
{
public:
    CherryBomb(int grid_x, int grid_y, int mouse_x, int mouse_y, float health);
    void Update() override;

private:
    std::shared_ptr<Util::Animation> m_exploded_animation;
    float m_fuse_time;
    float m_explosion_time;
    bool m_exploded;
};


#endif

