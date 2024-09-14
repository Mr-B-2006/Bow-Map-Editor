#pragma once
#ifndef STATES_H
#define STATES_H
#endif
#include <iostream>
#include "SFML/Graphics.hpp"
#include "States.h"
#include "Main_Menu.h"
#include "Editor.h"

enum state_keys //these enums tell us what state the program is in
{
	main_menu = 0, editor = 1, player = 2
};

class States //this class will be used to switch between program states (main menu, editor, etc)
{
private:
	int current_state = state_keys::main_menu;
	sf::Color bg; //background colour
public:
	std::string logged_in_as; //holds the username of the currently logged in user so we know who is logged in 
	Main_Menu mm;
	Editor ed;
	States(sf::RenderWindow &window, sf::View &view)
	{
		mm.construct_buttons(window, view);
	}
	int get_current_state(); //returns the current program state
	int set_current_state(int new_state, sf::RenderWindow& window, sf::View& view); //changes the current state
	sf::Color get_current_bg(); //returns the sf::Color of the background
	sf::Color set_current_bg(sf::Color new_bg); //sets the background's sf::Color
	void update_state_events(sf::RenderWindow& window, sf::View& view, sf::Event &event); //carries out event loop functions associated with a certain state
	void update_state(sf::RenderWindow &window, sf::View &view); //carries out main loop functions associated with a certain state
};