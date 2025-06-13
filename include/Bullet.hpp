#ifndef BULLET_HPP
#define BULLET_HPP

#include "Util/GameObject.hpp"

class Bullet : public Util::GameObject 
{
public:
    Bullet(float start_x, float start_y, float speed, bool is_slow_down, std::string png_name, float damage);
    virtual void Update();

    bool IsDestroyed() const { return m_destroyed; }
    void Destroy() { m_destroyed = true; }

private:
    float m_speed;
    bool m_destroyed;
    bool m_is_slow_down;
    float m_damage;
};

#endif
