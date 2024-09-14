#pragma once
#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H
#endif
#include <iostream>
#include "Drawable_calculations.h"
#include "SFML/Graphics.hpp"
#include "Button.h"

class Text_Field
{
private:
	sf::RectangleShape field;
	sf::RectangleShape caret; //a caret is the name of the rectangle that appears when you select a text prompt
	sf::Font field_font;
	sf::Text field_text;
	sf::Text entered_text;
	std::string entered_string;
	std::string censored_string; //this should put stars behind the last letter entered as to censor the entered password
	bool selected = false; //determines whether a text field has been selected, if a text field has been selected, we can allow text to be entered into it
public:
	Text_Field()
	{																		
		drawable_calculations::load_n_check_txt(field_font, field_text, "assets/fonts/prstartk.ttf");
		field_text.setCharacterSize(80);
		field_text.setScale(0.1, 0.1); 
		field_text.setFillColor(sf::Color(43, 43, 43));
		field.setFillColor(sf::Color::White);

		drawable_calculations::load_n_check_txt(field_font, entered_text, "assets/fonts/prstartk.ttf");
		entered_text.setCharacterSize(80);									 
		entered_text.setScale(0.1, 0.1);
		entered_text.setFillColor(sf::Color(43, 43, 43));
		
		field.setSize(sf::Vector2f(123, 12)); //if I was not under as many time constraints, I would change the way the text fields were sized in order to size them dynamically with windows, or any other kind of 'object' a text field is in
		caret.setSize(sf::Vector2f(1, 9));
		caret.setFillColor(sf::Color::Black);
		caret.setFillColor(sf::Color::Black);

	}
	void set_field_text(std::string new_text); //changes the text displayed next to the actual field
	void render_field(sf::RenderWindow &window); //draws the field text, the field itself, the entered text and the caret (if the field is selected)
	void set_field_position(int x, int y); //changes the position of the field and it's components
	sf::Vector2f getPos(); //returns the position of the field (the field text, specifically)
	sf::FloatRect getGlobalBounds(); //returns the global bounds of the field text
	void allow_text_entering(sf::RenderWindow &window, sf::Event &event); //allows text to be entered into the field
	void allow_censored_text_entering(sf::RenderWindow& window, sf::Event &event); //allows text to be entered into a field while also censoring it, this is useful for when a user enters personal data such as their passwords
	void clear(); //clears the entered text from a fields (the caret's positioning is handled automatically by another function)
	std::string getText(); //returns the entered text from a field
};