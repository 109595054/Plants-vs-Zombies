#include "PlantCard.hpp"
#include "PVZGame.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

PlantCard::PlantCard(PlantType type, float x, float y, int cost, const std::string& imagePath, const std::string& dragimagePath)
: m_plant_type(type)
, m_cost(cost)
, m_original_x(x)
, m_original_y(y)
, m_is_dragging(false)
, m_is_affordable(false)
{
    m_cardImage = std::make_unique<UIIMAGE>(x, y, imagePath);
    m_cardImage->SetZIndex(1);

    m_dragImage = std::make_unique<UIIMAGE>(x, y, dragimagePath);
    m_dragImage->SetZIndex(1);
    m_dragImage->SetVisible(false);
}


void PlantCard::Update()
{
    if (m_is_dragging)
    {
        auto mousePos = Util::Input::GetCursorPosition();
        m_dragImage->SetPosition(mousePos.x, mousePos.y);
        m_dragImage->Update();
        //LOG_INFO("PlantCard::Update {} {}", mousePos.x, mousePos.y);
    }

    m_cardImage->Update();
}


void PlantCard::StartDrag()
{
    if (!m_is_affordable)
    {
        return;
    }

    m_is_dragging = true;
    m_dragImage->SetVisible(true);
    LOG_INFO("Started dragging plant card: {}", static_cast<int>(m_plant_type));
}


bool PlantCard::IsMouseClick(float mouse_x, float mouse_y) const
{
    bool is_click = mouse_x >= m_original_x - CARD_WIDTH / 2 && mouse_x <= m_original_x + CARD_WIDTH / 2 &&  mouse_y >= 275 && mouse_y <= 359;
    //LOG_INFO("PlantCard::IsMouseClick plant card: ({}, {}, {}, {}, {}, {}, {})", mouse_x, mouse_y, m_original_x - CARD_WIDTH / 2,
     //  m_original_x + CARD_WIDTH / 2, CARD_WIDTH, CARD_HEIGHT, is_click);

    return is_click;
}

void PlantCard::EndDrag()
{
    if (!m_is_dragging) {
        return;
    }

    m_is_dragging = false;
    m_dragImage->SetPosition(m_original_x, m_original_y);
    m_dragImage->SetVisible(false);
    LOG_INFO("Ended dragging plant card: {}", static_cast<int>(m_plant_type));
}




