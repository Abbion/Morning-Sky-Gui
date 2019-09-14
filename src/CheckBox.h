#pragma once
#include <SFML/Graphics.hpp>
#include "ClickBase.h"

namespace ms
{
    class CheckBox : public ClickBase, public sf::Drawable
    {
        public:
            CheckBox(const sf::Vector2f& v_pos, const sf::Vector2f& v_size, bool* v_value);
            CheckBox(bool* v_value);
            ~CheckBox();

            void setForegroundAlwaysVisible(const bool v_set);	//Enables or disables the icon or text visibility when the checkbox is et on false

        private:
            bool* p_Checked = nullptr;
            bool m_ForegroundAlwaysVisible = false;

        private:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
            virtual void checkWithMouse();
    };
};