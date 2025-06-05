#ifndef PLANT_HPP
#define PLANT_HPP

#include "Game_Define.h"
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class Plant : public Util::GameObject 
{
public:
    Plant(int grid_x, int grid_y, int mouse_x, int mouse_y, float health, std::string string_png);
    virtual void Update();
    virtual void TakeDamage(float damage);

    int GetGridX() const { return m_grid_x; }
    int GetGridY() const { return m_grid_y; }
    bool IsDestroyed() const { return m_destroyed; }
    float GetHealth() const { return m_health; }
    virtual bool IsCanAttack() const { return !m_destroyed;}
    
protected:
    std::shared_ptr<Util::Animation> m_animation;
    int m_grid_x;
    int m_grid_y;
    float m_health;
    bool m_destroyed;
};

#endif
