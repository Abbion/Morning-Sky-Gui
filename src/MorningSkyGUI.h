#pragma once
#include <vector>
#include <string>

#include "Globals.h"
#include "functional"

#include "Button.h"
#include "CheckBox.h"
#include "ValueField.h"
#include "TextField.h"
#include "TextBox.h"
#include "Slider.h"


sf::Event* ms::Globals::G_Event = nullptr;

sf::Font MainFont;
sf::Font* ms::Globals::G_MainFontPtr = &MainFont;

const float ms::Globals::G_FontSizeAtStart = 14.0f;

const sf::Time ms::Globals::G_BlinkinkOffset = sf::milliseconds(500);
sf::Clock ms::Globals::G_BlinkingClock = sf::Clock();

sf::View ms::Globals::G_RresizedView = sf::View();
//------------------------------------------


namespace ms
{
    void Init(sf::RenderWindow& v_renderWindow, bool v_loadMainFont = true);	//Initializes the gui. It should be used right after creating a program window
	void setEventSource(sf::Event& v_event);	//Sets the event as a global value that can be used by any gui component in the check event function
	void checkGlobalEvents();					//Checks events that have a global reach
	void update();								//Updates the core components that are used commonly in gui components
};


void ms::Init(sf::RenderWindow& v_renderWindow, bool v_loadMainFont)
{
	if(v_loadMainFont)
	{
		if(!MainFont.loadFromFile("Fonts/Arial.ttf"))
		{
			std::cout << "msGUI ERROR: Class: NONE, File: MorningSkyGUI.h, Function: Init -> " <<
						"Can't load initial font. Path: \"Fonts/Arial.ttf\". Please create the folder and add the font file there or set the second argument of Init() to false.\n";
		}
	}

	Globals::G_RresizedView = v_renderWindow.getDefaultView();
    Mouse::setWindowPtr(v_renderWindow);
}

void ms::setEventSource(sf::Event& v_event)
{
	Globals::G_Event = &v_event;
}

void ms::checkGlobalEvents()
{
	if (Globals::G_Event->type == sf::Event::Resized)
	{
		sf::FloatRect ViewRect(0, 0, Globals::G_Event->size.width, Globals::G_Event->size.height);
		Globals::G_RresizedView = sf::View(ViewRect);
	}
}

void ms::update()
{
	Mouse::update();
}