#include "Mouse.h"

//Initialization================
bool ms::Mouse::m_DeltaHold_R = false;
bool ms::Mouse::m_Hold_R = false;

bool ms::Mouse::m_DeltaHold_L = false;
bool ms::Mouse::m_Hold_L = false;

bool ms::Mouse::m_DeltaHold_M = false;
bool ms::Mouse::m_Hold_M = false;

sf::Vector2i ms::Mouse::m_WindowPos = sf::Vector2i(0, 0);
sf::Vector2i ms::Mouse::m_GlobalPos = sf::Vector2i(0, 0);
sf::Vector2i ms::Mouse::m_DeltaPos = sf::Vector2i(0, 0);

const sf::RenderWindow* ms::Mouse::p_Window = nullptr;
//==============================


//Public========================
//Constructors and Destructors--
ms::Mouse::Mouse()
{
    
}


ms::Mouse::~Mouse()
{

}
//------------------------------


//Update and event check--------
void ms::Mouse::update()
{
    m_DeltaPos = m_GlobalPos;

    m_DeltaHold_R = m_Hold_R;
    m_DeltaHold_L = m_Hold_L;
    m_DeltaHold_M = m_Hold_M;

    m_Hold_R = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    m_Hold_L = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    m_Hold_M = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);

    m_GlobalPos = sf::Mouse::getPosition();
}
//------------------------------


//Setters-----------------------
void ms::Mouse::setWindowPtr(const sf::RenderWindow& v_window)
{
    p_Window = &v_window;
}
//------------------------------


//Getters-----------------------
const sf::Vector2i& ms::Mouse::getInWindowPos()
{
    m_WindowPos = sf::Mouse::getPosition(*p_Window);
    return m_WindowPos;
}


const sf::Vector2i& ms::Mouse::getGlobalPos()
{
    m_GlobalPos = sf::Mouse::getPosition();
    return m_GlobalPos;
}


const sf::Vector2i ms::Mouse::getDeltaPos()
{
    return sf::Vector2i(m_GlobalPos - m_DeltaPos);
}


const bool ms::Mouse::getClick(const sf::Mouse::Button Button)
{
    if(Button == sf::Mouse::Button::Right)
    {
        if(m_DeltaHold_R && !m_Hold_R)
            return true;
        else
            return false;
    }

    else if(Button == sf::Mouse::Button::Left)
    {
        if(m_DeltaHold_L && !m_Hold_L)
            return true;
        else
            return false;
    }

    else if(Button == sf::Mouse::Button::Middle)
    {
        if(m_DeltaHold_M && !m_Hold_M)
            return true;
        else
            return false;
    }

    else
        std::cout << "msGUI ERROR: Class: Mouse, Function: getClick() -> This button is not clickable\n";
}


const bool ms::Mouse::getPressed(const sf::Mouse::Button Button)
{
    if(Button == sf::Mouse::Button::Right)
        return m_Hold_R;

    else if(Button == sf::Mouse::Button::Left)
        return m_Hold_L;

    else if(Button == sf::Mouse::Button::Middle)
        return m_Hold_M;

    else
        std::cout << "msGUI ERROR: Class: Mouse, Function: getClick() -> This button is not pressable\n";
}
//------------------------------
//==============================