#include "BucketheadZombie.hpp"
#include "Game_Define.h"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

BucketHeadZombie::BucketHeadZombie(int grid_y, float y, float health, const std::vector<std::string>& paths, const std::vector<std::string> &attack_paths)
: Zombie(ZombieType::BUCKETHEAD_ZOMBIE, grid_y, y, health, BUCKETHEAD_ZOMBIE_SPEED, paths, attack_paths)
, m_bucket_destroyed(false)
, m_bucket_health(100.0f)
{

}


void BucketHeadZombie::TakeDamage(float damage)
{
    if (!m_bucket_destroyed)
    {
        m_bucket_health -= damage;
        if (m_bucket_health <= 0)
        {
            m_bucket_destroyed = true;
            LOG_INFO("Bucket destroyed! Zombie now vulnerable.");
        }
    }
    else
    {
        m_health -= damage;
        if (m_health <= 0)
        {
            m_destroyed = true;
            LOG_INFO("Bucket zombie destroyed!");
        }
    }
}
