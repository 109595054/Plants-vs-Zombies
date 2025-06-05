#ifndef LAWNMOWER_HPP
#define LAWNMOWER_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"
#include "Game_Define.h"
#include <memory>

class LawnMower : public Util::GameObject {
public:
    LawnMower(int grid_y, float x, float y);

    void Update();
    void Active();

    bool IsDestroyed() const { return m_destroyed; }
    bool IsActive() const { return m_active; }
    int GetGridY() const { return m_grid_y; }

private:
    std::shared_ptr<Util::Animation> m_idle_animation;
    std::shared_ptr<Util::Animation> m_active_animation;
    int m_grid_y;
    bool m_active;
    bool m_destroyed;
    float m_speed;
};

#endif // LAWNMOWER_HPP
