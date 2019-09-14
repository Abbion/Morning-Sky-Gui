#include "ClickBase.h"
#include "Functional.h"
#include <math.h>


//Public========================
//Constructors and Destructors--
ms::ClickBase::ClickBase()
{}


ms::ClickBase::~ClickBase()
{}
//------------------------------


//Update and event check--------
void ms::ClickBase::update()
{
    checkWithMouse();
}


void ms::ClickBase::checkEvents()
{}
//------------------------------


//Setters-----------------------
void ms::ClickBase::setPosition(const sf::Vector2f& v_pos)
{
    m_Position = v_pos;
	m_Box.setPosition(m_Position);
    m_HoverDrawable.setPosition(m_Position);

	if(m_TextSet)
		p_Text->setPosition(fn::GetOrigin(m_Box));

    else if(m_IconSet)
        m_Icon.setPosition(fn::GetOrigin(m_Box));
}


void ms::ClickBase::setSize(const sf::Vector2f& v_size)
{
    m_Size = v_size;
	m_Box.setSize(m_Size);
    m_HoverDrawable.setSize(m_Size);
}


void ms::ClickBase::setOrigin(const sf::Vector2f& v_origin)
{
    m_Origin = v_origin;
    m_Box.setOrigin(m_Origin);
    m_HoverDrawable.setOrigin(m_Origin);
}


void ms::ClickBase::scale(const sf::Vector2f& v_scale)
{
    m_Scale = v_scale;
    //Scale position
    if(m_PositionScalable)
	{
		sf::Vector2f ScaledPosition(m_Position.x * m_Scale.x, m_Position.y * m_Scale.y);
		m_Box.setPosition(ScaledPosition);
        m_HoverDrawable.setPosition(ScaledPosition);
	}

    //Scale size
    sf::Vector2f ScaledSize(m_Size.x * m_Scale.x, m_Size.y * m_Scale.y);
    sf::Vector2f ScaledOriginPosition(m_Origin.x * m_Scale.x, m_Origin.y * m_Scale.y);

	m_Box.setSize(ScaledSize);
    m_HoverDrawable.setSize(ScaledSize);
    m_Box.setOrigin(ScaledOriginPosition);
    m_HoverDrawable.setOrigin(ScaledOriginPosition);

    //Scale text
    if(m_TextSet)
    {
        unsigned int ScaledTextSize = m_TextSize * ((m_Scale.x + m_Scale.y) / 2); 
		p_Text->setCharacterSize(ScaledTextSize);
		p_Text->setOrigin(fn::GetOriginOfOneLineText(*p_Text));
		p_Text->setPosition(fn::GetOrigin(m_Box));
    }

    //Scale icon
    else if(m_IconSet)
    {
	    float MinScale = std::min(m_Scale.x, m_Scale.y);
	    m_Icon.setScale(sf::Vector2f(MinScale * m_IconScale.x, MinScale * m_IconScale.y));
	    m_Icon.setOrigin(fn::GetOrigin(m_Icon));
	    m_Icon.setPosition(fn::GetOrigin(m_Box));
    }
}


void ms::ClickBase::setIcon(sf::Sprite& v_sprite)
{
    m_IconSet = true;
    m_TextSet = false;

    m_Icon = v_sprite;

	m_Icon.setOrigin(fn::GetOrigin(m_Icon));
	m_Icon.setPosition(fn::GetOrigin(m_Box));
    m_IconScale = m_Icon.getScale();
}


void ms::ClickBase::setText(sf::Text& v_text)
{
    m_TextSet = true;
    m_IconSet = false;

    p_Text = &v_text;
    p_Text->setOrigin(fn::GetOriginOfOneLineText(*p_Text));
    p_Text->setPosition(fn::GetOrigin(m_Box));
    m_TextSize = p_Text->getCharacterSize();
}


void ms::ClickBase::selected(const bool v_set)
{
    m_Selected = v_set;
}
//------------------------------


//Private=======================
//setup the gui component-------
void ms::ClickBase::setup()
{
    m_Box.setPosition(m_Position);
    m_Box.setSize(m_Size);
	m_BackgroundColor = sf::Color::Magenta;
    m_Box.setFillColor(m_BackgroundColor);

    m_HoverDrawable.setPosition(m_Position);
    m_HoverDrawable.setSize(m_Size);
    m_HoverDrawable.setFillColor(sf::Color::Transparent);
}
//------------------------------