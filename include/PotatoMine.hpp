#ifndef POTATOMINE_HPP
#define POTATOMINE_HPP

#include "Plant.hpp"
#include "Util/Time.hpp"

static constexpr float POTATO_MINE_EXPLOSION_RADIUS = 50.0f;
static constexpr float POTATO_MINE_EXPLOSION_DAMAGE = 1800.0f;

class PotatoMine : public Plant
{
public:
    PotatoMine(int grid_x, int grid_y, int mouse_x, int mouse_y, float health);
    void Update() override;

    bool IsCanAttack() const override { return !m_destroyed && !m_exploded;}

private:
    std::shared_ptr<Util::Animation> m_armed_animation;
    std::shared_ptr<Util::Animation> m_exploded_animation;
    bool m_armed;
    float m_plant_time;    
    bool m_exploded;        
    float m_explosion_time;
};


#endif //POTATOMINE_HPP

