#include "Sun.hpp"
#include "Util/Time.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Image.hpp"


Sun::Sun(float x, float y, bool is_can_auto_collected)
:m_animation(std::make_unique<Util::Animation>(
    std::vector<std::string>{
        RESOURCE_DIR"/sun/1.png",
        RESOURCE_DIR"/sun/2.png",
        RESOURCE_DIR"/sun/3.png",
        RESOURCE_DIR"/sun/4.png",
        RESOURCE_DIR"/sun/5.png",
        RESOURCE_DIR"/sun/6.png",
        RESOURCE_DIR"/sun/7.png",
        RESOURCE_DIR"/sun/8.png",
        RESOURCE_DIR"/sun/9.png",
        RESOURCE_DIR"/sun/10.png",
        RESOURCE_DIR"/sun/11.png",
        RESOURCE_DIR"/sun/12.png",
        RESOURCE_DIR"/sun/13.png",
        RESOURCE_DIR"/sun/14.png",
        RESOURCE_DIR"/sun/15.png",
        RESOURCE_DIR"/sun/16.png",
        RESOURCE_DIR"/sun/17.png",
        RESOURCE_DIR"/sun/18.png",
        RESOURCE_DIR"/sun/19.png",
        RESOURCE_DIR"/sun/20.png",
        RESOURCE_DIR"/sun/21.png",
        RESOURCE_DIR"/sun/22.png",
        RESOURCE_DIR"/sun/23.png",
        RESOURCE_DIR"/sun/24.png",
        RESOURCE_DIR"/sun/25.png",
        RESOURCE_DIR"/sun/26.png",
        RESOURCE_DIR"/sun/27.png",
        RESOURCE_DIR"/sun/28.png",
        RESOURCE_DIR"/sun/29.png",
    },
    true, 50, true, 1000))
, m_collected(false)
, m_life_time(0)
, m_is_can_auto_collected(is_can_auto_collected)
, m_destroyed(false)
{
    m_Transform.translation.x = x;
    m_Transform.translation.y = y;
    SetDrawable(m_animation);
}


void Sun::Update()
{
    if (m_destroyed) {
        return;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB))
    {
        auto mousePos = Util::Input::GetCursorPosition();
        float dx = mousePos.x - m_Transform.translation.x;
        float dy = mousePos.y - m_Transform.translation.y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance < 30.0f)
        {
            m_collected = true;
            m_destroyed = true;
            SetVisible(false);
        }
    }
    else {
        m_life_time += Util::Time::GetDeltaTimeMs();
        if (m_life_time > 10000.0f)
        {
            if (m_is_can_auto_collected) {
                m_collected = true;
            }

            m_destroyed = true;
            SetVisible(false);
        }
    }
}




