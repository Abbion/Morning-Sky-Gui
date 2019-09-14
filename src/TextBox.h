#pragma once
#include "GuiEssential_Visual.h"
#include "Slider.h"
#include "TextPlus.h"
#include <vector>

namespace ms
{
	class TextBox : public GuiEssential_Visual, public sf::Drawable
	{
	public:
		Slider<int> TextSlider;

	public:
		TextBox(const sf::Vector2f& v_pos, const sf::Vector2f& v_size);
		~TextBox();

		virtual void update();
		virtual void checkEvents();

		virtual void setPosition(const sf::Vector2f& v_pos);
		virtual void setSize(const sf::Vector2f& v_size);
		virtual void setOrigin(const sf::Vector2f& v_origin);
		virtual void scale(const sf::Vector2f& v_scale);

		void setForegroundColor(const sf::Color& v_color);

		void setString(std::string& v_string);
		void addString(std::string& v_string);
		void addChar(char v_character);

		void SetSliderVisible(bool v_set);
		void setTextSize(const unsigned int v_size);
		void setFont(const sf::Font& v_font);

		void updateText();

		void selected(const bool v_set);				//Sets the button as active
        bool isHover() const { return m_Entered; };		//If the mouse is hovering over the button return true

	private:
		std::string m_Text;
		std::vector<TextPlus> m_TextOnScreen;
		std::vector<sf::FloatRect> m_IndividualTextBoxes;

		bool m_Selected = false;

		unsigned int m_FontSize = 0;
		const sf::Font* p_Font = nullptr;
		sf::Color m_TextColor;

		unsigned int m_LinesOnScreen = 0;
		unsigned int m_LinesInText = 0;
		unsigned int m_WholeTextHeight = 0;
		unsigned int m_LinesToSkip = 0;

		int m_SlideValue;
		const float m_SliderWidth = 10;
		bool m_SliderVisible = true;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void setup();

		void checkWithMouse();
		void wrapText();
		void updateTextIndividualBoxes();
		void updateSliderLength();
	};
};