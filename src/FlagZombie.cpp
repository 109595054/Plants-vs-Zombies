#include "FlagZombie.hpp"
#include "Game_Define.h"
#include "Util/Time.hpp"
#include "Util/Logger.hpp"

FlagZombie::FlagZombie(int grid_y, int y, float health, int monster_id, const std::vector<std::string> &paths, const std::vector<std::string> &attack_paths)
: Zombie(ZombieType::FLAG_ZOMBIE, grid_y, y, health, FLAG_ZOMBIE_SPEED, monster_id,  paths, attack_paths)
{

}
