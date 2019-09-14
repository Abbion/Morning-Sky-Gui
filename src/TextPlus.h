#pragma once
#include <SFML/Graphics.hpp>

namespace ms
{
    #define _MARGINRL 6 //Margin Left Right
    #define _MARGINTB 3 //Margin Top Bottom

    enum TextAlignment	//Ways the text can be aligned in the fRect
    {
        Top,
        Bottom,
        Left,
        Right,
        Middle
    };

    class TextPlus : public sf::Text
    {
    public:
        TextPlus(const sf::FloatRect& v_fRect);
        ~TextPlus();

        void setTextPosInRect(TextAlignment v_pos_x, TextAlignment v_pos_y);
        void updateInRectPos();			//Updates the text position. Should be used after any function that can change the text

        const float getTextTopLenght() const;		//Gets the double amout of the underline position. Used to aligh the text
        const float getLetterMaxHeight() const;		//Returns the maximu, height that the text can be
        sf::View getTextVew(const sf::RenderTarget& v_target) const;	//Gets the text view. Should be used as the target view before drawing

        const TextAlignment getTextAlignmentX() const { return m_PosX; }
        const TextAlignment getTextAlignmentY() const { return m_PosY; }

    private:
        const sf::FloatRect* p_fRect = nullptr;
        TextAlignment m_PosX = TextAlignment::Left;
        TextAlignment m_PosY = TextAlignment::Top;
        sf::Vector2f m_PosInRect;
    };
};