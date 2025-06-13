#include "ConeheadZombie.hpp"
#include "Game_Define.h"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

ConeheadZombie::ConeheadZombie(int grid_y, float y, float health, int monster_id, const std::vector<std::string>& paths, const std::vector<std::string> &attack_paths)
: Zombie(ZombieType::CONEHEAD_ZOMBIE, grid_y, y, health, CONEHEAD_ZOMBIE_SPEED, monster_id,  paths, attack_paths)
, m_cone_destroyed(false)
, m_cone_health(100.0f)
{

}


void ConeheadZombie::TakeDamage(float damage, bool is_die)
{
    float remain_damage = damage - m_cone_health;
    if (!m_cone_destroyed)
    {
        m_cone_health -= damage;
        if (m_cone_health <= 0)
        {
            m_cone_destroyed = true;
            LOG_INFO("Cone destroyed! Zombie now vulnerable.");
        }
    }

    if (remain_damage > 0)
    {
        m_health -= damage;
    }

    if (is_die) {
        m_health = 0;
    }

    if (m_health <= 0)
    {
        m_is_die = true;
        m_die_time = 0;
        m_animation->Pause();
        m_attack_animation->Pause();
        SetDrawable(m_death_animation);
        m_death_animation->Play();
        LOG_INFO("Conehead zombie destroyed!");
    }
}
