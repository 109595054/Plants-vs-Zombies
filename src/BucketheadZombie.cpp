#include "BucketheadZombie.hpp"
#include "Game_Define.h"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

BucketHeadZombie::BucketHeadZombie(int grid_y, float y, float health, int monster_id, const std::vector<std::string>& paths, const std::vector<std::string> &attack_paths)
: Zombie(ZombieType::BUCKETHEAD_ZOMBIE, grid_y, y, health, BUCKETHEAD_ZOMBIE_SPEED, monster_id,  paths, attack_paths)
, m_bucket_destroyed(false)
, m_bucket_health(100.0f)
{

}


void BucketHeadZombie::TakeDamage(float damage, bool is_die)
{
    if (!m_bucket_destroyed)
    {
        m_bucket_health++;
        if (m_bucket_health >= 10)
        {
            m_bucket_destroyed = true;
            LOG_INFO("Bucket destroyed! Zombie now vulnerable.");
        }
    }
    else
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
        LOG_INFO("Bucket zombie destroyed!");
    }
}
