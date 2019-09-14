#pragma once
#include "GuiEssential_Visual.h"

namespace ms
{
    class ClickBase : public GuiEssential_Visual
    {
    public:
        ClickBase();
        ~ClickBase();

        virtual void update();
        virtual void checkEvents();

        virtual void setPosition(const sf::Vector2f& v_pos);
		virtual void setSize(const sf::Vector2f& v_size);
		virtual void setOrigin(const sf::Vector2f& v_origin);
		virtual void scale(const sf::Vector2f& v_scale);

        void setIcon(sf::Sprite& v_sprite);
        void setText(sf::Text& v_text);

        void selected(const bool v_set);			//Sets the button as active
        bool isHover() const { return m_Entered; }	//If the mouse is hovering over the button return true

    private:
        unsigned int m_TextSize;

    protected:
        bool m_Selected = false;

        bool m_TextSet = false;
        sf::Text* p_Text = nullptr;
        bool m_IconSet = false;
        sf::Sprite m_Icon;
        sf::Vector2f m_IconScale;

    protected:
        virtual void checkWithMouse() = 0; 
        void setup();
    };
};
