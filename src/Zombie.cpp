#include "Zombie.hpp"
#include "Game_Define.h"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

Zombie::Zombie(ZombieType type, int grid_y, int y, float health, float speed, const std::vector<std::string> &paths, const std::vector<std::string> &attack_paths)
: m_animation(std::make_unique<Util::Animation>(paths, true, 50, true, 1000))
, m_attack_animation(std::make_unique<Util::Animation>(attack_paths, false, 50, true, 1000))
, m_death_animation(std::make_unique<Util::Animation>(std::vector<std::string>{
    RESOURCE_DIR"/Zombie/Die/BoomDie-0.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-1.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-2.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-3.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-4.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-5.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-6.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-7.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-8.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-9.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-10.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-11.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-12.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-13.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-14.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-15.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-16.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-17.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-18.png",
    RESOURCE_DIR"/Zombie/Die/BoomDie-19.png",
  },
  false, 50, false, 1000))
, m_type(type)
, m_grid_y(grid_y)
, m_health(health)
, m_speed(speed)
, m_destroyed(false)
, m_eating(false)
, m_reached_house(false)
, m_is_die(false)
, m_die_time(0)
, m_is_slow_down(false)
, m_start_slow_down_time(0)
{
    m_Transform.translation.x = LAWN_WIDTH / 2;
    m_Transform.translation.y = y;
    SetDrawable(m_animation);
}


void Zombie::Update()
{
    if (m_destroyed) {
        return;
    }

    m_animation->Play();
    Draw();

    m_start_slow_down_time += Util::Time::GetDeltaTimeMs();
    if (m_is_slow_down && m_start_slow_down_time >= 3000.0f) {
        m_start_slow_down_time = 0.0f;
        m_is_slow_down = false;
    }

    if (!m_eating && !m_is_die)
    {
        float speed = m_speed;
        if (m_is_slow_down) {
            speed = m_speed / 2;
        }

        m_Transform.translation.x -= speed * (Util::Time::GetDeltaTimeMs() / 1000.0f);
        if (m_Transform.translation.x < LAWN_START_X)
        {
            m_reached_house = true;
            LOG_INFO("Zombie::Update {}", m_Transform.translation.x);
        }
    }

    if (m_is_die) {
        m_die_time += Util::Time::GetDeltaTimeMs();
        if (m_die_time >= 5000.0f)
        {
            m_destroyed = true;
        }
    }
}


void Zombie::TakeDamage(float damage) 
{
    if (m_is_die || m_destroyed) {
        return;
    }

    m_health -= damage;
    if (m_health <= 0)
    {
        m_is_die = true;
        m_die_time = 0;
        m_animation->Pause();
        m_attack_animation->Pause();
        SetDrawable(m_death_animation);
        m_death_animation->Play();
    }
}

void Zombie::StartEating()
{
    if (m_is_die || m_destroyed) {
        return;
    }

    m_eating = true;
    m_animation->Pause();
    SetDrawable(m_attack_animation);
    m_attack_animation->Play();
    LOG_INFO("Zombie started eating!");
}

void Zombie::StopEating()
{
    m_eating = false;
    m_attack_animation->Pause();
    SetDrawable(m_animation);
    m_animation->Play();
    LOG_INFO("Zombie stopped eating!");
}

void Zombie::SlowDown()
{
    if (m_is_slow_down || m_is_die || m_destroyed) {
        return;
    }

    m_is_slow_down = true;
    m_start_slow_down_time = 0;
}

bool Zombie::IsCanAttack() const {
    if (m_destroyed || m_is_die) {
        return false;
    }

    if (m_Transform.translation.x > 610) {
        return false;
    }

    return true;
}

