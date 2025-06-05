#include "WallNut.hpp"
#include "Util/Logger.hpp"

WallNut::WallNut(int grid_x, int grid_y, int mouse_x, int mouse_y, float health)
: Plant(grid_x, grid_y, mouse_x, mouse_y, health, RESOURCE_DIR"/plant/WallNut/zz") // High health
, m_max_health(4000.0f)
{
    LOG_INFO("WallNut planted at grid {}", grid_x);
}

void WallNut::Update()
{
    if (m_destroyed) {
        return;
    }

    Plant::Update();
    UpdateAppearance();
}

void WallNut::TakeDamage(float damage)
{
    Plant::TakeDamage(damage);
    UpdateAppearance();

    if (m_health <= 0) {
        LOG_INFO("WallNut destroyed!");
    }
}

void WallNut::UpdateAppearance()
{
    // Update sprite based on health percentage
    float health_ratio = m_health / m_max_health;

    // In a full implementation, you would change the sprite based on health
    // For example:
    // - > 66% health: normal appearance
    // - 33-66% health: cracked appearance
    // - < 33% health: heavily damaged appearance

    if (health_ratio > 0.66f) {
        // Use normal WallNut sprite
        // SetImage(RESOURCE_DIR"/plant/WallNut/WallNut.png");
    } else if (health_ratio > 0.33f) {
        // Use cracked WallNut sprite
        // SetImage(RESOURCE_DIR"/plant/WallNut/WallNut_cracked1.png");
    } else {
        // Use heavily damaged WallNut sprite
        // SetImage(RESOURCE_DIR"/plant/WallNut/WallNut_cracked2.png");
    }
}