#pragma once
#include "Globals.h"
#include "GuiEssential_Visual.h"
#include "TextPlus.h"

#include <string>

namespace ms
{
	enum CharacterType
	{
		Letters = 1 << 0,
		Numbers = 1 << 1,
		Symbols = 1 << 2,
		Spaces = 1 << 3
	};

	class TextField : public GuiEssential_Visual, public sf::Drawable
	{
	public:
		TextField(const sf::Vector2f& v_pos, const float& v_width);
		TextField();
		~TextField();

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

		void hideText(bool v_set, const char v_symbol = '*');	//Creates a password like text field
		void setCharacterType(unsigned int v_characterType);	//Sets shat characters will be accepted to read form keyboard

		void clear();

		void selected(const bool v_set);						//Sets the button as active
        const bool isHover() const { return m_Entered; }		//If the mouse is hovering over the button return true
		const std::string getString() const { return m_String; }
	
	private:
		std::string m_String;
		std::string m_HiddenString;
		unsigned int m_CharTypeAccept;
		bool m_HideStringSet = false;
		char m_HideStringSymbol = '*';

		TextPlus m_StringOnScreen;
		sf::FloatRect m_StringOnScreenBox;
		unsigned int m_TextSize;
		const sf::Font* p_Font = nullptr;
		int m_SideMargins = _MARGINRL;

		bool m_Selected = false;
		bool m_SelectPositionReset = true;

		sf::RectangleShape m_Cursor;
		int m_CursorPos = 0;
		bool m_CursorVisible = true;
		bool m_CursorBlink = false;
		bool m_ResetTimer = false;
		bool m_RecalculateCursorPos = false;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void setup();

		void checkWithMouse();
		void readText();
		void slideValue();

		void updateCursorPos();
		void blinkingCursor();
	};
}