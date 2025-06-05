#ifndef WALLNUT_H
#define WALLNUT_H

#include "Plant.hpp"

class WallNut : public Plant
{
public:
    WallNut(int grid_x, int grid_y, int mouse_x, int mouse_y, float health);
    void Update() override;
    void TakeDamage(float damage) override;

private:
    void UpdateAppearance();
    float m_max_health;
};


#endif //WALLNUT_H
