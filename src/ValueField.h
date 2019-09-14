#pragma once
#include "Globals.h"
#include "GuiEssential_Visual.h"
#include "Functional.h"
#include "Mouse.h"
#include "TextPlus.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include <iostream>
#include <typeinfo>
#include <limits>



namespace ms
{
	enum ValueType
	{
		T_None,
		T_Float,
		T_Double,
		T_Int8,
		T_Int16,
		T_Int32,
		T_Int64,
	};
	


	template<class T>
	class ValueField : public GuiEssential_Visual, public sf::Drawable
	{
	public:
		ValueField(const sf::Vector2f& v_pos, const float& v_width, const T* v_value);
		ValueField(const T* v_value);		
		~ValueField();

		virtual void update();
		virtual void checkEvents();

		virtual void setPosition(const sf::Vector2f& v_pos);
		virtual void setSize(const sf::Vector2f& v_size);
		virtual void setOrigin(const sf::Vector2f& v_origin);
		virtual void scale(const sf::Vector2f& v_scale);

		void setForegroundColor(const sf::Color& v_color);		//Changes the color of the text
		void setTextSize(const unsigned int v_size);
		void setFont(const sf::Font& v_font);
		void alignTo(TextAlignment v_align);
		void setSideMargins(int v_sideMargin);					//Changes the left and right margins

		void selected(const bool v_set);						//Sets the button as active
        const bool isHover() const { return m_Entered; }		//If the mouse is hovering over the button return true


		void setValueDragMultiplier(float v_multiplier)	{m_ValueDragMultiplier = v_multiplier;}	//Changes how fast the
		void editable(bool v_set) 						{m_IsEditable = v_set;}					//Eables or disables the possibility to edit the value

	private:
		T* p_Value = nullptr;
		ValueType m_ValueType = ValueType::T_None;

		TextPlus m_ValueOnScreen;
		sf::FloatRect m_ValueOnScreenBox;
		std::string m_ValueStr;
		unsigned int m_TextSize;
		const sf::Font* p_Font = nullptr;
		int m_SideMargins = _MARGINRL;

		bool m_Selected = false;
		bool m_SelectPositionReset = true;
		bool m_IsEditable = true;

		sf::RectangleShape m_Cursor;
		int m_CursorPos = 0;
		bool m_CursorVisible = true;
		bool m_CursorBlink = false;
		bool m_ResetTimer = false;

		float m_ValueDragMultiplier = 1;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void setup();

		void setValueTypeAndValuePointer(const T* v_value);

		void checkWithMouse();
		void readText();
		void dragValue();
		
		void updateValue();
		void slideValue();
		void correctFloatsAfterRead();
		void clearZerosAfterRead();
		void correctFloatsAfterUpdate();	
		void applyValue();

		void updateCursorPos();
		void blinkingCursor();
	};
};



//Definitions############################################################################
//Public========================
//Constructors and Destructors--
template<class T>
ms::ValueField<T>::ValueField(const sf::Vector2f& v_pos, const float& v_width, const T* v_value) :
	m_ValueOnScreen(m_ValueOnScreenBox)
{
	m_Position = sf::Vector2f(v_pos);
	m_Size = sf::Vector2f(v_width, Globals::G_FontSizeAtStart + (_MARGINTB * 2));
	m_Scale = sf::Vector2f(1, 1);
	m_Origin = sf::Vector2f(0, 0);
	//Set the ValueField type and pass the pointer to the value
    setValueTypeAndValuePointer(v_value);
	//Setup the component
	setup();
}


template<class T>
ms::ValueField<T>::ValueField(const T* v_value) : 
	m_ValueOnScreen(m_ValueOnScreenBox)
{
	m_Position = sf::Vector2f(0, 0);
	m_Size = sf::Vector2f(100, Globals::G_FontSizeAtStart + (_MARGINTB * 2));
	m_Scale = sf::Vector2f(1, 1);
	//Set the ValueField type and pass the pointer to the value
    setValueTypeAndValuePointer(v_value);
	//Setup the component
	setup();
}


template<class T>
ms::ValueField<T>::~ValueField()
{ }
//------------------------------



//Update and event check--------
template<class T>
void ms::ValueField<T>::update()
{
	//Mouse and cursor
	checkWithMouse();

    if(m_Selected)
	{
		m_CursorVisible = true;
		dragValue();
		blinkingCursor();
	}

    else
	{
		m_CursorVisible = false;
		updateValue();
	}
}


