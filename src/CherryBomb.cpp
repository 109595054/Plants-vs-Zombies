#include "CherryBomb.hpp"

#include "PVZGame.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"

CherryBomb::CherryBomb(int grid_x, int grid_y, int mouse_x, int mouse_y, float health)
: Plant(grid_x, grid_y, mouse_x, mouse_y, health, RESOURCE_DIR"/plant/CherryBomb/qqq")
, m_exploded_animation(std::make_unique<Util::Animation>(std::vector<std::string>{
    RESOURCE_DIR"/Boom/Boom-0.png",
    RESOURCE_DIR"/Boom/Boom-1.png",
    RESOURCE_DIR"/Boom/Boom-2.png",
    RESOURCE_DIR"/Boom/Boom-3.png",
    RESOURCE_DIR"/Boom/Boom-4.png",
    RESOURCE_DIR"/Boom/Boom-5.png",
    RESOURCE_DIR"/Boom/Boom-6.png",
    RESOURCE_DIR"/Boom/Boom-7.png",
    RESOURCE_DIR"/Boom/Boom-8.png",
    RESOURCE_DIR"/Boom/Boom-9.png",
    RESOURCE_DIR"/Boom/Boom-10.png",
    RESOURCE_DIR"/Boom/Boom-11.png",
    RESOURCE_DIR"/Boom/Boom-12.png",
  },
  false, 50, false, 1000))
, m_fuse_time(0)
, m_explosion_time(0)
, m_exploded(false)
{

}

void CherryBomb::Update()
{
    if (m_destroyed) {
        return;
    }

    Plant::Update();
    if (!m_exploded) {
        m_fuse_time += Util::Time::GetDeltaTimeMs();
        if (!m_exploded && m_fuse_time >= 1500.0f) {
            m_exploded = true;
            m_explosion_time = 0;
            m_animation->Pause();
            SetDrawable(m_exploded_animation);
            m_exploded_animation->Play();
        }
    }

    if (m_exploded)
    {
        m_explosion_time += Util::Time::GetDeltaTimeMs();
        if (m_explosion_time >= 500.0f) {
            m_destroyed = true;
            m_exploded_animation->Pause();
            PVZGame::GetInstance().CherryBombExplod(m_Transform.translation.x, m_Transform.translation.y, EXPLOSION_RADIUS, EXPLOSION_DAMAGE);
            LOG_INFO("CherryBomb exploded at position ({}, {})", m_Transform.translation.x, m_Transform.translation.y);
        }
    }
}
