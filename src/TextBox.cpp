#include "TextBox.h"
#include "functional"
#include "Globals.h"

//Public========================
//Constructors and Destructors--
ms::TextBox::TextBox(const sf::Vector2f& v_pos, const sf::Vector2f& v_size) :
        TextSlider(&m_SlideValue, sliderDirection::Vertical)
{
	m_Position = sf::Vector2f(v_pos);
	m_Size = sf::Vector2f(v_size);
	m_Scale = sf::Vector2f(1, 1);
	m_Origin = sf::Vector2f(0, 0);
   	setup();
}


ms::TextBox::~TextBox()
{

}
//------------------------------


//Update and event check--------
void ms::TextBox::update()
{
    checkWithMouse();

	if(m_SliderVisible)
	{
		TextSlider.update();
		if(TextSlider.isActive())
		{
			updateTextIndividualBoxes();
			wrapText();
		}
	}
}


void ms::TextBox::checkEvents()
{
	if(m_SliderVisible) 
		TextSlider.checkEvents();
}
//------------------------------


//Setters-----------------------
void ms::TextBox::setPosition(const sf::Vector2f& v_pos)
{
	m_Position = v_pos;
	m_Box.setPosition(m_Position);
	m_HoverDrawable.setPosition(m_Position);
	TextSlider.setPosition(sf::Vector2f(fn::GetTRCorner(m_Box).x - m_SliderWidth, fn::GetTRCorner(m_Box).y));
}


void ms::TextBox::setSize(const sf::Vector2f& v_size)
{
	m_Size = v_size;
	m_Box.setSize(m_Size);
	m_HoverDrawable.setSize(m_Size);
	TextSlider.setPosition(sf::Vector2f(fn::GetTRCorner(m_Box).x - m_SliderWidth, fn::GetTRCorner(m_Box).y));
	TextSlider.setSize(sf::Vector2f(m_SliderWidth ,m_Size.y));
}


void ms::TextBox::setOrigin(const sf::Vector2f& v_origin)
{
	m_Origin = v_origin;
	m_Box.setOrigin(m_Origin);
	m_HoverDrawable.setOrigin(m_Origin);
	TextSlider.setPosition(sf::Vector2f(fn::GetTRCorner(m_Box).x - m_SliderWidth, fn::GetTRCorner(m_Box).y));
}


void ms::TextBox::scale(const sf::Vector2f& v_scale)
{
	m_Scale = v_scale;

	//Scale position
	if(m_PositionScalable)
	{
		sf::Vector2f ScaledPosition(m_Position.x * m_Scale.x, m_Position.y * m_Scale.y);
		m_Box.setPosition(ScaledPosition);
		m_HoverDrawable.setPosition(m_Position);
	}	

	//Scale origin
	sf::Vector2f ScaledOriginPosition(m_Origin.x * m_Scale.x, m_Origin.y * m_Scale.y);
	m_Box.setOrigin(ScaledOriginPosition);
	m_HoverDrawable.setOrigin(ScaledOriginPosition);

	//Scale size
	sf::Vector2f ScaledSize(m_Size.x * m_Scale.x, m_Size.y * m_Scale.y);
	m_Box.setSize(ScaledSize);
	m_HoverDrawable.setSize(m_Size);

	//Scale and set slider
	TextSlider.setPosition(sf::Vector2f(fn::GetTRCorner(m_Box).x - m_SliderWidth, fn::GetTRCorner(m_Box).y));
	TextSlider.setSize(sf::Vector2f(m_SliderWidth ,m_Size.y * m_Scale.y));
}


void ms::TextBox::setString(std::string& v_string)
{
	m_Text = v_string;
}


//Visual setters----------------
void ms::TextBox::setForegroundColor(const sf::Color& v_color)
{
	m_TextColor = v_color;
}
//------------------------------


//Adds a string to an existing string
void ms::TextBox::addString(std::string& v_string)
{
	m_Text += v_string;
}


//Adds a character to an existing string
void ms::TextBox::addChar(char v_character)
{
	m_Text.push_back(v_character);;
}


void ms::TextBox::SetSliderVisible(bool v_set)
{
	m_SliderVisible = v_set;
}


void ms::TextBox::setTextSize(const unsigned int v_size)
{
	m_FontSize = v_size;
}