template<class T>
void ms::ValueField<T>::checkEvents()
{
	readText();
}
//------------------------------



//Setters-----------------------
//Transformations---------------
template<class T>
void ms::ValueField<T>::setPosition(const sf::Vector2f& v_pos)
{
	m_Position = v_pos;
	m_Box.setPosition(m_Position);
    m_HoverDrawable.setPosition(m_Position);
	m_ValueOnScreenBox.left = m_Position.x + (m_SideMargins / 2) - m_Origin.x;
	m_ValueOnScreenBox.top = m_Position.y - m_Origin.y;
	m_ValueOnScreen.updateInRectPos();
}


template<class T>
void ms::ValueField<T>::setSize(const sf::Vector2f& v_size)
{
	m_Size = v_size;
	m_Box.setSize(m_Size);
	m_HoverDrawable.setSize(m_Size);
	m_ValueOnScreenBox.width = m_Size.x - m_SideMargins;
	m_ValueOnScreenBox.height = m_Size.y;
	m_ValueOnScreen.updateInRectPos();
}


template<class T>
void ms::ValueField<T>::setOrigin(const sf::Vector2f& v_origin)
{
	m_Origin = v_origin;
	m_Box.setOrigin(m_Origin);
	m_HoverDrawable.setOrigin(m_Origin);

	m_ValueOnScreenBox.left = m_Position.x + (m_SideMargins / 2) - m_Origin.x;
	m_ValueOnScreenBox.top = m_Position.y - m_Origin.y;
	m_ValueOnScreen.updateInRectPos();
}


template<class T>
void ms::ValueField<T>::scale(const sf::Vector2f& v_scale)
{
	m_Scale = v_scale;

	//Scale origin
	sf::Vector2f ScaledOriginPosition(m_Origin.x * m_Scale.x, m_Origin.y * m_Scale.y);

	//Scale position
	if(m_PositionScalable)
	{
		sf::Vector2f ScaledPosition(m_Position.x * m_Scale.x, m_Position.y * m_Scale.y);
		m_Box.setPosition(ScaledPosition);
		m_HoverDrawable.setPosition(ScaledPosition);

		m_ValueOnScreenBox.left = ScaledPosition.x + (m_SideMargins / 2 * m_Scale.x) - ScaledOriginPosition.x;
		m_ValueOnScreenBox.top = ScaledPosition.y - ScaledOriginPosition.y;
	}

	//Scale size	
    sf::Vector2f ScaledSize(m_Size.x * m_Scale.x, m_Size.y * m_Scale.y);


	m_Box.setSize(ScaledSize);
    m_HoverDrawable.setSize(ScaledSize);
    m_Box.setOrigin(ScaledOriginPosition);
    m_HoverDrawable.setOrigin(ScaledOriginPosition);

	//Scale text
	unsigned int ScaledTextSize = m_TextSize * ((m_Scale.x + m_Scale.y) / 2); 
	m_ValueOnScreen.setCharacterSize(ScaledTextSize);

	m_ValueOnScreenBox.width = ScaledSize.x - m_SideMargins * m_Scale.x;
	m_ValueOnScreenBox.height = ScaledSize.y;
	m_ValueOnScreen.updateInRectPos();

	//Scale cursor
	m_Cursor.setSize(sf::Vector2f(1, m_ValueOnScreen.getLetterMaxHeight() + 1));

	//Correct value position after scale if postion is not scaled
	if(!m_PositionScalable)
	{
		sf::Vector2f TopLeftCorner = fn::GetTLCorner(m_Box);
		m_ValueOnScreenBox.left = TopLeftCorner.x + (m_SideMargins / 2 * m_Scale.x);
		m_ValueOnScreenBox.top = TopLeftCorner.y;
	}
}
//------------------------------


//Foreground Visuals------------
template<class T>
void ms::ValueField<T>::setForegroundColor(const sf::Color& v_color)
{
	m_ValueOnScreen.setFillColor(v_color);
	m_Cursor.setFillColor(v_color);
}


template<class T>
void ms::ValueField<T>::setTextSize(const unsigned int v_size)
{
	m_TextSize = v_size;
	m_ValueOnScreen.setCharacterSize(m_TextSize);
	m_Cursor.setSize(sf::Vector2f(1, m_ValueOnScreen.getLetterMaxHeight() + 1));
	m_ValueOnScreen.updateInRectPos();
}


template<class T>
void ms::ValueField<T>::setFont(const sf::Font& v_font)
{
	p_Font = &v_font;
	m_ValueOnScreen.setFont(*p_Font);
}


