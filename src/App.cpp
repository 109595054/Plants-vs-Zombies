#include "App.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/SFX.hpp"

App::App()
:m_CurrentState(State::START)
{

}


void App::Start() {
    LOG_TRACE("Start");
    m_back_ground.UpdateBackground(BackgroundImage::State::MENU);
    m_begin_game_image = std::make_unique<UIIMAGE>(250, 200, RESOURCE_DIR"/Adventure_0.png");
    m_CurrentState = State::UPDATE;
}

void App::Update()
{
    m_back_ground.Update();
    if (PVZGame::GetInstance().GetGameState() == GAME_STATUS_PLAYING || PVZGame::GetInstance().GetGameState() == GAME_STATUS_OVER
        || PVZGame::GetInstance().GetGameState() == GAME_STATUS_WIN)
    {
        PVZGame::GetInstance().Update();
    }
    else
    {
        m_back_ground.UpdateBackground(BackgroundImage::State::MENU);
        m_begin_game_image->Update();
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB))
        {
            auto sfx = Util::SFX(RESOURCE_DIR"/audio/Click.wav");
            sfx.Play();

            auto mousePos = Util::Input::GetCursorPosition();
            bool is_click = mousePos.x >= 9 && mousePos.x <= 490 &&  mousePos.y >= 132  && mousePos.y <= 293;
            LOG_INFO("App::Update {} {} {}", mousePos.x, mousePos.y, is_click);
            if (is_click) {
                PVZGame::GetInstance().Start(1);
                m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
            }
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_1))
        {
            PVZGame::GetInstance().Start(1);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_2))
        {
            PVZGame::GetInstance().Start(2);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_3))
        {
            PVZGame::GetInstance().Start(3);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_4)) {
            PVZGame::GetInstance().Start(4);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_5))
        {
            PVZGame::GetInstance().Start(5);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_6))
        {
            PVZGame::GetInstance().Start(6);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_7))
        {
            PVZGame::GetInstance().Start(7);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_8))
        {
            PVZGame::GetInstance().Start(8);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_9))
        {
            PVZGame::GetInstance().Start(9);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::NUM_0))
        {
            PVZGame::GetInstance().Start(10);
            m_back_ground.UpdateBackground(BackgroundImage::State::LEVEL);
        }
    }

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
