#include "ConeheadZombie.hpp"
#include "Game_Define.h"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

ConeheadZombie::ConeheadZombie(int grid_y, float y, float health, const std::vector<std::string>& paths, const std::vector<std::string> &attack_paths)
: Zombie(ZombieType::CONEHEAD_ZOMBIE, grid_y, y, health, CONEHEAD_ZOMBIE_SPEED, paths, attack_paths)
, m_cone_destroyed(false)
, m_cone_health(100.0f)
{

}


void ConeheadZombie::TakeDamage(float damage)
{
    if (!m_cone_destroyed)
    {
        m_cone_health -= damage;
        if (m_cone_health <= 0)
        {
            m_cone_destroyed = true;
            LOG_INFO("Cone destroyed! Zombie now vulnerable.");
        }
    }
    else
    {
        m_health -= damage;
        if (m_health <= 0)
        {
            m_destroyed = true;
            LOG_INFO("Conehead zombie destroyed!");
        }
    }
}
