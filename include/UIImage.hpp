#ifndef UIIMAGE_H
#define UIIMAGE_H

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class UIIMAGE : public Util::GameObject {
public:
    UIIMAGE() = default;
    UIIMAGE(float x, float y, std::string png_name);
    ~UIIMAGE() override = default;

    void Update();
    void SetPosition(float x, float y);
};

#endif //UIIMAGE_H

