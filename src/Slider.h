#pragma once
#include "GuiEssential_Visual.h"
#include "Functional.h"
#include "Mouse.h"
#include <SFML/Graphics.hpp>

//INFO+++++++++++++
//Background of the slider = rail
//Foreground of the slider = cart
//Whole slider = slider
//+++++++++++++++++


namespace ms
{
    //Types of sliders
    enum sliderDirection
    {
        Vertical,
        Horizontal
    };
    //----------------


    template<typename T>
    class Slider : public GuiEssential, public sf::Drawable
    {
    public:
        Slider(const sf::Vector2f& v_pos, const sf::Vector2f& v_size, T* v_value, sliderDirection v_sliderDir, T v_Min, T v_Max);
        Slider(T* v_value, sliderDirection v_sliderDir);
		~Slider();

        virtual void update();
		virtual void checkEvents();

        virtual void setPosition(const sf::Vector2f& v_pos);
		virtual void setSize(const sf::Vector2f& v_size);
		virtual void setOrigin(const sf::Vector2f& v_origin);
		virtual void scale(const sf::Vector2f& v_scale);

        void setValue(const T& v_value);                    //Set a value to the slider and position the cart
        void setSliderAutoAdjustVal(float v_limit);         //Change the range of auto adjust slider
        void setValueRange(T v_min, T v_max);               //Sets the range of the values
        void setScrolling(bool v_set);                      //Set the ability to scroll
        void setScrollingPower(float v_power);				//Sets how fast the scrolling will be
        
        void calculateSliderPos();							//Updates the slider position if the value was changed by hand e.g. by void setValue()

        void selected(const bool v_set);					//Sets the button as active

        bool isActive() const   { return m_IsActive || m_Dragged; }		//Returns true if the slider is active e.g. the slider is beeing dragged
        bool isHover() const    { return m_Entered; }		//If the mouse is hovering over the button return true

        void setBackgroundColor(const sf::Color& v_color);
        void setBackgroundTexture(sf::Texture& v_texture);
        void setBackgroundTexture(sf::Texture& v_texture, sf::IntRect v_rect);

        void setForegroundColor(const sf::Color& v_color);
        void setForegroundHoverColor(const sf::Color& v_color);
        void setForegroundHoverTransparency(const sf::Uint8 v_power);
        void setForegroundTexture(sf::Texture& v_texture);
        void setForegroundTexture(sf::Texture& v_texture, sf::IntRect v_rect);

    private:
        bool m_Entered = false;
        bool m_Dragged = false;
        bool m_IsActive = false;
        sf::Vector2f m_GrabPoint;
        bool m_GrabPointSet = false;

        bool m_Selected = false;
        //------------------------

        bool m_PositionScalable = false;

        sliderDirection m_SliderDir;
        sf::RectangleShape m_Slider;

        T m_MaxValue = 0;
        T m_MinValue = 0;
        T* p_Value = nullptr;

        //Only for auto adjusting sliders
        float m_Limit = 0;
        bool m_AtuoAdjustSet = false;
        bool m_DrawSlider = true;

        //Scrolling
        bool m_ScrollingSet = false;
        float m_ScrollPower = 15;
        float m_ScrollBy = 0;
        //-------------------------------


        //Foreground look
        sf::Color m_BackgroundColor;
        sf::Texture* p_BackgroundTexture = nullptr;

        bool m_ForegroundHoverColorSet = false;
        sf::Color m_ForegroundHoverColor;
        sf::Color m_ForegroundColor;
        sf::Texture* p_ForegroundTexture = nullptr;
        sf::RectangleShape m_ForegroundHoverDrawable;
        //-------------------------------

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void setup();

        void checkWithMouse();
        void drag();
        void scroll();
        void sliderBlock();                 //Block the cart if it is out of the rail
        void autoResizeSlider();            //Only for auto adjust sliders
        void calculateValue();              
    };
};



//Declarations###########################################################################
//Public========================
//Constructors and destructors--
template<typename T>
ms::Slider<T>::Slider(const sf::Vector2f& v_pos, const sf::Vector2f& v_size, T* v_value, ms::sliderDirection v_sliderDir, T v_Min, T v_Max) :
    p_Value(v_value), m_SliderDir(v_sliderDir), m_MinValue(v_Min), m_MaxValue(v_Max)
{
    m_Position = sf::Vector2f(v_pos);
    m_Size = sf::Vector2f(v_size);
    m_Scale = sf::Vector2f(1, 1);
    setup();
}


