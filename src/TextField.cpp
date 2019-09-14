#include "TextField.h"
#include "Functional.h"
#include "Mouse.h"

//Public========================
//Constructors and Destructors--
ms::TextField::TextField(const sf::Vector2f& v_pos, const float& v_width) :
    m_StringOnScreen(m_StringOnScreenBox)
{
    m_Position = sf::Vector2f(v_pos);
    m_Size = sf::Vector2f(v_width, Globals::G_FontSizeAtStart + (_MARGINTB * 2));
    m_Scale = sf::Vector2f(1, 1);
	m_Origin = sf::Vector2f(0, 0);
    //Setup the component
	setup();
}

ms::TextField::TextField() :
    m_StringOnScreen(m_StringOnScreenBox)
{
    m_Position = sf::Vector2f(0, 0);
	m_Size = sf::Vector2f(100, Globals::G_FontSizeAtStart + (_MARGINTB * 2));
	m_Scale = sf::Vector2f(1, 1);
	//Setup the component
	setup();
}

ms::TextField::~TextField()
{ }
//------------------------------



//Update and event check--------
void ms::TextField::update()
{
    //Mouse and cursor
	checkWithMouse();

    if(m_Selected)
	{
		m_CursorVisible = true;
		blinkingCursor();
	}

	else
	{
		m_CursorVisible = false;
	}
}

void ms::TextField::checkEvents()
{
    readText();
}
//------------------------------



//Setters-----------------------
//Transformations---------------
void ms::TextField::setPosition(const sf::Vector2f& v_pos)
{
    m_Position = v_pos;
	m_Box.setPosition(m_Position);
    m_HoverDrawable.setPosition(m_Position);
	m_StringOnScreenBox.left = m_Position.x + (m_SideMargins / 2) - m_Origin.x;
	m_StringOnScreenBox.top = m_Position.y - m_Origin.y;
	m_StringOnScreen.updateInRectPos();
}

void ms::TextField::setSize(const sf::Vector2f& v_size)
{
    m_Size = v_size;
	m_Box.setSize(m_Size);
	m_HoverDrawable.setSize(m_Size);
	m_StringOnScreenBox.width = m_Size.x - m_SideMargins;
	m_StringOnScreenBox.height = m_Size.y;
	m_StringOnScreen.updateInRectPos();
	m_RecalculateCursorPos = true;
}

void ms::TextField::setOrigin(const sf::Vector2f& v_origin)
{
    m_Origin = v_origin;
	m_Box.setOrigin(m_Origin);
	m_HoverDrawable.setOrigin(m_Origin);

	m_StringOnScreenBox.left = m_Position.x + (m_SideMargins / 2) - m_Origin.x;
	m_StringOnScreenBox.top = m_Position.y - m_Origin.y;
	m_StringOnScreen.updateInRectPos();
}

void ms::TextField::scale(const sf::Vector2f& v_scale)
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

		m_StringOnScreenBox.left = ScaledPosition.x + (m_SideMargins / 2 * m_Scale.x) - ScaledOriginPosition.x;
		m_StringOnScreenBox.top = ScaledPosition.y - ScaledOriginPosition.y;
	}

	//Scale size
    sf::Vector2f ScaledSize(m_Size.x * m_Scale.x, m_Size.y * m_Scale.y);


	m_Box.setSize(ScaledSize);
    m_HoverDrawable.setSize(ScaledSize);
    m_Box.setOrigin(ScaledOriginPosition);
    m_HoverDrawable.setOrigin(ScaledOriginPosition);

	//Scale text
	unsigned int ScaledTextSize = m_TextSize * ((m_Scale.x + m_Scale.y) / 2); 
	m_StringOnScreen.setCharacterSize(ScaledTextSize);

	m_StringOnScreenBox.width = ScaledSize.x - m_SideMargins * m_Scale.x;
	m_StringOnScreenBox.height = ScaledSize.y;
	m_StringOnScreen.updateInRectPos();

	//Scale cursor
	m_Cursor.setSize(sf::Vector2f(1, m_StringOnScreen.getLetterMaxHeight() + 1));

	//Correct value position after scale if position is not scaled
	if(!m_PositionScalable)
	{
		sf::Vector2f TopLeftCorner = fn::GetTLCorner(m_Box);
		m_StringOnScreenBox.left = TopLeftCorner.x + (m_SideMargins / 2 * m_Scale.x);
		m_StringOnScreenBox.top = TopLeftCorner.y;
	}
}
//------------------------------


//Foreground Visuals------------
void ms::TextField::setForegroundColor(const sf::Color& v_color)
{
    m_StringOnScreen.setFillColor(v_color);
	m_Cursor.setFillColor(v_color);
}