void ms::TextBox::setFont(const sf::Font& v_font)
{
	p_Font = &v_font;
}
//------------------------------


//Other functions---------------
void ms::TextBox::updateText()
{
	updateTextIndividualBoxes();
	wrapText();
	updateSliderLength();
}
//------------------------------

void ms::TextBox::selected(const bool v_set)
{
	m_Selected = v_set;
}
//==============================


//Private=======================
//Drawing and view manipulation-
void ms::TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(Globals::G_RresizedView);
		target.draw(m_Box);
		if(m_HoverColorSet)
			target.draw(m_HoverDrawable);

		if(m_SliderVisible)
			target.draw(TextSlider);

		for(int i = 0; i < m_TextOnScreen.size(); i++)
		{
			target.setView(m_TextOnScreen.at(i).getTextVew(target));
			target.draw(m_TextOnScreen.at(i));
		}
	target.setView(Globals::G_RresizedView);
		
}
//------------------------------


//setup the textbox-------------
void ms::TextBox::setup()
{
	m_Box.setPosition(m_Position);
    m_Box.setSize(m_Size);
    m_Box.setFillColor(sf::Color::Magenta);

	m_HoverDrawable.setPosition(m_Position);
	m_HoverDrawable.setSize(m_Size);
	m_HoverDrawable.setFillColor(sf::Color::Transparent);

	p_Font = Globals::G_MainFontPtr;
	m_FontSize = Globals::G_FontSizeAtStart;
	m_TextColor = sf::Color::White;

    TextSlider.setPosition(sf::Vector2f(m_Position.x + m_Size.x - m_SliderWidth, m_Position.y));
    TextSlider.setSize(sf::Vector2f(m_SliderWidth ,m_Size.y));

	updateTextIndividualBoxes();
}
//------------------------------


//Real time events--------------
void ms::TextBox::checkWithMouse()
{
    if(!m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos()) && !Mouse::getPressed(sf::Mouse::Button::Left))
	{
		m_Entered = true;
	}

	else if((m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos())) || m_Selected)
	{
		if(m_HoverColorSet)
			m_HoverDrawable.setFillColor(m_HoverColor);
	}

	else
	{
		if(m_HoverColorSet)
			m_HoverDrawable.setFillColor(sf::Color::Transparent);
		m_Entered = false;
	}

	TextSlider.setScrolling(m_Entered);
}
//------------------------------


//Text wrapping and setup functions
void ms::TextBox::wrapText()
{
	if(!m_TextOnScreen.empty())
	{
		int Line = 0;
		int LinePushed = 0;
		std::string CuttedTex = m_Text;
		TextPlus TextModel = m_TextOnScreen.at(0);
		TextModel.setString(CuttedTex);
		sf::FloatRect BoxModel = m_IndividualTextBoxes.at(0);

		//Lambda function that pushes the line
		auto PushLineToScreen =[&](int & CutTo)
		{
			if((m_LinesOnScreen >= LinePushed) && (Line >= m_LinesToSkip))
			{
				std::string temp;
				temp.assign(CuttedTex, 0, CutTo);
				m_TextOnScreen.at(LinePushed).setString(temp);
				LinePushed++;
			}
		};
		//------------------------------------

		//Clear the lines
		for(TextPlus& clear : m_TextOnScreen)
		{
			clear.setString("");
		}
		//---------------

		//Loop through all chars
		for(int Letter = 0; Letter < CuttedTex.length(); Letter++)
		{
			//If there is an end line char in the string
			if(CuttedTex[Letter] == '\n')
			{
				//Save the line
				PushLineToScreen(Letter);
				//--------------
				//Cut the string
				CuttedTex.erase(CuttedTex.begin(), CuttedTex.begin() + Letter + 1);
				TextModel.setString(CuttedTex);
				//--------------
				Line++;
				Letter = -1;
			}

			//If the character is outside the text box
			else if(TextModel.findCharacterPos(Letter).x >= BoxModel.width)
			{
				//Find Last Space
				int LastSpace = 0;
				for (int i = Letter - 1; i >= 0; i--)
				{
					//Space found
					if(CuttedTex[i] == ' ')
					{
						LastSpace = i + 1;
						break;
					}
					//-------------------------

					//No space. Cut the text at the edge
					else if(i == 0)
					{
						LastSpace = Letter - 1;
						break;
					}
					//-------------------------
				}
				//Save the line
				PushLineToScreen(LastSpace);
				//--------------
				//Cut the string
				CuttedTex.erase(CuttedTex.begin(), CuttedTex.begin() + LastSpace);
				TextModel.setString(CuttedTex);
				//--------------
				Line++;
				Letter = -1;
			}
		}
	

		//The last line (if visible)
		int LastChar = CuttedTex.length();
		PushLineToScreen(LastChar);

		m_LinesInText = Line;

		//Calculate the whole height of all lines even if they are not visible
		m_WholeTextHeight = BoxModel.height * (Line - m_LinesOnScreen)
						+ (m_IndividualTextBoxes.at(m_IndividualTextBoxes.size() - 1).height);
	}
}


