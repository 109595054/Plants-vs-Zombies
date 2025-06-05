#ifndef UITEXT_H
#define UITEXT_H

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class UIText : public Util::GameObject {
public:
    UIText() = default;
    UIText(std::string font, std::string text_str, const int size, int x, int y);
    ~UIText() override = default;

    void Start();
    void Update();
    void SetStrText(std::string str) { m_text_str = str; }

private:
    std::string m_font;
    std::string m_text_str;
    int m_size;
    std::shared_ptr<Util::Text> m_text;
};

#endif //UITEXT_H