void ms::TextField::setTextSize(const unsigned int v_size)
{
    m_TextSize = v_size;
	m_StringOnScreen.setCharacterSize(m_TextSize);
	m_Cursor.setSize(sf::Vector2f(1, m_StringOnScreen.getLetterMaxHeight() + 1));
	m_StringOnScreen.updateInRectPos();
}


void ms::TextField::setFont(const sf::Font& v_font)
{
    p_Font = &v_font;
	m_StringOnScreen.setFont(*p_Font);
}
//------------------------------


//Align the text in the box-----
void ms::TextField::alignTo(TextAlignment v_align)
{
    if(v_align == TextAlignment::Left)
		m_StringOnScreen.setTextPosInRect(TextAlignment::Left, TextAlignment::Middle);
	else if(v_align == TextAlignment::Middle)
		m_StringOnScreen.setTextPosInRect(TextAlignment::Middle, TextAlignment::Middle);
	else
	{
		std::cout << "msGUI ERROR: Class: TextField, Function: alignTo() -> You can use only Left or Middle\n";
		return;
	}

	m_StringOnScreen.updateInRectPos();
}
//------------------------------

//Set margins on left and right border
void ms::TextField::setSideMargins(int v_sideMargin)
{
	m_SideMargins = v_sideMargin;
	m_StringOnScreenBox.left = m_StringOnScreenBox.left + (v_sideMargin / 2);
	m_StringOnScreenBox.width = m_StringOnScreenBox.width - v_sideMargin;
	m_StringOnScreen.updateInRectPos();
}
//------------------------------

//Text function setters---------
void ms::TextField::hideText(bool v_set, const char v_symbol)
{
	m_HideStringSet = v_set;
	m_HideStringSymbol = v_symbol;
}


void ms::TextField::setCharacterType(unsigned int v_characterType)
{
	m_CharTypeAccept = v_characterType;
}
//------------------------------


//Text manipulation-------------
void ms::TextField::clear()
{
	m_String.clear();
	m_HiddenString.clear();
	m_CursorPos = 0;

	if(m_HideStringSet)
		m_StringOnScreen.setString(m_HiddenString);
	else
		m_StringOnScreen.setString(m_String);
}

//------------------------------


void ms::TextField::selected(const bool v_set)
{
    m_Selected = v_set;
	if(m_SelectPositionReset)
	{
		m_CursorPos = m_String.size();
		m_SelectPositionReset = false;
	}
	updateCursorPos();
}
//------------------------------
//==============================


//Private=======================
//Drawing and view manipulation-
void ms::TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.setView(Globals::G_RresizedView);
		target.draw(m_Box);
		target.draw(m_HoverDrawable);

		target.setView(m_StringOnScreen.getTextVew(target));
			target.draw(m_StringOnScreen);

			if(m_CursorVisible && !m_CursorBlink)
				target.draw(m_Cursor);

	target.setView(Globals::G_RresizedView);
}
//------------------------------


//setup the gui component-------
void ms::TextField::setup()
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
	m_StringOnScreenBox.top = m_Position.y;
	m_StringOnScreenBox.left = m_Position.x + (m_SideMargins / 2);
	m_StringOnScreenBox.width = m_Size.x - m_SideMargins;
	m_StringOnScreenBox.height = m_Size.y;
	//------------------

    //Clear the string and update it
	m_String.clear();
	m_HiddenString.clear();
	//------------------

    //Setup the text that is visible on the screen
	m_TextSize = Globals::G_FontSizeAtStart;
	p_Font = Globals::G_MainFontPtr;
	m_StringOnScreen.setFont(*p_Font);
	m_StringOnScreen.setCharacterSize(m_TextSize);
	m_StringOnScreen.setFillColor(sf::Color::White);
	m_CharTypeAccept = (CharacterType::Letters | CharacterType::Numbers | CharacterType::Symbols | CharacterType::Spaces);
	//------------------

    //Setup the visible value
	m_StringOnScreen.setString(m_String);
	m_StringOnScreen.setTextPosInRect(TextAlignment::Left, TextAlignment::Middle);
	m_StringOnScreen.updateInRectPos();
	//------------------

    //Setup the cursor
	m_Cursor.setSize(sf::Vector2f(1, m_StringOnScreen.getLetterMaxHeight() + 1));
	m_Cursor.setFillColor(sf::Color::White);
	//------------------
}
//------------------------------


//Real time events and normal events
void ms::TextField::checkWithMouse()
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
			m_CursorPos = m_String.size();
			updateCursorPos();
		}
	}

	//--------------------------
	else if(m_Entered && Mouse::getClick(sf::Mouse::Button::Left))
	{
		m_StringOnScreen.setPosition(0, 0);
		if(m_HideStringSet)
			m_StringOnScreen.setString(m_HiddenString);
		else
			m_StringOnScreen.setString(m_String);
		m_Selected = false;
		m_Entered = false;
		m_SelectPositionReset = true;
		m_StringOnScreen.updateInRectPos();
	}

	else if(!m_Selected)
	{
		if(m_HoverColorSet)
			m_HoverDrawable.setFillColor(sf::Color::Transparent);
		m_Entered = false;
	}
}
//------------------------------


