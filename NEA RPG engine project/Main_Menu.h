#pragma once
#ifndef MAIN_MENU_H //Makes sure to define the header if it is not defined already
#define MAIN_MENU_H
#endif
#include <iostream>
#include <iomanip>
#include <random>
#include "SFML/Graphics.hpp"
#include "Button.h"
#include "UI_Window.h"
#include "Text_Field.h"
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "hashpp.h"

class Main_Menu
{
private: 
	//maybe should add user id string in user_details or something
	Button login_button, sign_up_button, settings_button, exit_button, edit_button, play_button, log_out_button;
	sf::Font mm_header_font;
	sf::Text mm_header;
	sf::Text logged_in_as;
	sf::Text display_username;
	UI_Window mm_win; //maybe rename these
	Text_Field mm_fields[3]; 
	std::string User_details[3];
public:
	enum button_pressed
	{
		sign_up = 0, login = 1, settings = 2, go_to_editor = 3, go_to_player = 4
	};
	enum details
	{
		Username = 0, Password = 1, Password_confirmation = 2,
	};
	int what_button = -1; //determines what part of a menu the user is currently at
	Main_Menu()	
	{
		drawable_calculations::load_n_check_txt(mm_header_font, mm_header, "assets/fonts/prstartk.ttf");
		mm_header.setString("Welcome!");
		mm_header.setCharacterSize(110); //in SFML, anti-ailiasing is used by default, this ailiases the text so it doesnt look blurry
		mm_header.setScale(0.1, 0.1); //since we set a high character size on the above to stop extreme ailiasing while keeping the character size we want, we can set the scale to 0.1, thus making the character size actually 8 pixels large, this means that our text will not be ailiased which will thereby not make the text so blurry
		mm_header.setFillColor(sf::Color(43, 43, 43)); //one of the two main colours used in the UI mockup
		drawable_calculations::load_n_check_txt(mm_header_font, logged_in_as, "assets/fonts/prstartk.ttf");
		logged_in_as.setCharacterSize(110); 
		logged_in_as.setScale(0.1, 0.1); 
		logged_in_as.setFillColor(sf::Color(43, 43, 43));
		display_username.setCharacterSize(110); 
		display_username.setScale(0.1, 0.1);
		display_username.setFillColor(sf::Color(43, 43, 43));
		drawable_calculations::load_n_check_txt(mm_header_font, display_username, "assets/fonts/prstartk.ttf");
		mm_fields[details::Username].set_field_text("Username:");
		mm_fields[details::Password].set_field_text("Password:");
		mm_fields[details::Password_confirmation].set_field_text("Re-enter\nPassword:");

	}
	void construct_buttons(sf::RenderWindow& window, sf::View& view); //calls Button::full_construct() for each button
	void menu_layout(sf::RenderWindow& window, sf::View& view); //handles positioning each button, window and text element
	void handle_events(sf::RenderWindow& window, sf::View& view, sf::Event &event); //handles program operations that happen in SFML's event loop, such as mouse clicking
	void render_menu(sf::RenderWindow &window, sf::View &view); //handles drawing menu elements and positioning that needs to happen in the main loop
	void register_account(); //responsible for registering a user into their account, uses adapted code from the first development iteration
	void log_in(); //responsible for logging a user into their account, uses adapted code from the first development iteration
};