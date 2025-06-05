#include "Chomper.hpp"
#include "PVZGame.hpp"
#include "Util/Logger.hpp"

Chomper::Chomper(int grid_x, int grid_y, int mouse_x, int mouse_y, float health)
: Plant(grid_x, grid_y, mouse_x, mouse_y, health, RESOURCE_DIR"/plant/Chomper/ccc")
, m_digesting_animation(std::make_unique<Util::Animation>(std::vector<std::string>{
    RESOURCE_DIR"/plant/Chomper/ChomperDigest/ChomperDigest-0.png",
    RESOURCE_DIR"/plant/Chomper/ChomperDigest/ChomperDigest-1.png",
    RESOURCE_DIR"/plant/Chomper/ChomperDigest/ChomperDigest-2.png",
    RESOURCE_DIR"/plant/Chomper/ChomperDigest/ChomperDigest-3.png",
    RESOURCE_DIR"/plant/Chomper/ChomperDigest/ChomperDigest-4.png",
    RESOURCE_DIR"/plant/Chomper/ChomperDigest/ChomperDigest-5.png",
  },
  false, 1, true, 1))
, m_is_digesting(false)
, m_digest_start_time(0)
{
    LOG_INFO("Chomper planted at grid ({}, {})", grid_x, grid_y);
}

void Chomper::Update()
{
    if (m_destroyed) {
        return;
    }

    Plant::Update();
    if (m_is_digesting) {
        m_digest_start_time += Util::Time::GetDeltaTimeMs();
        if (m_digest_start_time >= 2000.0f) {
            m_is_digesting = false;
            m_digest_start_time = 0.0f;
            m_digesting_animation->Pause();
            SetDrawable(m_animation);
            m_animation->Play();
        }
    }

    if (!m_is_digesting) {
        if (PVZGame::GetInstance().CheckChomperDigest(m_Transform.translation.x, m_Transform.translation.y)) {
            m_is_digesting = true;
            m_animation->Pause();
            SetDrawable(m_digesting_animation);
            m_digesting_animation->Play();
        }
    }
}