//Align the text in box---------
template<class T>
void ms::ValueField<T>::alignTo(TextAlignment v_align)
{
	if(v_align == TextAlignment::Left)
		m_ValueOnScreen.setTextPosInRect(TextAlignment::Left, TextAlignment::Middle);
	else if(v_align == TextAlignment::Middle)
		m_ValueOnScreen.setTextPosInRect(TextAlignment::Middle, TextAlignment::Middle);
	else
	{
		std::cout << "msGUI ERROR: Class: ValueField, Function: alignTo() -> You can use only Left or Middle\n";
		return;
	}

	m_ValueOnScreen.updateInRectPos();
}
//------------------------------

//Set margins on left and right border
template<class T>
void ms::ValueField<T>::setSideMargins(int v_sideMargin)
{
	m_SideMargins = v_sideMargin;
	m_ValueOnScreenBox.left = m_ValueOnScreenBox.left + (v_sideMargin / 2);
	m_ValueOnScreenBox.width = m_ValueOnScreenBox.width - v_sideMargin;
	m_ValueOnScreen.updateInRectPos();
}
//------------------------------


template<class T>
void ms::ValueField<T>::selected(const bool v_set)
{
	m_Selected = v_set;
	if(m_SelectPositionReset)
	{
		m_CursorPos = m_ValueStr.size();
		m_SelectPositionReset = false;
	}
	updateCursorPos();
}
//------------------------------
//==============================


//Private=======================
//Drawing and view manipulation-
template<class T>
void ms::ValueField<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(Globals::G_RresizedView);
		target.draw(m_Box);
		target.draw(m_HoverDrawable);

		target.setView(m_ValueOnScreen.getTextVew(target));
			target.draw(m_ValueOnScreen);

			if(m_CursorVisible && !m_CursorBlink)
				target.draw(m_Cursor);

	target.setView(Globals::G_RresizedView);
}
//------------------------------


//setup the gui component-------
template<class T>
void ms::ValueField<T>::setup()
{
	//Setup the box
    m_Box.setPosition(m_Position);
    m_Box.setSize(m_Size);
    m_Box.setFillColor(sf::Color::Magenta);
	//------------------

	//Setup the HoverRect
	m_HoverDrawable.setPosition(m_Position);
    m_HoverDrawable.setSize(m_Size);
    m_HoverDrawable.setFillColor(sf::Color::Transparent);
	//------------------

	//Setup the TextBox
	m_ValueOnScreenBox.top = m_Position.y;
	m_ValueOnScreenBox.left = m_Position.x + (m_SideMargins / 2);
	m_ValueOnScreenBox.width = m_Size.x - m_SideMargins;
	m_ValueOnScreenBox.height = m_Size.y;
	//------------------

	//Clear the string and update it
	m_ValueStr.clear();
	updateValue();
	//------------------

	//Setup the text that is visible on the screen
	m_TextSize = Globals::G_FontSizeAtStart;
	p_Font = Globals::G_MainFontPtr;
	m_ValueOnScreen.setFont(*p_Font);
	m_ValueOnScreen.setCharacterSize(m_TextSize);
	m_ValueOnScreen.setFillColor(sf::Color::White);
	//------------------

	//Setup the visible value
	m_ValueOnScreen.setString(m_ValueStr);
	m_ValueOnScreen.setTextPosInRect(TextAlignment::Left, TextAlignment::Middle);
	m_ValueOnScreen.updateInRectPos();
	//------------------

	//Setup the cursor
	m_Cursor.setSize(sf::Vector2f(1, m_ValueOnScreen.getLetterMaxHeight() + 1));
	m_Cursor.setFillColor(sf::Color::White);
	//------------------
}
//------------------------------


//Set the data type and the value pointer-----
template<class T>
void ms::ValueField<T>::setValueTypeAndValuePointer(const T* v_value)
{
	if (typeid(T) == typeid(float))				m_ValueType = ValueType::T_Float;
	else if (typeid(T) == typeid(double))		m_ValueType = ValueType::T_Double;
	else if (typeid(T) == typeid(char))			m_ValueType = ValueType::T_Int8;
	else if (typeid(T) == typeid(short))		m_ValueType = ValueType::T_Int16;
	else if (typeid(T) == typeid(int))			m_ValueType = ValueType::T_Int32;
	else if (typeid(T) == typeid(long long))	m_ValueType = ValueType::T_Int64;
	else
	{
		std::cout << "msGUI ERROR: Class: ValueField, Function: setValueTypeAndValuePointer() -> Can't use " << typeid(T).name() << " value type.\n";
		return;
	}
	p_Value = const_cast<T*>(v_value);
}
//------------------------------


