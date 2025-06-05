#include "SunFlower.hpp"

#include "PVZGame.hpp"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

SunFlower::SunFlower(int grid_x, int grid_y, int mouse_x, int mouse_y, float health, int gem_sun_interval)
: Plant(grid_x, grid_y, mouse_x, mouse_y, health, RESOURCE_DIR"/plant/SunFlower/SunFlower")
, m_last_sun_time(0)
, m_gen_sun_interval(gem_sun_interval)
{

}


void SunFlower::Update()
{
    if (m_destroyed) {
        return;
    }

    Plant::Update();
    float currentTime = Util::Time::GetElapsedTimeMs();
    if (currentTime - m_last_sun_time >= m_gen_sun_interval)
    {
        PVZGame::GetInstance().GenSun(m_Transform.translation.x + 15, m_Transform.translation.y + 15);
        m_last_sun_time = currentTime;
        LOG_INFO("SunFlower::Update gen Sun");
    }
}

