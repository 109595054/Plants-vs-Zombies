#ifndef CHOMPER_HPP
#define CHOMPER_HPP

#include "Plant.hpp"

class Chomper : public Plant
{
public:
    Chomper(int grid_x, int grid_y, int mouse_x, int mouse_y, float health);
    void Update() override;

private:
    std::shared_ptr<Util::Animation> m_digesting_animation;
    bool m_is_digesting;     // 是否正在消化
    float m_digest_start_time; // 开始消化的时间
};

#endif //CHOMPER_HPP
