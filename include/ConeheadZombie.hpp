#ifndef CONEHEADZOMBIE_HPP
#define CONEHEADZOMBIE_HPP

#include "Zombie.hpp"

class ConeheadZombie : public Zombie {
public:
    ConeheadZombie(int grid_y, float y, float health, int monster_id, const std::vector<std::string>& paths, const std::vector<std::string> &attack_paths);
    void TakeDamage(float damage, bool is_die = false) override;

private:
    bool m_cone_destroyed;
    float m_cone_health;
};

#endif //CONEHEADZOMBIE_HPP