//Read the text and handle events
void ms::TextField::readText()
{
	if(m_Selected)
	{
		//Read text-------------
		if(ms::Globals::G_Event->type == sf::Event::TextEntered)
		{
			bool AddCharacterToString = false;


			//Checking what character can be entered
			if((m_CharTypeAccept & CharacterType::Numbers) && (Globals::G_Event->text.unicode < 58 && Globals::G_Event->text.unicode > 43 && Globals::G_Event->text.unicode != 47))
			{ AddCharacterToString = true; }

			else if((m_CharTypeAccept & CharacterType::Letters) && ((Globals::G_Event->text.unicode < 91 && Globals::G_Event->text.unicode > 64) 
																 || (Globals::G_Event->text.unicode < 123 && Globals::G_Event->text.unicode > 96)))
			{ AddCharacterToString = true; }
			
			else if((m_CharTypeAccept & CharacterType::Symbols) && ((Globals::G_Event->text.unicode < 48 && Globals::G_Event->text.unicode > 32) 
																 || (Globals::G_Event->text.unicode < 65 && Globals::G_Event->text.unicode > 57) 
																 || (Globals::G_Event->text.unicode < 97 && Globals::G_Event->text.unicode > 90) 
																 || (Globals::G_Event->text.unicode < 127 && Globals::G_Event->text.unicode > 122)))
			{ AddCharacterToString = true; }

			else if ((m_CharTypeAccept & CharacterType::Spaces) && (Globals::G_Event->text.unicode == 32))
			{ AddCharacterToString = true; }
			//------------------


			if(AddCharacterToString)
			{
				m_String.resize(m_String.size());
				m_String.insert(m_String.begin() + m_CursorPos, (char)Globals::G_Event->text.unicode);
				
				if(m_HideStringSet)
				{
					m_HiddenString.resize(m_HiddenString.size());
					m_HiddenString.insert(m_HiddenString.begin() + m_CursorPos, m_HideStringSymbol);
				}

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
				m_StringOnScreen.setPosition(0, 0);
				m_Selected = false;
				m_SelectPositionReset = true;
			}
			//Delete the last character
			else if(Globals::G_Event->key.code == sf::Keyboard::Backspace)
			{
				if(m_CursorPos > 0)
				{
					m_String.erase(m_String.begin() + m_CursorPos - 1);
					if(m_HideStringSet)
						m_HiddenString.erase(m_HiddenString.begin() + m_CursorPos - 1);
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
				if(m_CursorPos < m_String.size())
				{
					m_CursorPos++;
					m_ResetTimer = true;
				}
			}
		}

		//Update the changes
		if(m_HideStringSet)
			m_StringOnScreen.setString(m_HiddenString);
		else
			m_StringOnScreen.setString(m_String);

		updateCursorPos();
		slideValue();
		m_StringOnScreen.updateInRectPos();
	}
}
//------------------------------


//Slides the value if it gets out of the border
void ms::TextField::slideValue()
{
	//If the text is out of the box scroll it
	//To the right
	if(m_Cursor.getPosition().x > m_StringOnScreenBox.width)
	{
		float moveBy = fn::GetTRCorner(m_Cursor).x - m_StringOnScreenBox.width;
		m_StringOnScreen.move(-moveBy, 0);
	}

	//To the left
	else if(m_Cursor.getPosition().x < 0)
	{
		float moveBy = fn::GetTLCorner(m_Cursor).x;
		m_StringOnScreen.move(-moveBy, 0);
	}
}
//------------------------------


//Cursor Visuals----------------
//Update cursor position--------
void ms::TextField::updateCursorPos()
{
	if(m_String.empty() && m_RecalculateCursorPos)
	{
		m_String = "I";
		m_StringOnScreen.setString(m_String);
		m_StringOnScreen.updateInRectPos();
		m_Cursor.setPosition(m_StringOnScreen.findCharacterPos(0) + sf::Vector2f(0, m_StringOnScreen.getTextTopLenght() - 1));
		m_String.clear();

		if(m_HideStringSet)
			m_StringOnScreen.setString(m_HiddenString);
		else
			m_StringOnScreen.setString(m_String);
		
		m_RecalculateCursorPos = false;
	}
	else
		m_Cursor.setPosition(m_StringOnScreen.findCharacterPos(m_CursorPos) + sf::Vector2f(0, m_StringOnScreen.getTextTopLenght() - 1));
}
//------------------------------


//Allows the cursor to blink----
void ms::TextField::blinkingCursor()
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