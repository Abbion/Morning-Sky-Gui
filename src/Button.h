#pragma once
#include "ClickBase.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace ms
{
	class Button : public ClickBase, public sf::Drawable
	{
	public:
		Button(const sf::Vector2f& v_pos, const sf::Vector2f& v_size, bool* v_value);
		Button(bool* v_value);
		~Button();

	private:
		bool* p_Clicked = nullptr;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void checkWithMouse();
	};
};