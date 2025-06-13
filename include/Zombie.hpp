#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "Game_Define.h"
#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

class Zombie : public Util::GameObject 
{
public:
    Zombie(ZombieType type, int grid_y, int y, float health, float speed, int monster_id, const std::vector<std::string> &paths, const std::vector<std::string> &attack_paths);
    virtual void Update();
    virtual void TakeDamage(float damage, bool is_die = false);

    void StartEating();
    void StopEating();

    void SlowDown();

    int GetGridy() const { return m_grid_y;}
    bool IsDestroyed() const { return m_destroyed; }
    bool HasReachedHouse() const { return m_reached_house; }
    bool IsEating() const { return m_eating; }
    float GetHealth() const { return m_health; }
    bool IsCanAttack() const;
    bool IsDie() const { return m_is_die;}
    int GetMonsterid() const {return m_monster_id;}
    
protected:
    std::shared_ptr<Util::Animation> m_animation;
    std::shared_ptr<Util::Animation> m_attack_animation;
    std::shared_ptr<Util::Animation> m_death_animation;
    ZombieType m_type;
    int m_grid_y;
    float m_health;
    float m_speed;
    bool m_destroyed;
    bool m_eating;
    bool m_reached_house;
    bool m_is_die;
    int  m_die_time;
    bool m_is_slow_down;
    int m_start_slow_down_time;
    int m_monster_id;
};

#endif
