#ifndef BUCKETHEADZOMBIE_HPP
#define BUCKETHEADZOMBIE_HPP

#include "Zombie.hpp"

class BucketHeadZombie : public Zombie
{
public:
    BucketHeadZombie(int grid_y, float y, float health, const std::vector<std::string>& paths, const std::vector<std::string> &attack_paths);
    void TakeDamage(float damage) override;

private:
    bool m_bucket_destroyed;
    float m_bucket_health;
};

#endif //BUCKETHEADZOMBIE_HPP

