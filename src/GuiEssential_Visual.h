#pragma once
#include "GuiEssential.h"

namespace ms
{
    class GuiEssential_Visual : public GuiEssential
    {
    public:
        GuiEssential_Visual() {}
        virtual ~GuiEssential_Visual() {}

        void setBackgroundColor(const sf::Color& v_color);
        void setHoverColor(const sf::Color& v_color);
        void setHoverTransparency(const sf::Uint8 v_power);

        void setBackgroundTexture(sf::Texture& v_texture);
        void setBackgroundTexture(sf::Texture& v_texture, sf::IntRect v_rect);

    protected:
        sf::Color m_BackgroundColor;
        bool m_HoverColorSet = false;
        sf::Color m_HoverColor;
        bool m_HoverTextureSet = false;
        
        sf::Texture* p_BackgroundTexture = nullptr;
        sf::RectangleShape m_HoverDrawable;
    };
};
