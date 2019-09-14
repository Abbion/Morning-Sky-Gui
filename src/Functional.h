#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "GuiEssential.h"
#include <math.h>

namespace ms
{
    extern bool TEXT_CENTERING_FLAG;

    
    inline void ACCEPT_LOWER_TEXT_CENTERING_PRECISION(bool v_set)
    {
        TEXT_CENTERING_FLAG = v_set;
    }
    

    namespace fn
    {   
        //Get Corner Position for shape
        inline const sf::Vector2f GetTLCorner(const sf::Shape& Shape)
        {
            float RealOutlineThickness = Shape.getOutlineThickness();
            return sf::Vector2f(Shape.getGlobalBounds().left + RealOutlineThickness, Shape.getGlobalBounds().top + RealOutlineThickness);  
        }

        inline const sf::Vector2f GetTRCorner(const sf::Shape& Shape)
        {
            float RealOutlineThickness = Shape.getOutlineThickness();
            return sf::Vector2f(Shape.getGlobalBounds().left + Shape.getGlobalBounds().width - RealOutlineThickness, Shape.getGlobalBounds().top + RealOutlineThickness);
        }

        inline const sf::Vector2f GetBRCorner(const sf::Shape& Shape)
        {
            float RealOutlineThickness = Shape.getOutlineThickness();
            return sf::Vector2f(Shape.getGlobalBounds().left + Shape.getGlobalBounds().width - RealOutlineThickness, Shape.getGlobalBounds().top + Shape.getGlobalBounds().height - RealOutlineThickness);
        }

        inline const sf::Vector2f GetBLCorner(const sf::Shape& Shape)
        {
            float RealOutlineThickness = Shape.getOutlineThickness();
            return sf::Vector2f(Shape.getGlobalBounds().left + RealOutlineThickness, Shape.getGlobalBounds().top + Shape.getGlobalBounds().height - RealOutlineThickness);
        }
        //----------------------


        //Get more info for shape
        inline const sf::Vector2f GetOrigin(const sf::Shape& Shape)
        {
            return sf::Vector2f (Shape.getGlobalBounds().left + (Shape.getGlobalBounds().width / 2.0f),
                                Shape.getGlobalBounds().top  + (Shape.getGlobalBounds().height / 2.0f));
        }

        inline const float GetWidth(const sf::Shape& Shape)
        {
            float RealOutlineThickness = Shape.getOutlineThickness();
            return Shape.getGlobalBounds().width - (2 * RealOutlineThickness);
        }

        inline const float GetHeight(const sf::Shape& Shape)
        {
            float RealOutlineThickness = Shape.getOutlineThickness();
            return Shape.getGlobalBounds().height - (2 * RealOutlineThickness);
        }
        //----------------------


        //Get Corner Position for text
        inline const sf::Vector2f GetTLCorner(const sf::Text& Text)
        {
            return sf::Vector2f(Text.getGlobalBounds().left, Text.getGlobalBounds().top);
        }

        inline const sf::Vector2f GetTRCorner(const sf::Text& Text)
        {
            return sf::Vector2f(Text.getGlobalBounds().left + Text.getGlobalBounds().width, Text.getGlobalBounds().top);
        }

        inline const sf::Vector2f GetBRCorner(const sf::Text& Text)
        {
            return sf::Vector2f(Text.getGlobalBounds().left + Text.getGlobalBounds().width, Text.getGlobalBounds().top + Text.getGlobalBounds().height);
        }

        inline const sf::Vector2f GetBLCorner(const sf::Text& Text)
        {
            return sf::Vector2f(Text.getGlobalBounds().left, Text.getGlobalBounds().top + Text.getGlobalBounds().height);
        }
        //----------------------


        //Get more info for text
        inline const sf::Vector2f GetOrigin(const sf::Text& Text)
        {   
            //I don't know why dividing character size by 4 is working
            return sf::Vector2f (Text.getGlobalBounds().width / 2.0f, (Text.getCharacterSize() / 4) + Text.getGlobalBounds().height / 2.0f);
        }

        inline const float GetWidth(const sf::Text& Text)
        {
            return Text.getGlobalBounds().width;
        }

        inline const float GetHeight(const sf::Text& Text)
        {
            return Text.getGlobalBounds().height;
        }
        //----------------------



