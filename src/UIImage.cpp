#include "UIImage.hpp"
#include "Util/Time.hpp"
#include "Util/Image.hpp"
#include "Game_Define.h"
#include "PVZGame.hpp"

UIIMAGE::UIIMAGE(float x, float y, std::string png_name)
: GameObject(std::make_unique<Util::Image>(png_name), 0)
{
    m_Transform.translation.x = x;
    m_Transform.translation.y = y;
}


void UIIMAGE::Update()
{
    Draw();
}

void UIIMAGE::SetPosition(float x, float y)
{
    m_Transform.translation.x = x;
    m_Transform.translation.y = y;
}