//Real time events and normal events
template<class T>
void ms::ValueField<T>::checkWithMouse()
{
	if(m_IsEditable)
	{
		if(!m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos()))
		{
			if(m_HoverColorSet)
				m_HoverDrawable.setFillColor(m_HoverColor);

			if(!Mouse::getPressed(sf::Mouse::Button::Left))
				m_Entered = true;
		}

		//Check if the mouse is colliding with the button
		else if(m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos()))
		{
			//If button is clicked
			if(Mouse::getClick(sf::Mouse::Button::Left))
			{
				m_Selected = true;
				m_ResetTimer = true;
				m_CursorPos = m_ValueStr.size();
				updateCursorPos();
			}
		}
		//----------------------

		else if(m_Entered && Mouse::getClick(sf::Mouse::Button::Left))
		{
			applyValue();
			m_ValueOnScreen.setPosition(0, 0);
			m_ValueOnScreen.setString(m_ValueStr);
			m_Selected = false;
			m_Entered = false;
			m_SelectPositionReset = true;
			m_ValueOnScreen.updateInRectPos();
		}

		else if(!m_Selected)
		{
			if(m_HoverColorSet)
				m_HoverDrawable.setFillColor(sf::Color::Transparent);
			m_Entered = false;
		}
	}
}
//------------------------------


//Read the text and handle events
template<class T>
void ms::ValueField<T>::readText()
{
	if(m_IsEditable)
	{
		if(m_Selected)
		{
			//Read text-------------
			if(Globals::G_Event->type == sf::Event::TextEntered)
			{
				if(Globals::G_Event->text.unicode < 58 && Globals::G_Event->text.unicode > 43 && Globals::G_Event->text.unicode != 47)
				{
					m_ValueStr.resize(m_ValueStr.size());
					m_ValueStr.insert(m_ValueStr.begin() + m_CursorPos, (char)Globals::G_Event->text.unicode);
					m_CursorPos++;
					m_ResetTimer = true;
				}
			}

			//Other events----------
			if(Globals::G_Event->type == sf::Event::KeyPressed)
			{
				//Apply value
				if(Globals::G_Event->key.code == sf::Keyboard::Enter)
				{
					applyValue();
					m_ValueOnScreen.setPosition(0, 0);
					m_Selected = false;
					m_SelectPositionReset = true;
				}
				//Delete the last character
				else if(Globals::G_Event->key.code == sf::Keyboard::Backspace)
				{
					if(m_CursorPos > 0)
					{
						m_ValueStr.erase(m_ValueStr.begin() + m_CursorPos - 1);
						m_CursorPos--;
						m_ResetTimer = true;
					}
				}
				//Move the cursor to the left
				else if(Globals::G_Event->key.code == sf::Keyboard::Left)
				{
					if(m_CursorPos > 0)
					{
						m_CursorPos--;
						m_ResetTimer = true;
					}
				}
				//Move the cursor to the right
				else if (Globals::G_Event->key.code == sf::Keyboard::Right)
				{
					if(m_CursorPos < m_ValueStr.size())
					{
						m_CursorPos++;
						m_ResetTimer = true;
					}
				}
				//Delete the whole value
				else if(Globals::G_Event->key.code == sf::Keyboard::Delete)
				{
					m_ValueStr.clear();
					m_CursorPos = 0;
					m_ResetTimer = true;
				}
			}

			//Update the changes
			m_ValueOnScreen.setString(m_ValueStr);
			updateCursorPos();
			slideValue();
			m_ValueOnScreen.updateInRectPos();
		}
	}
}
//------------------------------


//Gives a possibility to change the value by moving the mouse while holding left control 
template<class T>
void ms::ValueField<T>::dragValue()
{
	if(m_IsEditable)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && m_Selected)
		{
			*p_Value += (T)Mouse::getDeltaPos().x * m_ValueDragMultiplier;
			updateValue();
			m_CursorVisible = false;
			m_CursorPos = m_ValueStr.length();
		}
	}
}
//------------------------------


//Text Check Functions----------
//Convert the value to a string visible on the screen
template<class T>
void ms::ValueField<T>::updateValue()
{
	m_ValueStr = std::to_string(*p_Value);
	if(m_ValueType == ValueType::T_Float || m_ValueType == ValueType::T_Double)
		correctFloatsAfterUpdate();
	m_ValueOnScreen.setString(m_ValueStr);
}
//------------------------------


