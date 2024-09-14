#pragma once
#ifndef UI_WINDOW_H
#define UI_WINDOW_H
#endif
#include <iostream>
#include "Drawable_calculations.h"
#include "SFML/Graphics.hpp"
#include "Text_Field.h"
#include "Button.h"

class UI_Window
{
private:
	std::string title_str;
	sf::Text title;
	sf::Texture window_texture;
	sf::Texture close_button_texture;
	sf::Texture enter_icon_texture;
	sf::Sprite enter_icon;
	sf::Sprite close_button;
	bool show_enter = true;
public:
	sf::Font win_font;
	sf::Sprite window_sprite; //if I was under lesser time constraints I would make the windows be dynamically sized, like buttons, currently however, they are not
	Button enter_button;
	UI_Window()
	{
		drawable_calculations::load_n_check_spr(window_texture, window_sprite, "assets/images/Main menu UI/UI Windows/full window.png");
		drawable_calculations::load_n_check_spr(close_button_texture, close_button, "assets/images/Main menu UI/UI Windows/window close button.png");
		drawable_calculations::load_n_check_txt(win_font, title, "assets/fonts/prstartk.ttf");
		drawable_calculations::load_n_check_spr(enter_icon_texture, enter_icon,"assets/images/Main menu UI/enter icon.png");
		title.setCharacterSize(80); 
		title.setScale(0.1, 0.1); 
		title.setFillColor(sf::Color(43, 43, 43));
	}
	void construct_button(sf::RenderWindow &window, sf::View &view); //constructs the enter button and handles sizing it
	void set_title(std::string new_title); //sets the window title
	void draw_ui_window(sf::RenderWindow& window, sf::View &view); //draws the window and its components, also handles positioning that needs to be done in the main loop
	void handle_events(sf::RenderWindow &window, sf::Event &event, int &change_button); //handles parts of the program that need to occur in the event loop
	bool is_button_pressed(sf::RenderWindow& window, sf::View &view, sf::Event &event);
	void set_enter(bool toggle); //handles whether the enter button should be drawn or not
	bool get_enter();
	sf::Vector2f getPos();
	sf::FloatRect getGlobalBounds();
};