template<typename T>
ms::Slider<T>::Slider(T* v_value, ms::sliderDirection v_sliderDir) :
    p_Value(v_value), m_SliderDir(v_sliderDir)
{
    m_Position = sf::Vector2f(0, 0);
    if(m_SliderDir == ms::sliderDirection::Horizontal)
	    m_Size = sf::Vector2f(200, 20);
    else
        m_Size = sf::Vector2f(20, 200);
	m_Scale = sf::Vector2f(1, 1);
   setup();
}


template<typename T>
ms::Slider<T>::~Slider()
{

}
//------------------------------



//Update and event check--------
template<typename T>
void ms::Slider<T>::update()
{
    checkWithMouse();
    if(m_Dragged)
    {
        drag();
        sliderBlock();
        calculateValue();
    }

    if(m_ScrollingSet)
    {
        scroll();
    }

	if (m_ForegroundHoverColorSet)
	{
		m_ForegroundHoverDrawable.setPosition(m_Slider.getPosition());
		m_ForegroundHoverDrawable.setSize(m_Slider.getSize());
	}
}


template<typename T>
void ms::Slider<T>::checkEvents()
{
    if(m_ScrollingSet)
    {
        if(Globals::G_Event->type == sf::Event::MouseWheelScrolled)
        {
            m_ScrollBy = -Globals::G_Event->mouseWheelScroll.delta;
            m_IsActive = true;
        }
        else
            m_IsActive = false;
    }
}
//------------------------------



//Setters-----------------------
template<typename T>
void ms::Slider<T>::setPosition(const sf::Vector2f& v_pos)
{
    m_Position = v_pos;
    m_Box.setPosition(m_Position);
    m_Slider.setPosition(m_Position);
}


template<typename T>
void ms::Slider<T>::setSize(const sf::Vector2f& v_size)
{
    m_Size = v_size;
    m_Box.setSize(m_Size);
    if(m_SliderDir == sliderDirection::Horizontal)
		m_Slider.setSize(sf::Vector2f(m_Size.y, m_Size.y));
    else
        m_Slider.setSize(sf::Vector2f(m_Size.x, m_Size.x));
}


template<class T>
void ms::Slider<T>::setOrigin(const sf::Vector2f& v_origin)
{
    m_Origin = v_origin;
    m_Box.setOrigin(m_Origin);
    m_Slider.setPosition(fn::GetTLCorner(m_Box));
}


template<class T>
void ms::Slider<T>::scale(const sf::Vector2f& v_scale)
{
    m_Scale = v_scale;

    //Scale position
	if(m_PositionScalable)
	{
        sf::Vector2f ScaledPosition(m_Position.x * m_Scale.x, m_Position.y * m_Scale.y);
        m_Box.setPosition(ScaledPosition);
    }

    //Scale origin
	sf::Vector2f ScaledOriginPosition(m_Origin.x * m_Scale.x, m_Origin.y * m_Scale.y);
    m_Box.setOrigin(ScaledOriginPosition);

    //Scale size	
    sf::Vector2f ScaledSize(m_Size.x * m_Scale.x, m_Size.y * m_Scale.y);
    m_Box.setSize(ScaledSize);
    

    if(m_SliderDir == sliderDirection::Horizontal)
        m_Slider.setSize(sf::Vector2f(ScaledSize.y, ScaledSize.y));
    else
        m_Slider.setSize(sf::Vector2f(ScaledSize.x, ScaledSize.x));
    
    m_Slider.setPosition(fn::GetTLCorner(m_Box));
}


template<typename T>
void ms::Slider<T>::setValue(const T& v_value)
{
    *p_Value = v_value;
    calculateSliderPos();
}


//v_limit = 0 disable auto adjustment
template<typename T>
void ms::Slider<T>::setSliderAutoAdjustVal(float v_limit)
{
    if(v_limit == 0)
    {
        m_AtuoAdjustSet = false;
        if(m_SliderDir == sliderDirection::Horizontal) 
            m_Slider.setSize(sf::Vector2f(m_Size.y * m_Scale.y, m_Size.y * m_Scale.y));
        else
            m_Slider.setSize(sf::Vector2f(m_Size.x * m_Scale.x, m_Size.x * m_Scale.x));
    }

    else
    {
        m_AtuoAdjustSet = true;
        m_Limit = v_limit;
        autoResizeSlider();
    }
}


