#include "Plant.hpp"

Plant::Plant(int grid_x, int grid_y, int mouse_x, int mouse_y, float health, std::string string_png)
: m_animation(std::make_unique<Util::Animation>(
    std::vector<std::string>{
        string_png + ".png",
        string_png + "1.png",
        string_png + "2.png",
        string_png + "3.png",
        string_png + "4.png",
        string_png + "5.png",
        string_png + "6.png",
        string_png + "7.png",
        string_png + "8.png",
    },
    true, 50, true, 1000))
, m_grid_x(grid_x)
, m_grid_y(grid_y)
, m_health(health)
, m_destroyed(false)
{
    m_Transform.translation.x = mouse_x;
    m_Transform.translation.y = mouse_y;
    SetDrawable(m_animation);
}


void Plant::Update()
{
    if (m_destroyed) {
        return;
    }

    m_animation->Play();
    Draw();
}


void Plant::TakeDamage(float damage) 
{
    m_health -= damage;
    if (m_health <= 0)
    {
        m_destroyed = true;
    }
}