//Slides the value if it gets out of the border
template<class T>
void ms::ValueField<T>::slideValue()
{
	//If the text is out of the box scroll it
	//To the right
	if(m_Cursor.getPosition().x > m_ValueOnScreenBox.width)
	{
		float moveBy = fn::GetTRCorner(m_Cursor).x - m_ValueOnScreenBox.width;
		m_ValueOnScreen.move(-moveBy ,0);
	}

	//To the left
	else if(m_Cursor.getPosition().x < 0)
	{
		float moveBy = fn::GetTLCorner(m_Cursor).x;
		m_ValueOnScreen.move(-moveBy ,0);
	}
}
//------------------------------


//Corrects the float value after it was written
template<class T>
void ms::ValueField<T>::correctFloatsAfterRead()
{
	//If there is no text
	if(m_ValueStr.empty())
	{
		m_ValueStr = "0.0";
	}

	else
	{
		//Find the first dot
		unsigned int DotPos = 0;
		DotPos = (m_ValueStr.find_first_of('.') < m_ValueStr.find_first_of(',') ? m_ValueStr.find_first_of('.') : m_ValueStr.find_first_of(','));
		
		//If there are no dots
		if(DotPos == std::string::npos)
		{
			m_ValueStr += ".0";
			clearZerosAfterRead();
		}

		else
		{
			//erase all dots
			for(int i = 0; i < m_ValueStr.size(); i++)
			{
				if(m_ValueStr.at(i) == '.' || m_ValueStr.at(i) == ',')
				{
					m_ValueStr.erase(m_ValueStr.begin() + i);
					i--;
				}
			}

			//add the dot
			m_ValueStr.resize(m_ValueStr.size());
			m_ValueStr.insert(m_ValueStr.begin() + DotPos, '.');

			if(DotPos + 6 < m_ValueStr.size() - 1)
				m_ValueStr.resize(DotPos + 6);

			clearZerosAfterRead();
		}

		//Count how many numbers after the dot
		int NumbersAfterDot = m_ValueStr.length() - DotPos - 1;
		if(NumbersAfterDot > 4)
		{
			for(int i = 0; i < NumbersAfterDot - 4; i++)
				m_ValueStr.pop_back();
		}
	}
}
//------------------------------


//Clear excessive zeros-----------
template<class T>
void ms::ValueField<T>::clearZerosAfterRead()
{
	unsigned int StartPoint = 0;
	//If the value is a negative value
	if(m_ValueStr.at(0) == '-')
		StartPoint = 1;

	//Clears the zeros from the front of the value
	for (int i = StartPoint; i < m_ValueStr.size(); i++)
	{
		if(m_ValueStr.at(i) == '0')
		{
			m_ValueStr.erase(m_ValueStr.begin() + i);
			i--;
		}
		
		else
			break;
	}

	//If there is a dot at the start
	if (m_ValueStr.at(StartPoint) == '.')
	{
		m_ValueStr.resize(m_ValueStr.size());
		m_ValueStr.insert(m_ValueStr.begin() + StartPoint, '0');
	}

	//Clears the zeros from the back of the value
	for(int i = m_ValueStr.size() - 1; i > StartPoint; i--)
	{
		if(m_ValueStr.at(i) == '0')
			m_ValueStr.pop_back();

		else if(m_ValueStr.at(i) == '.')
		{
			m_ValueStr.push_back('0');
			break;
		}

		else
			break;
	}

	//Clears the excessive minus symbols
	for (int i = StartPoint; i < m_ValueStr.size(); i++)
	{
		if(m_ValueStr.at(i) == '-')
		{
			m_ValueStr.erase(m_ValueStr.begin() + i);
			i--;
		}
	}
}
//------------------------------


//Correct the float value after it was updated by the original object/value
template<class T>
void ms::ValueField<T>::correctFloatsAfterUpdate()
{
	unsigned int DotPos = m_ValueStr.find_first_of('.');
	int AfterDot = m_ValueStr.length() - DotPos - 1;
	while(AfterDot > 4)
	{
		m_ValueStr.pop_back();
		AfterDot--;
	}

	while(m_ValueStr.at(m_ValueStr.length() - 1) == '0')
		m_ValueStr.pop_back();
	
	if(m_ValueStr.length() - 1 == DotPos)
		m_ValueStr.push_back('0');
}
//------------------------------


