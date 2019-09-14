#pragma once
#include <SFML/Graphics.hpp>

namespace ms
{
	//Base class for all kinds of GUI elements
	class GuiEssential
	{
	protected:	
		sf::RectangleShape m_Box;
		sf::Vector2f m_Position;
		sf::Vector2f m_Size;
		sf::Vector2f m_Scale;
		sf::Vector2f m_Origin;

		bool m_PositionScalable;
		bool m_Entered = false;
		bool m_Hover;

	public:
		GuiEssential() {}
		virtual ~GuiEssential() {}

		virtual void update() = 0;
		virtual void checkEvents() = 0;

		virtual void setPosition	(const sf::Vector2f& v_pos) 	{}
		virtual void setSize		(const sf::Vector2f& v_size)	{}
		virtual void setOrigin		(const sf::Vector2f& v_origin)	{}
		virtual void scale			(const sf::Vector2f& v_scale)	{}
		void setPositionScalable	(bool v_set) 					{ m_PositionScalable = v_set; }	//Enablest the position to be also scalable in the scale() function

		inline const sf::Vector2f& getPosition() 	const  	{ return m_Position; }
		inline const sf::Vector2f& getSize() 		const 	{ return m_Size; }
		inline const sf::Vector2f& getScale()		const	{ return m_Scale; }
		inline const sf::Vector2f& getOrigin()		const	{ return m_Origin; }
		inline const sf::RectangleShape& getBox()	const	{ return m_Box; }
	};
};
