#include "Globals.h"
#include "Button.h"
#include "Functional.h"
#include "Mouse.h"
#include <iostream>


//Public========================
//Constructors and Destructors--
ms::Button::Button(const sf::Vector2f& v_pos, const sf::Vector2f& v_size, bool* v_value) :
	p_Clicked(v_value)
{
	*p_Clicked = false;
	m_Position = sf::Vector2f(v_pos);
	m_Size = sf::Vector2f(v_size);
	m_Scale = sf::Vector2f(1, 1);
	setup();
}


ms::Button::Button(bool* v_Value) :
	p_Clicked(v_Value)
{
	*p_Clicked = false;
	m_Position = sf::Vector2f(0, 0);
	m_Size = sf::Vector2f(100, 50);
	m_Scale = sf::Vector2f(1, 1);
	setup();
}


ms::Button::~Button()
{}
//------------------------------


//Private=======================
//Drawing and view manipulation
void ms::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Box);
	target.draw(m_HoverDrawable);

	if(m_TextSet)
		target.draw(*p_Text);
	else if(m_IconSet)
		target.draw(m_Icon);
}
//------------------------------


//Real time events--------------
void ms::Button::checkWithMouse()
{
	//Check if the mouse is colliding with the button and if the left mouse is not held
	if((!m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos())) || m_Selected)
	{
		if(m_HoverColorSet)
			m_HoverDrawable.setFillColor(m_HoverColor);

		if(!Mouse::getPressed(sf::Mouse::Button::Left))
			m_Entered = true;
	}

	//Check if the mouse is colliding with the button
	else if(m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos()))
	{
		if (Mouse::getClick(sf::Mouse::Button::Left))
		{
			*p_Clicked = true;
		}
		else
		{
			*p_Clicked = false;
		}
	}
	//----------------------

	else
	{
		if(m_HoverColorSet)
			m_HoverDrawable.setFillColor(sf::Color::Transparent);

		m_Entered = false;
	}
}
//------------------------------
//==============================