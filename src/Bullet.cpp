#include "Bullet.hpp"
#include "Util/Time.hpp"
#include "Util/Image.hpp"
#include "Game_Define.h"
#include "PVZGame.hpp"

Bullet::Bullet(float start_x, float start_y, float speed, bool is_slow_down, std::string png_name, float damage)
: GameObject(std::make_unique<Util::Image>(png_name), 10)
, m_speed(speed)
, m_destroyed(false)
, m_is_slow_down(is_slow_down)
, m_damage(damage)
{
    m_Transform.translation.x = start_x;
    m_Transform.translation.y = start_y;
}


 void Bullet::Update()
{
    if (m_destroyed) {
        return;
    }

   // LOG_INFO("Bullet::Update {0}, {1}", m_Transform.translation.x, m_Transform.translation.y);
    m_Transform.translation.x += m_speed * (Util::Time::GetDeltaTimeMs() / 1000.0f);
    
    // Remove bullet if it goes off screen
    if (PVZGame::GetInstance().CheckBulletCollisions(m_Transform.translation.x, m_Transform.translation.y, m_is_slow_down, m_damage)) {
        m_destroyed = true;
        SetVisible(false);
       // LOG_INFO("Bullet::Update {0}, {1}", m_Transform.translation.x, m_Transform.translation.y);
   }
   else if (m_Transform.translation.x > LAWN_WIDTH + 100)
   {
        m_destroyed = true;
        SetVisible(false);
      // LOG_INFO("Bullet::Update {0}, {1}", m_Transform.translation.x, m_Transform.translation.y);
   }

    Draw();
}