template<typename T>
void ms::Slider<T>::setValueRange(T v_min, T v_max)
{
    m_MinValue = v_min;
    m_MaxValue = v_max;
}


template<typename T>
void ms::Slider<T>::setScrolling(bool v_set)
{
    m_ScrollingSet = v_set;
}


template<typename T>
void ms::Slider<T>::setScrollingPower(float v_power)
{
    m_ScrollPower = v_power;
}


//If we set the value for the slider we must change its position here
template<typename T>
void ms::Slider<T>::calculateSliderPos()
{
    float FullRangeValue = std::abs(m_MinValue) + std::abs(m_MaxValue); //The whole value from 0 to full range
    if(FullRangeValue != 0)
    {
        float procentage = (float)(*p_Value + std::abs(m_MinValue))/FullRangeValue;     //Where in percentage move the cart
        if(procentage > 1)
            procentage = 1;
        else if(procentage < 0)
            procentage = 0;

         //For horizontal sliders
        if(m_SliderDir == sliderDirection::Horizontal)
            m_Slider.setPosition(fn::GetTLCorner(m_Box).x + (procentage * fn::GetWidth(m_Box) - (m_Slider.getSize().y / 2)), m_Slider.getPosition().y);
        //For vertical sliders
        else
            m_Slider.setPosition(m_Slider.getPosition().x, fn::GetTLCorner(m_Box).y + (procentage * fn::GetHeight(m_Box) - (m_Slider.getSize().x / 2)));

        //Check if the cart is in the rail
        sliderBlock();
    }

    //Error
    else
    {
        std::cout << "msGUI ERROR: Class: Slider, Function: calculateSliderPos() -> " << std::endl;
        std::cout << "| m_MinValue = " << m_MinValue << " | + | m_MaxValue = " << m_MaxValue << " | = 0" << std::endl;
        std::cout << "You can't divide by 0" << std::endl;
    }
}
//------------------------------


template<typename T>
void ms::Slider<T>::selected(const bool v_set)
{
    m_Selected = v_set;
	calculateSliderPos();
}


//Visual stetters---------------
template<typename T>
void ms::Slider<T>::setBackgroundColor(const sf::Color& v_color)
{
    m_BackgroundColor = v_color;
    m_Box.setFillColor(m_BackgroundColor);
}


template<typename T>
void ms::Slider<T>::setBackgroundTexture(sf::Texture& v_texture)
{
    p_BackgroundTexture = &v_texture;
    m_Box.setTexture(p_BackgroundTexture);
    m_Box.setFillColor(sf::Color::White);
}


template<typename T>
void ms::Slider<T>::setBackgroundTexture(sf::Texture& v_texture, sf::IntRect v_rect)
{
    p_BackgroundTexture = &v_texture;
    m_Box.setTexture(p_BackgroundTexture);
    m_Box.setTextureRect(v_rect);
    m_Box.setFillColor(sf::Color::White);
}


template<typename T>
void ms::Slider<T>::setForegroundColor(const sf::Color& v_color)
{
    m_ForegroundColor = v_color;
    m_Slider.setFillColor(m_ForegroundColor);
}


template<typename T>
void ms::Slider<T>::setForegroundHoverColor(const sf::Color& v_color)
{
    m_ForegroundHoverColorSet = true;
    m_ForegroundHoverColor = v_color;
	m_ForegroundHoverColor.a = 50;
}


template<typename T>
void ms::Slider<T>::setForegroundHoverTransparency(const sf::Uint8 v_power)
{
    m_ForegroundHoverColor.a = 1 - v_power;
}


template<typename T>
void ms::Slider<T>::setForegroundTexture(sf::Texture& v_texture)
{
    p_ForegroundTexture = &v_texture;
    m_Slider.setTexture(p_ForegroundTexture);
    m_Slider.setFillColor(sf::Color::White);
}


template<typename T>
void ms::Slider<T>::setForegroundTexture(sf::Texture& v_texture, sf::IntRect v_rect)
{
    p_ForegroundTexture = &v_texture;
	m_Slider.setTexture(p_ForegroundTexture);
    m_Slider.setTextureRect(v_rect);
    m_Slider.setFillColor(sf::Color::White);
}
//------------------------------
//==============================