//Check the value if its properly written and correct if it's not
//Check what data type is used and how should it be written
template<class T>
void ms::ValueField<T>::applyValue()
{
	//OVERFLOW COMMUNICATE
	if(m_ValueType == ValueType::T_Float)
	{
		correctFloatsAfterRead();
		float Corrected = atof(m_ValueStr.c_str());
		*p_Value = Corrected;

		if(Corrected > 100000 || Corrected < -100000)
			std::cout << "msGUI ERROR: Class: ValueField, Function: applyValue() -> Due to floating-point type precision limits. It is recomended to use values between -1000000 to 1000000\n";
	}

	else if(m_ValueType == ValueType::T_Double)
	{
		correctFloatsAfterRead();
		double Corrected = std::stod(m_ValueStr);
		*p_Value = Corrected;

		if(Corrected > 100000000000 || Corrected < -100000000000)
			std::cout << "msGUI ERROR: Class: ValueField, Function: applyValue() -> Due to double type precision limits. It is recomended to use values between -1000000000000 to 1000000000000\n";

	}

	else if(m_ValueType == ValueType::T_Int8)
	{
		int real = atoi(m_ValueStr.c_str());

		if(real < std::numeric_limits<char>::min() || real > std::numeric_limits<char>::max())
		{
			std::cout <<  "msGUI ERROR: Class: ValueField, Function: applyValue() -> Your value overflows! Value range " << (int)std::numeric_limits<char>::min()
					  << " to " <<  (int)std::numeric_limits<char>::max() << std::endl;

			int reset;
			if(real > 0)
				reset = (int)std::numeric_limits<char>::max();
			else
				reset = (int)std::numeric_limits<char>::min();
			
			*p_Value = reset;
			m_ValueStr = std::to_string(reset);
		}

		else
			*p_Value = real;
	}

	else if(m_ValueType == ValueType::T_Int16)
	{
		int real = atoi(m_ValueStr.c_str());

		if(real < std::numeric_limits<short>::min() || real > std::numeric_limits<short>::max())
		{
			std::cout <<  "msGUI ERROR: Class: ValueField, Function: applyValue() -> Your value overflows! Value range" << std::numeric_limits<short>::min()
					  << " to " <<  std::numeric_limits<short>::max() << std::endl;
			short reset = 0;
			*p_Value = reset;
		}

		else
			*p_Value = real;
	}	

	else if(m_ValueType == ValueType::T_Int32)
	{
		long long real = atoll(m_ValueStr.c_str());
		int Corrected = atoi(m_ValueStr.c_str());
		m_ValueStr = std::to_string(Corrected);
		*p_Value = Corrected;

		if(real < std::numeric_limits<int>::min() || real > std::numeric_limits<int>::max())
		{
			std::cout <<  "msGUI ERROR: Class: ValueField, Function: applyValue() -> Your value overflows! Value range" << std::numeric_limits<int>::min()
					  << " to " <<  std::numeric_limits<int>::max() << std::endl;
		}
	}

	else if(m_ValueType == ValueType::T_Int64)
	{
		long long real = atoll(m_ValueStr.c_str());
		m_ValueStr = std::to_string(real);
		*p_Value = real;

		if(real < std::numeric_limits<long long>::min() || real > std::numeric_limits<long long>::max())
		{
			std::cout <<  "msGUI ERROR: Class: ValueField, Function: applyValue() -> Your value overflows! Value range" << std::numeric_limits<long long>::min()
					  << " to " <<  std::numeric_limits<long long>::max() << std::endl;
		}
	}	
}
//------------------------------


//Cursor Visuals---------------
//Update cursor position------
template<class T>
void ms::ValueField<T>::updateCursorPos()
{
	m_Cursor.setPosition(m_ValueOnScreen.findCharacterPos(m_CursorPos) + sf::Vector2f(0, m_ValueOnScreen.getTextTopLenght() - 1));
}
//------------------------------


//Allows the cursor to blink
template<class T>
void ms::ValueField<T>::blinkingCursor()
{
	if(m_ResetTimer)
	{
		Globals::G_BlinkingClock.restart();
		m_ResetTimer = false;
		m_CursorBlink = false;
	}

	else
	{
		if(Globals::G_BlinkingClock.getElapsedTime().asMilliseconds() > Globals::G_BlinkinkOffset.asMilliseconds())
		{
			m_CursorBlink = !m_CursorBlink;
			Globals::G_BlinkingClock.restart();
		}
	}
}
//------------------------------
//==============================