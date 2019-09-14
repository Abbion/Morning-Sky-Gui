#include "Globals.h"
#include "CheckBox.h"
#include "Functional.h"
#include "Mouse.h"
#include <iostream>
#include <math.h>


//Public========================
//Constructors and Destructors--
ms::CheckBox::CheckBox(const sf::Vector2f& v_pos, const sf::Vector2f& v_size, bool* v_value) :
	p_Checked(v_value)
{
	m_Position = sf::Vector2f(v_pos);
	m_Size = sf::Vector2f(v_size);
	m_Scale = sf::Vector2f(1, 1);
	setup();
}


ms::CheckBox::CheckBox(bool* v_value) :
	p_Checked(v_value)
{
	m_Position = sf::Vector2f(0, 0);
	m_Size = sf::Vector2f(25, 25);
	m_Scale = sf::Vector2f(1, 1);
	setup();
}


ms::CheckBox::~CheckBox()
{}
//------------------------------


//Setters-----------------------
void ms::CheckBox::setForegroundAlwaysVisible(const bool v_set)
{
	m_ForegroundAlwaysVisible = v_set;
}
//------------------------------


//Private=======================
//Drawing and view manipulation
void ms::CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Box);
	target.draw(m_HoverDrawable);

	if(m_TextSet &&  (*p_Checked || m_ForegroundAlwaysVisible))
		target.draw(*p_Text);
	else if(m_IconSet && (*p_Checked || m_ForegroundAlwaysVisible))
		target.draw(m_Icon);
}
//------------------------------

//Real time events--------------
void ms::CheckBox::checkWithMouse()
{
	//Check if the mouse is colliding with the button and if the left mouse is not held
	if((!m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos())) || m_Selected)
	{	
		if(m_HoverColorSet)
			m_HoverDrawable.setFillColor(m_HoverColor);

		if(!Mouse::getPressed(sf::Mouse::Button::Left))
			m_Entered = true;
	}

	//if the mouse entered, check if the user clicked
	else if(m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos()))
	{
		//If the button is clicked
		if (ms::Mouse::getClick(sf::Mouse::Button::Left))
		{
			if(!*p_Checked)
			{
				*p_Checked = true;
			}
			else
			{
				*p_Checked = false;
			}
		}
	}
	//----------------------

	//The mouse is not colliding with the box
	else
	{
		if(m_HoverColorSet)
			m_HoverDrawable.setFillColor(sf::Color::Transparent);

		m_Entered = false;
	}
}
//------------------------------
//==============================