//Private=======================
//Drawing and view manipulation-
template<typename T>
void ms::Slider<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_Box);

    if(m_DrawSlider)
    {
        target.draw(m_Slider);
		if(m_ForegroundHoverColorSet)
			target.draw(m_ForegroundHoverDrawable);
    }
}
//------------------------------


//Slider setup
template<typename T>
void ms::Slider<T>::setup()
{
    m_Box.setPosition(m_Position);
    m_Box.setSize(m_Size);
    m_Box.setFillColor(sf::Color::Magenta);

    m_Slider.setPosition(m_Position);
	m_Slider.setFillColor(sf::Color::White);

    if(m_SliderDir == sliderDirection::Horizontal)
        m_Slider.setSize(sf::Vector2f(m_Size.y, m_Size.y));
    else
        m_Slider.setSize(sf::Vector2f(m_Size.x, m_Size.x));

    m_ForegroundHoverDrawable.setFillColor(sf::Color::Transparent);
}
//------------------------------


//Real time events--------------
template<typename T>
void ms::Slider<T>::checkWithMouse()
{
    //Check if the mouse entered the rail meaning the button is not pressed and the mouse isn't dragging anything
    if(!m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos()) && !Mouse::getPressed(sf::Mouse::Button::Left))
    {
    	m_Entered = true;
    }

    //If we entered clear we can drag
    else if ((m_Entered && m_Box.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos())) || m_Selected)
	{
		if ((m_Slider.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos())) || m_Selected)
			m_ForegroundHoverDrawable.setFillColor(m_ForegroundHoverColor);
		else
			m_ForegroundHoverDrawable.setFillColor(sf::Color::Transparent);

        if(Mouse::getPressed(sf::Mouse::Left))
            m_Dragged = true;
        else
        {
            m_Dragged = false;
            m_GrabPointSet = false;
        }
	}

	else if(m_Entered && !Mouse::getPressed(sf::Mouse::Left))
	{
		m_ForegroundHoverDrawable.setFillColor(sf::Color::Transparent);
		m_Dragged = false;
        m_Entered = false;
        m_GrabPointSet = false;
	}
}
//------------------------------


//Other functions---------------
//Allows to drag the slider
template<typename T>
void ms::Slider<T>::drag()
{
    //If we are not colliding with the cart we calculate a grab point in the middle and cart and attach the cart to that point.
    if(!m_GrabPointSet)
    {
        if(!m_Slider.getGlobalBounds().contains((sf::Vector2f)Mouse::getInWindowPos()))
        {
            if(m_SliderDir == sliderDirection::Horizontal)
                m_Slider.setPosition(sf::Vector2f(Mouse::getInWindowPos().x - (fn::GetWidth(m_Slider) / 2) ,m_Slider.getPosition().y));
            else if(m_SliderDir == sliderDirection::Vertical)
                m_Slider.setPosition(sf::Vector2f(m_Slider.getPosition().x, Mouse::getInWindowPos().y - (fn::GetHeight(m_Slider) / 2)));
        }
        
        //Set the grab point to the place where the mouse is colliding with the cart
        //Or if we are not colliding with the cart read the upper if statement
        m_GrabPoint = (sf::Vector2f)Mouse::getInWindowPos() - m_Slider.getPosition();
        m_GrabPointSet = true;
    }

    //If we are colliding with the cart and we have a drag point we can move it
    if(m_SliderDir == sliderDirection::Horizontal)
        m_Slider.setPosition(sf::Vector2f(ms::Mouse::getInWindowPos().x - m_GrabPoint.x , m_Slider.getPosition().y));

    else if(m_SliderDir == sliderDirection::Vertical)
        m_Slider.setPosition(sf::Vector2f(m_Slider.getPosition().x , ms::Mouse::getInWindowPos().y - m_GrabPoint.y));
}
//------------------------------

//Scrolling with the scroll wheel
template<typename T>
void ms::Slider<T>::scroll()
{
    if(m_SliderDir == sliderDirection::Horizontal)
        m_Slider.move(sf::Vector2f(m_ScrollBy * m_ScrollPower, 0));
    else
        m_Slider.move(sf::Vector2f(0, m_ScrollBy * m_ScrollPower));

    m_ScrollBy = 0;
    sliderBlock();
    calculateValue();
}
//------------------------------


