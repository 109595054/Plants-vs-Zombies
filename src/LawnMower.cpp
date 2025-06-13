#include "LawnMower.hpp"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"
#include <cmath>

#include "PVZGame.hpp"

LawnMower::LawnMower(int grid_y, float x, float y)
: m_grid_y(grid_y)
, m_active(false)
, m_destroyed(false)
, m_speed(100.0f)
{
    m_Transform.translation.x = x;
    m_Transform.translation.y = y;

    std::vector<std::string> idle_paths;
    idle_paths.push_back(RESOURCE_DIR"/car.png");
    m_idle_animation = std::make_unique<Util::Animation>(idle_paths, true, 100, true, 1000);

    std::vector<std::string> active_paths;
    active_paths.push_back(RESOURCE_DIR"/car.png");
    m_active_animation = std::make_unique<Util::Animation>(active_paths, true, 100, true, 1000);

    SetDrawable(m_idle_animation);
    LOG_INFO("LawnMower created at grid_y: {}, position: ({}, {})", m_grid_y, x, y);
}


void LawnMower::Update()
{
    if (m_destroyed)
    {
        return;
    }
    
    if (m_active)
    {
        m_Transform.translation.x += m_speed * (Util::Time::GetDeltaTimeMs() / 1000.0f);
        m_active_animation->Play();
        PVZGame::GetInstance().CheckLawnerTrigger(m_grid_y, m_Transform.translation.x, 80.0f);

        if (m_Transform.translation.x > LAWN_END_X + 100)
        {
            m_destroyed = true;
            LOG_INFO("LawnMower destroyed after reaching end of screen");
        }
    }
    else
    {
        m_idle_animation->Play();
    }
    
    Draw();
}

void LawnMower::Active()
{
    if (m_active || m_destroyed)
    {
        return;
    }
    
    m_active = true;
    SetDrawable(m_active_animation);
    LOG_INFO("LawnMower active at grid_y: {}", m_grid_y);
}
