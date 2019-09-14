#pragma once
#include <SFML/Graphics.hpp>

namespace ms
{
	class Globals
	{
	public:
		Globals() {};
		~Globals() {};

		static sf::Event* G_Event;
		static sf::Font* G_MainFontPtr;
		const static float G_FontSizeAtStart;
		const static sf::Time G_BlinkinkOffset;
		static sf::Clock G_BlinkingClock;
		static sf::View G_RresizedView;
	};
};