//Block the slider if the cart is out of the rail
template<typename T>
void ms::Slider<T>::sliderBlock()
{
    //For horizontal sliders
    if(m_SliderDir == sliderDirection::Horizontal)
    {
        //Bind to the left
        if(fn::GetTLCorner(m_Slider).x < fn::GetTLCorner(m_Box).x)
        {
            m_Slider.setPosition(sf::Vector2f(fn::GetTLCorner(m_Box).x, m_Slider.getPosition().y));
        }

        //Blind to the right
        else if(fn::GetTRCorner(m_Slider).x > fn::GetTRCorner(m_Box).x)
        {
            m_Slider.setPosition(sf::Vector2f(fn::GetTRCorner(m_Box).x - fn::GetWidth(m_Slider), m_Slider.getPosition().y));
        }
    }
	
    //For vertical sliders
    else if (m_SliderDir == sliderDirection::Vertical)
    {
        //Bind to the top
        if(fn::GetTRCorner(m_Slider).y < fn::GetTRCorner(m_Box).y)
        {
            m_Slider.setPosition(sf::Vector2f(m_Slider.getPosition().x, fn::GetTRCorner(m_Box).y));
        }

        //Bind to the bottom
        else if(fn::GetBRCorner(m_Slider).y > fn::GetBRCorner(m_Box).y)
        {
             m_Slider.setPosition(sf::Vector2f(m_Slider.getPosition().x, fn::GetBRCorner(m_Box).y - fn::GetHeight(m_Slider)));
        }
    }
}
//------------------------------


//If we have an auto adjuster slider we must resize it depending on the limit
template<typename T>
void ms::Slider<T>::autoResizeSlider()
{
    //Don't draw the slider
    if(m_Limit >= m_MaxValue)
        m_DrawSlider = false;

    else
    {
        float FullRangeValue = std::abs(m_MinValue) + std::abs(m_MaxValue);    //The whole value from 0 to full range
        float procentage = (float)m_Limit/FullRangeValue;   //How much percentage of the high should we set

        //For horizontal sliders
        if(m_SliderDir == sliderDirection::Horizontal) 
        {
            float xSize = fn::GetWidth(m_Box) * procentage;
            if(xSize < 5)   //if size is smaller than 5px set the size to 5px
                xSize = 5;
            m_Slider.setSize(sf::Vector2f(xSize, m_Slider.getSize().y));
        }
        
        //For vertical sliders
        else
        {
            float ySize = fn::GetHeight(m_Box) * procentage;
            if(ySize < 5)   //if size is smaller than 5px set the size to 5px
                ySize = 5;
            m_Slider.setSize(sf::Vector2f(m_Slider.getSize().x, ySize));
        }

		m_DrawSlider = true;
    }
}
//------------------------------


//Calculates the value depending on the cart in rail percentage position
template<typename T>
void ms::Slider<T>::calculateValue()
{
     //For horizontal sliders
    if(m_SliderDir == sliderDirection::Horizontal)
    {
        float Max = fn::GetWidth(m_Box) - fn::GetWidth(m_Slider);                       //Max length of the rail
        float FullRangeValue = std::abs(m_MinValue) + std::abs(m_MaxValue);    //The whole value from 0 to full range
        if(Max <= 0)
            std::cout << "ERROR: slide width is zero or less.";
        else
        {
            float procentage = (fn::GetTLCorner(m_Slider).x - fn::GetTLCorner(m_Box).x) / Max; //Calculate the percentage
            *p_Value = m_MinValue + (procentage * FullRangeValue);  //Calculate the value depending on the percentage
        }
    }

    //For vertical sliders
	else if (m_SliderDir == sliderDirection::Vertical)
	{
		float Max = fn::GetHeight(m_Box) - fn::GetHeight(m_Slider);                     //Max length of the rail
        float FullRangeValue = std::abs(m_MinValue) + std::abs(m_MaxValue);    //The whole value from 0 to full range
        if(Max <= 0)
            std::cout << "ERROR: slide width is zero or less.";
        else
        {
            float procentage = (fn::GetTLCorner(m_Slider).y - fn::GetTLCorner(m_Box).y) / Max; //Calculate the percentage
			*p_Value = m_MinValue + (procentage * FullRangeValue);   //Calculate the value depending on the percentage
        }
	}
}
//------------------------------
//==============================