        //Get Coner Position for sprites
        inline const sf::Vector2f GetTLCorner(const sf::Sprite& Sprite)
        {
            return sf::Vector2f(Sprite.getGlobalBounds().left, Sprite.getGlobalBounds().top);
        }

        inline const sf::Vector2f GetTRCorner(const sf::Sprite& Sprite)
        {
            return sf::Vector2f(Sprite.getGlobalBounds().left + Sprite.getGlobalBounds().width, Sprite.getGlobalBounds().top);
        }

        inline const sf::Vector2f GetBRCorner(const sf::Sprite& Sprite)
        {
            return sf::Vector2f(Sprite.getGlobalBounds().left + Sprite.getGlobalBounds().width, Sprite.getGlobalBounds().top + Sprite.getGlobalBounds().height);
        }

        inline const sf::Vector2f GetBLCorner(const sf::Sprite& Sprite)
        {
            return sf::Vector2f(Sprite.getGlobalBounds().left, Sprite.getGlobalBounds().top + Sprite.getGlobalBounds().height);
        }
        //----------------------

        //Get more info for sprites
        inline const sf::Vector2f GetOrigin(const sf::Sprite& Sprite)
        {   
            return sf::Vector2f (Sprite.getTextureRect().width / 2, Sprite.getTextureRect().height / 2);
        }
        //----------------------

        
        //Other usefull functions
        inline const sf::Vector2f GetOriginOfOneLineText(const sf::Text& v_text)
        {
            std::string TextStr = v_text.getString();
            float MaxHeight = 0;
            for (int i = 0; i < TextStr.size(); i++)
            {
                if(TextStr.at(i) == '\n')
                {
                    if(!TEXT_CENTERING_FLAG)
                    {
                        std::cout << "msGUI ERROR: Class: NONE, File: Functional.h, Function: GetOriginOfOneLineText() -> could not be used because the text has a new line character.\n" << 
                                        "Another text centering function has been used but it might be not super precise.\n" << 
                                        "To get rid of this error use [ACCEPT_LOWER_TEXT_CENTERING_PRECISION(true)]\n";
                    }
                    return GetOrigin(v_text);
                }

                sf::Uint32 character = TextStr.at(i);
		        MaxHeight = std::max(v_text.getFont()->getGlyph(character, v_text.getCharacterSize(), false).bounds.height, MaxHeight);
            }

            return sf::Vector2f(v_text.getGlobalBounds().width / 2, v_text.getCharacterSize() - (MaxHeight / 2));
        }
        //----------------------
    };


    //DEGUG that helped me with coding 
    namespace dbg
    {
        inline const sf::RectangleShape DrawTextBounds(const sf::Text& Text)
        {
            sf::RectangleShape Bounds;
            Bounds.setPosition(sf::Vector2f(Text.getGlobalBounds().left, Text.getGlobalBounds().top));
            Bounds.setSize(sf::Vector2f(Text.getGlobalBounds().width, Text.getGlobalBounds().height));
            Bounds.setOutlineColor(sf::Color::Green);
            Bounds.setOutlineThickness(1.0f);
            Bounds.setFillColor(sf::Color::Transparent);
            return Bounds;
        }

        inline const sf::RectangleShape DrawTextOrigin(const sf::Text& Text)
        {
            sf::RectangleShape Bounds;
            Bounds.setSize(sf::Vector2f(2, 2));
            Bounds.setOrigin(Bounds.getSize() / 2.0f);
            Bounds.setPosition(Text.getPosition());
            Bounds.setFillColor(sf::Color::Blue);
            return Bounds;
        }

        inline const sf::RectangleShape DrawOrigin(const sf::Sprite& Sprite)
        {
            sf::RectangleShape Bounds;
            Bounds.setSize(sf::Vector2f(2, 2));
            Bounds.setOrigin(Bounds.getSize() / 2.0f);
            Bounds.setPosition(Sprite.getPosition());
            Bounds.setFillColor(sf::Color::Blue);
            return Bounds;
        }

        inline const sf::RectangleShape DrawRectangleFromFloatRect(const sf::FloatRect& fRect)
        {
            sf::RectangleShape Rect;
            Rect.setPosition(sf::Vector2f(fRect.left, fRect.top));
            Rect.setSize(sf::Vector2f(fRect.width, fRect.height));
            Rect.setOutlineThickness(1);
            Rect.setOutlineColor(sf::Color::Yellow);
            Rect.setFillColor(sf::Color::Transparent);
            return Rect;
        }
    };
};