void ms::TextBox::updateTextIndividualBoxes()
{
	//Calculate what portion of the text is shown
	const unsigned int ScrolledBy = m_SlideValue;
	int MoveBoxBy = 0;

	if((int)(m_FontSize * 1.2) + _MARGINTB != 0)
		m_LinesToSkip = ScrolledBy / ((int)(m_FontSize * 1.2) + _MARGINTB);
	else
	{
		std::cout << "msGUI ERROR: Class: TextBox, Function: updateTextIndividualBoxes() -> can't divide by zero\n";
		return;
	}
	
	MoveBoxBy = ScrolledBy % ((int)(m_FontSize * 1.2) + _MARGINTB);
	//------------------------------------------

	//Boxes setup------------------
	m_IndividualTextBoxes.clear();	//Delete all boxes

	int i = 0;
	sf::FloatRect NewBox;

	//Main box setup
	NewBox.top = fn::GetTLCorner(m_Box).y + _MARGINTB;
	NewBox.left = fn::GetTLCorner(m_Box).x + _MARGINRL;
	NewBox.width = fn::GetWidth(m_Box) - fn::GetWidth(TextSlider.getBox()) - (2 * _MARGINRL);
	NewBox.height = (m_FontSize * 1.2) + _MARGINTB;
	//--------------

	m_IndividualTextBoxes.push_back(NewBox);
	m_IndividualTextBoxes.at(0).height -= MoveBoxBy;

	//Setup the rest of the boxes
	while(NewBox.top + NewBox.height < fn::GetBRCorner(m_Box).y)
	{	
		NewBox.top = m_IndividualTextBoxes.at(i).top + m_IndividualTextBoxes.at(i).height + 1;
		m_IndividualTextBoxes.push_back(NewBox);
		i++;
	}
	//-----------------------
	
	//Cut down the last box
	float LastBoxHeight = fn::GetBRCorner(m_Box).y - NewBox.top - _MARGINTB;
	if(LastBoxHeight > 0)
		m_IndividualTextBoxes.at(m_IndividualTextBoxes.size() - 1).height = fn::GetBRCorner(m_Box).y - NewBox.top - _MARGINTB;
	else
	{
		m_IndividualTextBoxes.pop_back();
		i--;
	}


	m_LinesOnScreen = i;
	//------------------------------------

	//Text setup--------------------------
	m_TextOnScreen.clear();
	for(sf::FloatRect& fRect : m_IndividualTextBoxes)
	{
		
		TextPlus NewLine(fRect);
		NewLine.setFont(*p_Font);
		NewLine.setCharacterSize(m_FontSize);
		NewLine.setFillColor(m_TextColor);
		//Change new text adding
    	NewLine.setTextPosInRect(TextAlignment::Left, TextAlignment::Middle);
		//---------------------
    	NewLine.updateInRectPos();
		m_TextOnScreen.push_back(NewLine);
	}

	//Move the first line up(cutting the top portion of text effect)
	if(!m_TextOnScreen.empty())
		m_TextOnScreen.at(0).move(0, -MoveBoxBy);
}

void ms::TextBox::updateSliderLength()
{
	if(!m_TextOnScreen.empty())
	{
		TextSlider.setValueRange(0, m_WholeTextHeight);
		TextSlider.setSliderAutoAdjustVal(m_LinesOnScreen * ((int)(m_FontSize * 1.2) + _MARGINTB));
	}
}
//------------------------------
//==============================