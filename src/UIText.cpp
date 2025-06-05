#include "UIText.hpp"
#include "Util/Color.hpp"
#include "Util/Time.hpp"
#include "Util/TransformUtils.hpp"

UIText::UIText(std::string font, std::string text_str, const int size, int x, int y)
:m_font(font)
,m_text_str(text_str)
,m_size(size)
{
    m_Transform.translation.x = x;
    m_Transform.translation.y = y;
}


void UIText::Start() {
    m_text = std::make_unique<Util::Text>(m_font, m_size, "0",Util::Color::FromRGB(255, 255, 255));
    SetDrawable(m_text);
}

void UIText::Update()
{
    m_text->SetText(m_text_str);
    m_text->SetColor(Util::Color::FromName(Util::Colors::BLACK));
    Draw();
}

