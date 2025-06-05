#ifndef PLANTCARD_HPP
#define PLANTCARD_HPP

#include "UIImage.hpp"
#include "Util/Text.hpp"
#include "Util/Transform.hpp"

enum class PlantType;

class PlantCard
{
public:
    PlantCard(PlantType type, float x, float y, int cost, const std::string& imagePath, const std::string& dragimagePath);
    ~PlantCard() = default;

    void Update();
    void StartDrag();
    bool IsMouseClick(float mouse_x, float mouse_y) const;
    void EndDrag();

    PlantType GetPlantType() const { return m_plant_type; }
    int GetCost() const { return m_cost; }
    void SetAffordable(bool affordable) { m_is_affordable = affordable; }
    bool IsDragging() const { return m_is_dragging; }

private:
    PlantType m_plant_type;
    int m_cost;
    float m_original_x;
    float m_original_y;
    bool  m_is_dragging;
    bool  m_is_affordable;
    std::unique_ptr<UIIMAGE> m_cardImage;
    std::unique_ptr<UIIMAGE> m_dragImage;
};

#endif //PLANTCARD_HPP
