#include "GuiEssential_Visual.h"
#include <SFML/Graphics/Color.hpp>


//Public========================
void ms::GuiEssential_Visual::setBackgroundColor(const sf::Color& v_color)
{
    m_BackgroundColor = v_color;
    m_Box.setFillColor(m_BackgroundColor);
}


void ms::GuiEssential_Visual::setHoverColor(const sf::Color& v_color)
{
    m_HoverColorSet = true;
	m_HoverColor = v_color;
    m_HoverColor.a = 50;
}


void ms::GuiEssential_Visual::setHoverTransparency(const sf::Uint8 v_power)
{
    m_HoverColor.a = 255 - v_power;
}


void ms::GuiEssential_Visual::setBackgroundTexture(sf::Texture& v_texture)
{
    p_BackgroundTexture = &v_texture;
    m_Box.setTexture(p_BackgroundTexture);
    m_Box.setFillColor(sf::Color::White);
}


void ms::GuiEssential_Visual::setBackgroundTexture(sf::Texture& v_texture, sf::IntRect v_rect)
{
    p_BackgroundTexture = &v_texture;
    m_Box.setTexture(p_BackgroundTexture);
    m_Box.setTextureRect(v_rect);
    m_Box.setFillColor(sf::Color::White);
}
//==============================