#include "PotatoMine.hpp"

#include "PVZGame.hpp"
#include "Util/Logger.hpp"

PotatoMine::PotatoMine(int grid_x, int grid_y, int mouse_x, int mouse_y, float health)
: Plant(grid_x, grid_y, mouse_x, mouse_y, health, RESOURCE_DIR"/plant/PotatoMine/PotatoMine")
, m_armed_animation(std::make_unique<Util::Animation>(std::vector<std::string>{
    RESOURCE_DIR"/plant/PotatoMine/1.png"
    },
    false, 50, true, 1000))
, m_exploded_animation(std::make_unique<Util::Animation>(std::vector<std::string>{
    RESOURCE_DIR"/plant/PotatoMine/PotatoMine_mashed.png",
  },
  false, 50, true, 1000))
, m_armed(false)
, m_plant_time(0.0f)
, m_exploded(false)
, m_explosion_time(0.0f)
{
    m_animation->Pause();
    SetDrawable(m_armed_animation);
    m_armed_animation->Play();
}

void PotatoMine::Update()
{
    if (m_destroyed){
        return;
    }

    Plant::Update();
    if (!m_armed) {
        m_plant_time += Util::Time::GetDeltaTimeMs();
        if (m_plant_time >= 3000.0f)
        {
            m_armed = true;
            m_armed_animation->Pause();
            SetDrawable(m_animation);
            m_animation->Play();
        }
    }

    if (m_armed && !m_exploded) {
        if (PVZGame::GetInstance().CheckPotatoMineExplod(m_Transform.translation.x, m_Transform.translation.y, POTATO_MINE_EXPLOSION_RADIUS)) {
            PVZGame::GetInstance().PotatoMineExplod(m_Transform.translation.x, m_Transform.translation.y, POTATO_MINE_EXPLOSION_RADIUS, POTATO_MINE_EXPLOSION_DAMAGE);
            m_exploded = true;
            m_explosion_time = 0;
            m_animation->Pause();
            SetDrawable(m_exploded_animation);
            m_exploded_animation->Play();
        }
    }

    if (m_exploded) {
        m_explosion_time += Util::Time::GetDeltaTimeMs();
        if (m_explosion_time >= 500.0f) {
            m_destroyed = true;
            m_exploded_animation->Pause();
            //PVZGame::GetInstance().PotatoMineExplod(m_Transform.translation.x, m_Transform.translation.y, POTATO_MINE_EXPLOSION_RADIUS, POTATO_MINE_EXPLOSION_DAMAGE);
        }
    }
}
