#include "PeaShooter.hpp"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"
#include "Bullet.hpp"
#include "PVZGame.hpp"

PeaShooter::PeaShooter(int grid_x, int grid_y, int mouse_x, int mouse_y, float health)
: Plant(grid_x, grid_y, mouse_x, mouse_y, health, RESOURCE_DIR"/plant/Peashooter/Peashooter")
, m_last_shoot_time(0)
{
    m_bullets.clear();
}
    
    
void PeaShooter::Update()
{
    Plant::Update();
    float currentTime = Util::Time::GetElapsedTimeMs();
    
    // Shoot every 2 seconds if zombies are present
    if (currentTime - m_last_shoot_time > 4000.0f && PVZGame::GetInstance().ShouldShoot(m_grid_y))
    {
        m_bullets.push_back(std::make_unique<Bullet>(m_Transform.translation.x, m_Transform.translation.y + 14,  BULLET_SPEED,  false, RESOURCE_DIR"/plant/PeaNormal_0.png", 10));
        m_last_shoot_time = currentTime;
    }

    RecycleBullet();
}


void PeaShooter::RecycleBullet()
{
    for (auto it = m_bullets.begin(); it != m_bullets.end();)
    {
        (*it)->Update();
        if ((*it)->IsDestroyed())
        {
            it = m_bullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}