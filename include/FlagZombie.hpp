#ifndef FLAGZOMBIE_HPP
#define FLAGZOMBIE_HPP

#include "Zombie.hpp"

class FlagZombie : public Zombie
{
public:
    FlagZombie(int grid_y, int y, float health, const std::vector<std::string> &paths, const std::vector<std::string> &attack_paths);
};

#endif //FLAGZOMBIE_HPP

