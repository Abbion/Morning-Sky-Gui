#include "Globals.h"
#include "TextPlus.h"
#include "Functional.h"

//Publiic=======================
//Constructors and Destructors--
ms::TextPlus::TextPlus(const sf::FloatRect& v_fRect) : 
    p_fRect(&v_fRect)
{
    
}

ms::TextPlus::~TextPlus()
{

}
//------------------------------


//Setters-----------------------
void ms::TextPlus::setTextPosInRect(TextAlignment v_pos_x, TextAlignment v_pos_y)
{
    if(v_pos_x == TextAlignment::Top || v_pos_x == TextAlignment::Bottom)
    {
        std::cout << "msGUI ERROR: Class: TextPlus, Function: setTextPosInRect() -> Text caannot be alligned to Top or Bottom on the x axis";
        return;
    }

    if(v_pos_y == TextAlignment::Left || v_pos_y == TextAlignment::Right)
    {
        std::cout << "msGUI ERROR: Class: TextPlus, Function: setTextPosInRect() -> Text caannot be alligned to Left or Right on the y axis";
        return;
    }

    m_PosX = v_pos_x;
    m_PosY = v_pos_y;
}
//------------------------------


//Update------------------------
void ms::TextPlus::updateInRectPos()
{
    if(!this->getString().isEmpty())
    {
        //Horizontal------------
        if(m_PosX == TextAlignment::Left)
            m_PosInRect.x = 0;
        else if(m_PosX == TextAlignment::Middle)
           m_PosInRect.x = (p_fRect->width / 2) - (getLocalBounds().width / 2);
        else if(m_PosX == TextAlignment::Right)
            m_PosInRect.x = p_fRect->width - getLocalBounds().width;

        if(m_PosInRect.x < 0)
            m_PosInRect.x = 0;
        //----------------------
        
        //Vertical--------------
        if(m_PosY == TextAlignment::Top)
            m_PosInRect.y = 0;
        else if(m_PosY == TextAlignment::Middle)
            m_PosInRect.y = (p_fRect->height / 2) - (getLetterMaxHeight() / 2) - getTextTopLenght();
        else if(m_PosY == TextAlignment::Bottom)
            m_PosInRect.y = p_fRect->height - getLocalBounds().height - getTextTopLenght();

        if(m_PosInRect.y < 0)
            m_PosInRect.y = 0;
        //----------------------
    }
}
//------------------------------


//Getters-----------------------
const float ms::TextPlus::getTextTopLenght() const
{
    return Text::getFont()->getUnderlinePosition(getCharacterSize()) * 2;
}


const float ms::TextPlus::getLetterMaxHeight() const
{
    return Text::getFont()->getGlyph(48, getCharacterSize(), true).bounds.height;
}


sf::View ms::TextPlus::getTextVew(const sf::RenderTarget& v_target) const
{
    sf::View TextView(sf::FloatRect(0, 0, p_fRect->width, p_fRect->height));
    float Cut_x = p_fRect->width / v_target.getSize().x;
    float Cut_y = p_fRect->height / v_target.getSize().y;  
    float Pos_x = (p_fRect->left + m_PosInRect.x) / v_target.getSize().x;
    float Pos_y = (p_fRect->top + m_PosInRect.y) / v_target.getSize().y;

    TextView.setViewport(sf::FloatRect(Pos_x, Pos_y, Cut_x, Cut_y));
	
	return TextView;
}
//------------------------------
//==============================