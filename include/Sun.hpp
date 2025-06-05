#ifndef SUM_HPP
#define SUM_HPP

#include "Util/GameObject.hpp"
#include "Util/Animation.hpp"

class Sun : public Util::GameObject {
public:
    Sun(float x, float y, bool is_can_auto_collected);
    virtual void Update();

    bool IsCollected() const { return m_collected; }
    bool IsDestroyed() const { return m_destroyed; }

private:
    std::shared_ptr<Util::Animation> m_animation;
    bool m_collected;
    float m_life_time;
    bool m_is_can_auto_collected;
    bool m_destroyed;
};

#endif
