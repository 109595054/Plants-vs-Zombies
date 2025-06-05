#ifndef CARDMANAGER_H
#define CARDMANAGER_H

#include "Game_Define.h"
#include "PlantCard.hpp"

class CardManager
{
public:
    CardManager();
    ~CardManager() = default;

    void Update();
    void InitCard(int level);
    void OnMouseLBPressed(glm::vec2 &mouse_pos);
    void OnMouseLBUp();
    void HandleMouseInput();
    PlantType GetDraggedPlantType() const;
    void RefreshCard(int sun_count);
    void OnPlantPlaced();
    bool HasDraggedPlant() const { return m_draggedCard != nullptr; }

private:
    void CreateCard(PlantType type, int index, int cost, std::string imagePath, std::string dragimagePath);

    std::vector<std::unique_ptr<PlantCard>> m_cards;
    PlantCard* m_draggedCard;
};


#endif //CARDMANAGER_H
