#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "BackgroundImage.hpp"
#include "PVZGame.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    App();
    void Start();
    void Update();
    void End(); // NOLINT(readability-convert-member-functions-to-static)
    State GetCurrentState() const { return m_CurrentState; }

private:
    State m_CurrentState;
    BackgroundImage m_back_ground;
    std::unique_ptr<UIIMAGE> m_begin_game_image;
};

#endif
