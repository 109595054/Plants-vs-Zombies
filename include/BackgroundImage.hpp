#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class BackgroundImage : public Util::GameObject {

public:
    enum class State {
        MENU,
        LEVEL
    };

    BackgroundImage() : GameObject(
            std::make_unique<Util::Image>(RESOURCE_DIR"/MainMenu.png"), -10) {
    }

    void UpdateBackground(State state) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        if (state == State::MENU) {
            temp->SetImage(RESOURCE_DIR"/MainMenu.png");
        } else if (state == State::LEVEL) {
            temp->SetImage(ImagePath());
        }
    }

    void Update() {
        Draw();
    }

private:
    inline std::string ImagePath() {
        //return RESOURCE_DIR"/Image/Background/level" + std::to_string(level) + ".png";
        return RESOURCE_DIR"/map0.jpg";
    }
};

#endif //BACKGROUND_IMAGE_HPP