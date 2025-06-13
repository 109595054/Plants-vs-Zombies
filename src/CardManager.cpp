#include "CardManager.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/BGM.hpp"

CardManager::CardManager()
: m_draggedCard(nullptr)
{
    m_cards.clear();
}


void CardManager::Update()
{
    for (auto& card : m_cards)
    {
        card->Update();
    }
}


void CardManager::InitCard(int level)
{
    m_cards.clear();
    CreateCard(PlantType::SHOVEL, 0, 0, RESOURCE_DIR"/shovel.png", RESOURCE_DIR"/shovel.png");
    CreateCard(PlantType::PEASHOOTER, 1, PEASHOOTER_COST, RESOURCE_DIR"/card/card_peashooter.png", RESOURCE_DIR"/card/card_peashooter_drag.png");
    
    if (level > 1)
    {
        CreateCard(PlantType::SUN_FLOWER, 2, SUNFLOWER_COST, RESOURCE_DIR"/card/card_sunflower.png", RESOURCE_DIR"/card/card_sunflower_drag.png");
    }
    
    if (level > 2)
    {
        CreateCard(PlantType::CHERRY_BOMB, 3, CHERRYBOMB_COST, RESOURCE_DIR"/card/card_cherrybomb.png", RESOURCE_DIR"/card/card_cherrybomb_drag.png");
    }
    
    if (level > 3)
    {
        CreateCard(PlantType::WALL_NUT, 4, WALLNUT_COST, RESOURCE_DIR"/card/card_wallnut.png", RESOURCE_DIR"/card/card_wallnut_drag.png");
    }

    if (level > 5)
    {
        CreateCard(PlantType::POTATO_MINE, 5, POTATO_MINE_COST, RESOURCE_DIR"/card/card_potatomine.png", RESOURCE_DIR"/card/card_potatomine_drag.png");
    }

    if (level > 6)
    {
        CreateCard(PlantType::SNOWPEA, 6, SNOWPEA_COST, RESOURCE_DIR"/card/card_snowpea.png", RESOURCE_DIR"/card/card_snowpea_drag.png");
    }

    if (level > 8) {
        CreateCard(PlantType::CHOMPER, 7, CHOMPER_COST, RESOURCE_DIR"/card/card_chomper.png", RESOURCE_DIR"/card/card_chomper_drag.png");
    }

    LOG_INFO("CardManager initialized for level {}", level);
}


void CardManager::OnMouseLBPressed(glm::vec2 &mouse_pos)
{
    if(!HasDraggedPlant())
    {
        for (auto& card : m_cards)
        {
            if (card->IsMouseClick(mouse_pos.x, mouse_pos.y))
            {
                card->StartDrag();
                m_draggedCard = card.get();
                break;
            }
        }
    }
}


void CardManager::OnMouseLBUp()
{
    if (m_draggedCard != nullptr)
    {
        m_draggedCard->EndDrag();
        m_draggedCard = nullptr;
    }
}

void CardManager::HandleMouseInput()
{
    auto mousePos = Util::Input::GetCursorPosition();
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB))
    {
        for (auto& card : m_cards)
        {
            if (card->IsMouseClick(mousePos.x, mousePos.y))
            {
                card->StartDrag();
                m_draggedCard = card.get();
                break;
            }
        }
    }

    if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB))
    {
        if (m_draggedCard != nullptr)
        {
            m_draggedCard->EndDrag();
            m_draggedCard = nullptr;
        }
    }
}

PlantType CardManager::GetDraggedPlantType() const
{
    if (m_draggedCard != nullptr)
    {
        return m_draggedCard->GetPlantType();
    }

    return PlantType::NONE;
}


void CardManager::RefreshCard(int sun_count)
{
    for (auto& card : m_cards)
    {
        card->SetAffordable(sun_count >= card->GetCost());
    }
}



void CardManager::OnPlantPlaced()
{
    if (m_draggedCard != nullptr)
    {
        m_draggedCard->EndDrag();
        m_draggedCard = nullptr;
    }
}




void CardManager::CreateCard(PlantType type, int index, int cost, std::string imagePath, std::string dragimagePath)
{
    float x = CARD_START_X + index * CARD_WIDTH + CARD_SPACING;
    float y = LAWN_START_Y + 30;
    m_cards.push_back(std::make_unique<PlantCard>(type, x, y, cost, imagePath, dragimagePath));
    LOG_INFO("CardManager initialized for level {}, {}", x, y);
}


