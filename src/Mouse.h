#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace ms
{
    class Mouse
    {
    public:
        Mouse();
        ~Mouse();

        static void update();

        static void setWindowPtr(const sf::RenderWindow& v_window);

        const static sf::Vector2i& getInWindowPos();
        const static sf::Vector2i& getGlobalPos();
        const static sf::Vector2i getDeltaPos();
        const static bool getClick(const sf::Mouse::Button v_button);
        const static bool getPressed(const sf::Mouse::Button v_button);

    private:
        static sf::Vector2i m_WindowPos;
        static sf::Vector2i m_GlobalPos;
        static sf::Vector2i m_DeltaPos;

        static bool m_DeltaHold_R;
        static bool m_Hold_R;

        static bool m_DeltaHold_L;
        static bool m_Hold_L;

        static bool m_DeltaHold_M;
        static bool m_Hold_M;

        const static sf::RenderWindow* p_Window;
    };
};